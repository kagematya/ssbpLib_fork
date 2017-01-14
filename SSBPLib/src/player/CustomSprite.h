#pragma once
#include "State.h"
#include "SS5Player.h"

namespace ss{

/**
 * CustomSprite
 */
class CustomSprite
{
private:
	static unsigned int ssSelectorLocation;
	static unsigned int	ssAlphaLocation;
	static unsigned int	sshasPremultipliedAlpha;

	//	static CCGLProgram* getCustomShaderProgram();

private:
	//	CCGLProgram*	_defaultShaderProgram;
	bool				_useCustomShaderProgram;
	float				_opacity;
	int					_hasPremultipliedAlpha;
	int					_colorBlendFuncNo;
	bool				_flipX;
	bool				_flipY;

public:
	float				_mat[16];
	State				_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	Player*				_ssplayer;
	float				_liveFrame;
	SSV3F_C4B_T2F_Quad	_sQuad;

	//�G�t�F�N�g�p�p�����[�^
	SsEffectRenderV2*	refEffect;
	SsPartState			partState;

	//���[�V�����u�����h�p�X�e�[�^�X
	State				_orgState;

	//�G�t�F�N�g����p���[�N
	bool effectAttrInitialized;
	float effectTimeTotal;

	//Ver4�݊��p���[�N
	SsVector3		_temp_position;
	SsVector3		_temp_rotation;
	SsVector2		_temp_scale;

public:
	CustomSprite();
	virtual ~CustomSprite();

	static CustomSprite* create();

	void initState()
	{
		_state.init();
		_isStateChanged = true;
	}

	void setStateValue(float& ref, float value)
	{
		if(ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(int& ref, int value)
	{
		if(ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(bool& ref, bool value)
	{
		if(ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setStateValue(SSV3F_C4B_T2F_Quad& ref, SSV3F_C4B_T2F_Quad value)
	{
		//		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}

	void setState(const State& state)
	{
		setStateValue(_state.flags, state.flags);
		setStateValue(_state.cellIndex, state.cellIndex);
		setStateValue(_state.x, state.x);
		setStateValue(_state.y, state.y);
		setStateValue(_state.z, state.z);
		setStateValue(_state.pivotX, state.pivotX);
		setStateValue(_state.pivotY, state.pivotY);
		setStateValue(_state.rotationX, state.rotationX);
		setStateValue(_state.rotationY, state.rotationY);
		setStateValue(_state.rotationZ, state.rotationZ);
		setStateValue(_state.scaleX, state.scaleX);
		setStateValue(_state.scaleY, state.scaleY);
		setStateValue(_state.opacity, state.opacity);
		setStateValue(_state.size_X, state.size_X);
		setStateValue(_state.size_Y, state.size_Y);
		setStateValue(_state.uv_move_X, state.uv_move_X);
		setStateValue(_state.uv_move_Y, state.uv_move_Y);
		setStateValue(_state.uv_rotation, state.uv_rotation);
		setStateValue(_state.uv_scale_X, state.uv_scale_X);
		setStateValue(_state.uv_scale_Y, state.uv_scale_Y);
		setStateValue(_state.boundingRadius, state.boundingRadius);
		setStateValue(_state.isVisibled, state.isVisibled);
		setStateValue(_state.flipX, state.flipX);
		setStateValue(_state.flipY, state.flipY);
		setStateValue(_state.blendfunc, state.blendfunc);
		setStateValue(_state.colorBlendFunc, state.colorBlendFunc);
		setStateValue(_state.colorBlendType, state.colorBlendType);

		setStateValue(_state.quad, state.quad);
		_state.texture = state.texture;
		_state.rect = state.rect;
		memcpy(&_state.mat, &state.mat, sizeof(_state.mat));

		setStateValue(_state.instanceValue_curKeyframe, state.instanceValue_curKeyframe);
		setStateValue(_state.instanceValue_startFrame, state.instanceValue_startFrame);
		setStateValue(_state.instanceValue_endFrame, state.instanceValue_endFrame);
		setStateValue(_state.instanceValue_loopNum, state.instanceValue_loopNum);
		setStateValue(_state.instanceValue_speed, state.instanceValue_speed);
		setStateValue(_state.instanceValue_loopflag, state.instanceValue_loopflag);
		setStateValue(_state.effectValue_curKeyframe, state.effectValue_curKeyframe);
		setStateValue(_state.effectValue_startTime, state.effectValue_startTime);
		setStateValue(_state.effectValue_speed, state.effectValue_speed);
		setStateValue(_state.effectValue_loopflag, state.effectValue_loopflag);

		_state.Calc_rotationX = state.Calc_rotationX;
		_state.Calc_rotationY = state.Calc_rotationY;
		_state.Calc_rotationZ = state.Calc_rotationZ;
		_state.Calc_scaleX = state.Calc_scaleX;
		_state.Calc_scaleY = state.Calc_scaleY;
		_state.Calc_opacity = state.Calc_opacity;

	}


	// override
	virtual void draw(void);
	virtual void setOpacity(unsigned char opacity);

	// original functions
	void changeShaderProgram(bool useCustomShaderProgram);
	bool isCustomShaderProgramEnabled() const;
	void setColorBlendFunc(int colorBlendFuncNo);
	SSV3F_C4B_T2F_Quad& getAttributeRef();

	void setFlippedX(bool flip);
	void setFlippedY(bool flip);
	bool isFlippedX();
	bool isFlippedY();
	void sethasPremultipliedAlpha(int PremultipliedAlpha);

public:
};

} //namespace ss