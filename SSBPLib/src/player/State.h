﻿#pragma once

#include "SS5PlayerTypes.h"
#include <memory>
#include "math/SSRect.h"
#include "math/Matrix.h"
#include "InstancePartStatus.h"
#include "EffectPartStatus.h"
#include "PlayerDef.h"

namespace ss{
class DataArrayReader;
struct AnimationInitialData;


/**
 * State
 * パーツの情報を格納します。Stateの内容をもとに描画処理を作成してください。
 */
struct State{
	int flags;						/// このフレームで更新が行われるステータスのフラグ
	int cellIndex;					/// パーツに割り当てられたセルの番号
	float x;						/// SS5アトリビュート：X座標
	float y;						/// SS5アトリビュート：Y座標
	float z;						/// SS5アトリビュート：Z座標
	float pivotX;					/// 原点Xオフセット＋セルに設定された原点オフセットX
	float pivotY;					/// 原点Yオフセット＋セルに設定された原点オフセットY
	float rotationX;				/// X回転
	float rotationY;				/// Y回転
	float rotationZ;				/// Z回転
	float scaleX;					/// Xスケール
	float scaleY;					/// Yスケール
	int opacity;					/// 不透明度（0～255）
	float size_X;					/// SS5アトリビュート：Xサイズ
	float size_Y;					/// SS5アトリビュート：Xサイズ
	float uv_move_X;				/// SS5アトリビュート：UV X移動
	float uv_move_Y;				/// SS5アトリビュート：UV Y移動
	float uv_rotation;				/// SS5アトリビュート：UV 回転
	float uv_scale_X;				/// SS5アトリビュート：UV Xスケール
	float uv_scale_Y;				/// SS5アトリビュート：UV Yスケール
	float boundingRadius;			/// SS5アトリビュート：当たり半径
	
	bool flipX;						/// 横反転
	bool flipY;						/// 縦反転
	bool isVisibled;				/// 非表示


	BlendType colorBlendVertexFunc;	/// SS5アトリビュート：カラーブレンドのブレンド方法
	int colorBlendVertexFlags;		/// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
	SSV3F_C4B_T2F_Quad quad;		/// 頂点データ、座標、カラー値、UVが含まれる（頂点変形、サイズXY、UV移動XY、UVスケール、UV回転、反転が反映済）
	TextuerData texture;			/// セルに対応したテクスチャ番号（ゲーム側で管理している番号を設定する）
	SSRect rect;					/// セルに対応したテクスチャ内の表示領域（開始座標、幅高さ）
	BlendType blendfunc;			/// パーツに設定されたブレンド方法
	Matrix mat;						/// パーツの位置を算出するためのマトリクス（親子関係計算済）
																
	
	InstancePartStatus instanceValue;	//インスタンスアトリビュート
	EffectPartStatus effectValue;		//エフェクトアトリビュート


	//readerを介してデータを読み取る
	void readData(DataArrayReader& reader, const AnimationInitialData* init);



	//現在のStateの情報を元にuvを計算する
	void uvCompute(SSV3F_C4B_T2F_Quad* q, SSTex2F uv_tl/*top,left*/, SSTex2F uv_br/*bottom,right*/) const;
	//現在のStateの情報を元にverexを計算する
	void vertexCompute(SSV3F_C4B_T2F_Quad* q, const SSRect& cellRect/*, const SSQuad3& vertexTransform*/) const;
	////現在のStateの情報を元にmatrixを計算する。setIdentityから始めます
	//void matrixCompute(Matrix *matrix) const;

	void init();

	//bool operator==(const State &s) const;
	//bool operator!=(const State &s) const;

	State() { init(); }
};

} //namespace ss
