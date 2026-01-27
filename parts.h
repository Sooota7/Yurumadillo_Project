#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"
#include	"anim.h"

//#define FRAME_MAX (30)

//class FRAME
//{
//	XMFLOAT3 m_Position;//座標
//	XMFLOAT3 m_Rotation;//回転率
//	XMFLOAT3 m_Scaling;	//拡大率
//
//public:
//	void InitializeFrame();
//	void FinalizeFrame();
//
//	void		SetPosition(XMFLOAT3 pos) { m_Position = pos; };
//	XMFLOAT3	GetPosition() { return m_Position; };
//
//	void		SetRotation(XMFLOAT3 rot) { m_Rotation = rot; };
//	XMFLOAT3	GetRotation() { return m_Rotation; };
//
//	void		SetScaling(XMFLOAT3 sca) { m_Scaling = sca; };
//	XMFLOAT3	GetScaling() { return m_Scaling; };
//
//};

#define LASTPOS (10)

class PARTS
{
private:
	XMFLOAT3 m_Position;//座標
	XMFLOAT3 m_Velocity;//移動値
	XMFLOAT3 m_Rotation;//回転率
	XMFLOAT3 m_Scaling;	//拡大率

	//初期位置を入れる場所
	XMFLOAT3 m_AnimInisPosition;
	int lasPosMax;
	int NowPos;

	ANIM fps;

	//目標地点、動きのゴール
	XMFLOAT3 m_AnimLastPosition[LASTPOS];
	XMFLOAT3 m_AnimLastPosition_Use;

	//合計のプラス値
	XMFLOAT3 m_AnimNowPosition;

	bool	 m_Loop;

	
public:

	//FRAME m_Frame[FRAME_MAX];

	void	 SetlasPosMax(int pos) { lasPosMax = pos; };
	int		 GetlasPosMax() { return lasPosMax; };
	void	 SetNowMax(int pos) { NowPos = pos; };
	int		 GetNowPos() { return NowPos; };
	void	 SetInisPosition(XMFLOAT3 pos) { m_AnimInisPosition = pos; };
	XMFLOAT3 GetInisPosition() { return m_AnimInisPosition; };
	void	 SetNowPosition(XMFLOAT3 pos) { m_AnimNowPosition = pos; };
	XMFLOAT3 GetNowPosition() { return m_AnimNowPosition;};
	void	 SetAnimLastPosition(XMFLOAT3 pos,int no) { m_AnimLastPosition[no] = pos; };
	XMFLOAT3 GetAnimLastPosition(int no) { return m_AnimLastPosition[no]; };
	void	 SetAnimLastPositionUse(XMFLOAT3 pos) { m_AnimLastPosition_Use = pos; };
	XMFLOAT3 GetAnimLastPositionUse() { return m_AnimLastPosition_Use; };
	void	 SetAnimLoop(bool bo) { m_Loop = bo; };
	bool	 GetAnimLoop() { return m_Loop; };

	//void	 PartsAnimUpdate();

public:
	void	PartsSet(XMFLOAT3 position,XMFLOAT3 rotation);
	void	PartsInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const char* model);
	void	PartsFinalize();
	void	PartsUpdate();
	void	PartsDraw();
	
	MODEL*	m_Model;		//モデル

	void SetPartsPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetPartsPosition() { return m_Position; };

	void SetPartsRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetPartsRotation() { return m_Rotation; };

	void SetPartsScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetPartsScaling() { return m_Scaling; };

	void SetPartsVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetPartsVelocity() { return m_Velocity; };

	
};