﻿#pragma once
#include "State.h"
#include "SS5Player.h"
#include "math/Matrix.h"

namespace ss{
class SsEffectRenderV2;

/**
 * CustomSprite
 */
class CustomSprite{
private:
	bool				_flipX;
	bool				_flipY;

public:
	Matrix				_mat;
	State				_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	bool				_haveChildPlayer;		//ChildPlayer(インスタンスアニメーション)を持っているならtrue


	//エフェクト用パラメータ
	SsEffectRenderV2*	refEffect;

	//モーションブレンド用ステータス
	State				_orgState;

	//エフェクト制御用ワーク
	bool effectAttrInitialized;
	float effectTimeTotal;

public:
	CustomSprite();
	virtual ~CustomSprite();


	void setState(const State& state){
		_isStateChanged = true;
		_state = state;
	}
};

} //namespace ss
