#pragma once

#include <memory>
#include "DxLib.h"
#include "ss/SS5ResourceManager.h"
#include "ss/SS5EventListener.h"

namespace ss{
	class SS5Player;
}


class EventListener : public ss::SS5EventListener{
public:
	//テクスチャのロード・リリースのイベント。内部ではPlayer単位で管理されます
	ss::TextureID TextureLoad(int cellMapIndex, const std::string& texturePath, ss::SsTexWrapMode wrapmode, ss::SsTexFilterMode filtermode) override{
		SS_LOG("【EVENT】(SSTextureLoad) name:%s", texturePath.c_str());
		return LoadGraph(texturePath.c_str());	//テクスチャ読みこみ
	}
	void TextureRelease(ss::TextureID textureId) override{
		SS_LOG("【EVENT】(SSTextureRelease) id%d", static_cast<int>(textureId));
		DeleteGraph(textureId);					//テクスチャ開放
	}


	//描画
	void DrawSprite(const ss::SSV3F_C4B_T2F_Quad& quad, ss::TextureID textureId, ss::BlendType blendType, ss::BlendType colorBlendVertexType) override{
		//DXライブラリ用の頂点バッファを作成する
		VERTEX_3D vertex[4] = {
			vertex3Dfrom(quad.tl),
			vertex3Dfrom(quad.bl),
			vertex3Dfrom(quad.tr),
			vertex3Dfrom(quad.br)
		};
		//3Dプリミティブの表示
		DrawPolygon3DBase(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP, textureId, true);
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
};



class QuickStart{
public:
	QuickStart();
	~QuickStart();

	void initialize();
	void finalize();

	void update();
	void draw();

private:
	
	ss::SS5ResourceManager m_ss5ResourceManager;	//ssbpファイルの登録先
	
	ss::SS5Player *m_ss5Player;						//ssbpファイルの再生単位
	EventListener m_eventListener;					//SS5Player向けのイベント処理の実装
};

