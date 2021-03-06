﻿#pragma once
#include <string>
#include "SS5PlayerTypes.h"
#include "math/Matrix.h"
#include "player/PlayerDef.h"
#include "player/InstancePartStatus.h"
#include "player/EffectPartStatus.h"
#include "player/UserData.h"

namespace ss{
struct UserData;
class InstancePartStatus;
class EffectPartStatus;


/** ロードイベントなどを捕まえるのでこれを継承して作ってください */
class SS5EventListener{
public:
	SS5EventListener(){}
	virtual ~SS5EventListener(){}


	//テクスチャのロード・リリースのイベント。内部ではPlayer単位で管理されます
	virtual TextureID TextureLoad(int cellMapIndex, const std::string& texturePath, SsTexWrapMode wrapmode, SsTexFilterMode filtermode) = 0;
	virtual void TextureRelease(TextureID textureId) = 0;

	//描画
	virtual void DrawSprite(const SSV3F_C4B_T2F_Quad& quad, TextureID textureId, BlendType blendType, BlendType colorBlendVertexType) = 0;

	
	/**
	 * 再生フレームに制限をかけます
	 * @param frame		これからPlayerが処理したいフレーム番号
	 * @param maxFrame	アニメーションの総フレーム数
	 * @return 制限をかけた後のフレーム番号
	 */
	virtual int LimitFrame(int frame, int maxFrame){
		return ss::wrap<int>(frame, 0, maxFrame);		//ループ再生になります
		
		//例:
		//return ss::clamp<int>(frame, 0, maxFrame-1);	//最終フレームで止める
		//return ss::wrap<int>(frame, 3, 7);			//3～6フレームでループさせる
		//if(frame>10){ return 5; }else{ return frame; }//10フレームを過ぎたら5フレームに飛ばす
	}

	/**
	 * ユーザーデータがあったときに呼ばれる
	 * @param userData	一時オブジェクトなのでコピーして使ってください
	 * @param frame		userDataが設定されているフレーム
	 */
	virtual void OnUserData(const UserData& userData, int frame){}

	
	
	//ChildPlayer -----------------------------------------
	/**
	 * インスタンスアニメーションのロード・リリースのイベント
	 * @param parentPartIndex	親になるパーツのindex
	 * @param animName			再生アニメーション名
	 */
	virtual void ChildPlayerLoad(int parentPartIndex, const std::string& animName){}
	virtual void ChildPlayerRelease(int parentPartIndex){}

	/**
	 * 更新時に呼び出されるイベント。
	 * 親のパーツの情報を伝播させるために必要になります。
	 * Player内部ではChildPlayerの制御はしないため、このイベントを活用してください。
	 * @param parentPartIndex	親パーツのindex
	 * @param parentWorldMatrix	親パーツのワールド行列(アタッチするなら、子供のPlayerにセットしてください)
	 * @param parentAlpha		親パーツのアルファ値[0:1]
	 * @param parentFrame		親の再生フレーム
	 * @param instanceAttribute	インスタンスアニメーションの再生制御情報
	 */
	virtual void ChildPlayerUpdate(
		int parentPartIndex, const Matrix& parentWorldMatrix, float parentAlpha,
		int parentFrame, const InstancePartStatus& instanceAttribute
	){
		/* int frame = instanceAttribute.getFrame(parentFrame); */
	}

	/** 描画イベント */
	virtual void ChildPlayerDraw(int parentPartIndex){}

	
	//Effect ----------------------------------------------
	/**
	 * エフェクトのロード・リリースのイベント
	 * @param parentPartIndex	親になるパーツのindex
	 * @param effectName		エフェクト名
	 * @param seed				エフェクトのseed値
	 */
	virtual void EffectLoad(int parentPartIndex, const std::string& effectName, int seed){}
	virtual void EffectRelease(int parentPartIndex){}
	
	/**
	 * 更新時に呼び出されるイベント。
	 * 親のパーツの情報を伝播させるために必要になります。
	 * Player内部ではEffectの制御はしないため、このイベントを活用してください。
	 * @param parentPartIndex	親パーツのindex
	 * @param parentWorldMatrix	親パーツのワールド行列(アタッチするなら、子供のPlayerにセットしてください)
	 * @param parentAlpha		親パーツのアルファ値[0:1]
	 * @param parentFrame		親の再生フレーム
	 * @param parentSeedOffset	親のランダムSeed値
	 * @param effectAttribute	エフェクトの再生制御情報
	 */
	virtual void EffectUpdate(
		int parentPartIndex, const Matrix& parentWorldMatrix, float parentAlpha,
		int parentFrame, int parentSeedOffset, const EffectPartStatus& effectAttribute
	){
		/* float frame = effectAttribute.getFrame(parentFrame); */
	}

	/** 描画イベント */
	virtual void EffectDraw(int parentPartIndex){}
	
};


} //namespace ss

