#pragma once

#include <vector>
#include <string>
#include "DxLib.h"
#include "ss/SS5EventListener.h"
#include "ss/SS5ResourceManager.h"
#include "ss/SS5Effect.h"

#define SSBP_REGIST_NAME "ssbpRegistName"

class EventListener : public ss::SS5EventListener{
public:
	EventListener(ss::SS5ResourceManager* resMgr) :m_resMgr(resMgr){}
	~EventListener(){
		for(int textureId : m_textures){
			DeleteGraph(textureId);
		}
		m_textures.clear();
	}

	//テクスチャを事前に読み込みするためのコールバック
	void texturePreloadCallback(int cellMapIndex, const std::string& filename, ss::SsTexWrapMode wrapmode, ss::SsTexFilterMode filtermode){
		int textureId = LoadGraph(filename.c_str());
		m_textures.push_back(textureId);		//必ずcellMapIndex順(0～)呼ばれるので、push_backすればok
	}


	//テクスチャのロード・リリースのイベント。内部ではPlayer単位で管理されます
	ss::TextureID TextureLoad(int cellMapIndex, const std::string& texturePath, ss::SsTexWrapMode wrapmode, ss::SsTexFilterMode filtermode) override{
		int textureId = m_textures[cellMapIndex];	//テクスチャは事前に読み込みしておくため、ここではreturnするだけ
		SS_LOG("【EVENT】(SSTextureLoad) name:%s, id:%d", texturePath.c_str(), textureId);
		return textureId;
	}
	void TextureRelease(ss::TextureID textureId) override{
		SS_LOG("【EVENT】(SSTextureRelease) id%d", static_cast<int>(textureId));
	}


	//描画 //ひとまずSS5PlayerPlatform.cppの中身を持ってきた。
	void DrawSprite(const ss::SSV3F_C4B_T2F_Quad& quad, ss::TextureID textureId, ss::BlendType blendType, ss::BlendType colorBlendVertexType) override{
		//描画ファンクション
		switch(blendType){
		case ss::BLEND_MIX:		///< 0 ブレンド（ミックス）
			if(quad.tl.colors.a == 255 && quad.tr.colors.a == 255 && quad.bl.colors.a == 255 && quad.br.colors.a == 255){
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
			else{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			}
			break;
		case ss::BLEND_MUL:		///< 1 乗算
			SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
			break;
		case ss::BLEND_ADD:		///< 2 加算
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			break;
		case ss::BLEND_SUB:		///< 3 減算
			SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
			break;
		}

	
		//頂点カラーブレンド
		//厳密に再現するには専用のシェーダーを使い、テクスチャにカラー値を合成する必要がある
		//作成する場合はssShader_frag.h、CustomSpriteのコメントとなってるシェーダー処理を参考にしてください。
		switch(colorBlendVertexType){
		case ss::BLEND_MIX:		///< 0 これは、頂点カラーの色にするのが正しいらしい
			break;
		case ss::BLEND_MUL:		///< 1 乗算
			// ブレンド方法は乗算以外未対応
			// とりあえず左上の色を反映させる
			SetDrawBright(quad.tl.colors.r, quad.tl.colors.g, quad.tl.colors.b);
			break;
		case ss::BLEND_ADD:		///< 2 加算
			break;
		case ss::BLEND_SUB:		///< 3 減算
			break;
		}

		//DXライブラリ用の頂点バッファを作成する
		VERTEX_3D vertex[4] = {
			vertex3Dfrom(quad.tl),
			vertex3Dfrom(quad.bl),
			vertex3Dfrom(quad.tr),
			vertex3Dfrom(quad.br)
		};
		//3Dプリミティブの表示
		DrawPolygon3DBase(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP, textureId, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドステートを通常へ戻す
		SetDrawBright(255, 255, 255);
	}


	//エフェクトのイベント ----------------------------------------------------
	void EffectLoad(int parentPartIndex, const std::string& effectName, int seed) override{
		assert(m_effects.find(parentPartIndex) == m_effects.end());
		//エフェクトを生成する
		ss::SS5Effect* effect = m_resMgr->createEffect(this, SSBP_REGIST_NAME, effectName, seed);
		m_effects.insert(std::make_pair(parentPartIndex, effect));
	}
	void EffectRelease(int parentPartIndex) override{
		auto it = m_effects.find(parentPartIndex);
		assert(it != m_effects.end());
		
		//エフェクトを破棄する
		m_resMgr->destroyEffect(it->second);
		m_effects.erase(it);
	}

	void EffectUpdate(
		int parentPartIndex, const ss::Matrix& parentWorldMatrix, float parentAlpha,
		int parentFrame, int parentSeedOffset, const ss::EffectPartStatus& effectAttribute
	) override{
		auto it = m_effects.find(parentPartIndex);
		assert(it != m_effects.end());

		ss::SS5Effect* effect = it->second;
		effect->effectUpdate(parentWorldMatrix, parentAlpha, parentFrame, parentSeedOffset, effectAttribute);
	}
	
	void EffectDraw(int parentPartIndex) override{
		auto it = m_effects.find(parentPartIndex);
		assert(it != m_effects.end());
		
		ss::SS5Effect* effect = it->second;
		effect->draw();
	}


private:
	//DXライブラリ用頂点バッファ作成関数
	static VERTEX_3D vertex3Dfrom(const ss::SSV3F_C4B_T2F &vct){
		VERTEX_3D v = {
			{ vct.vertices.x, vct.vertices.y, vct.vertices.z },
			vct.colors.b, vct.colors.g, vct.colors.r, vct.colors.a,
			vct.texCoords.u(), vct.texCoords.v()
		};
		return v;
	}
	
	std::vector<int> m_textures;

	ss::SS5ResourceManager* m_resMgr;
	std::map<int, ss::SS5Effect*> m_effects;
};

