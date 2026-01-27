#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;
//
//enum PLAYER_PARTS
//{
//	PARTS_HEAD,
//	PARTS_BODY,
//	PARTS_ARM_RIGHT,
//	PARTS_ARM_LEFT,
//	PARTS_LEG_RIGHT,
//	PARTS_LEG_LEFT,
//	PARTS_MAX
//};

#define FLAME_MAX (60)

class FLAME
{
public:
	//XMFLOAT3 m_fps[FLAME_MAX];
	bool	SetPos;
	bool	Fill;
	XMFLOAT3	Position;
	XMFLOAT3	Rotation;

};

class ANIM
{
public:
	FLAME fps[FLAME_MAX];

	void SetInisFlame(int flame_no, XMFLOAT3 pos, XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f)) { fps[flame_no].Position = pos; fps[flame_no].SetPos = true; fps[flame_no].Fill = true; fps[flame_no].Rotation = rot; };

	void AnimInitialize();

};
//
//XMFLOAT3 Anim_Head_Move[FLAME_MAX]
//{
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//};
//XMFLOAT3 Anim_Body_Move[FLAME_MAX]
//{
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//};
//
//
//XMFLOAT3 Anim_ArmR_Move[FLAME_MAX]
//{ 
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.5f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//	{0.0f,0.0f,0.0f},
//};
//XMFLOAT3 Anim_ArmL_Move[FLAME_MAX]
//{
//
//
//};
//XMFLOAT3 Anim_LegR_Move[FLAME_MAX]
//{
//
//
//};
//XMFLOAT3 Anim_LegL_Move[FLAME_MAX]
//{
//
//
//};
