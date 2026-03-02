//=================================================================
// 
// BossAnimation.h
// 2026/02/19		藤原　海斗
// 
//=================================================================
#pragma once
#include "anim.h"
#include "parts.h"
#include "model.h"

#define BOMBFLOWT_ANIM_SPEED (1.0f / 3.0f)  // 1.0fで60f=1秒、1/3で180f=3秒

enum BOMBFLOWT_ANIMATION_STATE
{
	BOMBFLOWT_STATE_NONE = 0,	//通常
	BOMBFLOWT_STATE_IDLE,	//通常
	BOMBFLOWT_STATE_MOVE,		//ムーブ
	BOMBFLOWT_STATE_MAX
};

enum BOMBFLOWT_PARTS
{
	BOMBFLOWT_PARTS_BODY = 0,
	BOMBFLOWT_PARTS_ARM_RIGHT,
	BOMBFLOWT_PARTS_ARM_LEFT,
	BOMBFLOWT_PARTS_MAX,
};

class BOMBFLOWT_ANIMATION
{
public:
	ANIM m_Anim[BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX];
};

class BOMBFLOWTANIMATION
{
private:
	
	PARTS m_Parts[BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX];		//パーツ(ポジション、ローテーション、スケーリング)
	BOMBFLOWT_ANIMATION m_BossAnim[BOMBFLOWT_STATE_MAX];						//アニメーションデータ(モーションデータ)

	float m_FrameCnt;	//フレームカウント
	BOMBFLOWT_ANIMATION_STATE m_State;	//ステート
	bool m_AnimLoop;	//ループするかどうか

public:
	void	BombFlowtAnim_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	BombFlowtAnim_Finalize();
	void	BombFlowtAnim_Update(XMFLOAT3 bossPos, XMFLOAT3 bossRot);
	void	BombFlowtAnim_Draw(MODEL* model, MODEL* model2, MODEL* model3);

	void	BombFlowtAnim_Hokan(BOMBFLOWT_ANIMATION_STATE state);
	void	BombFlowtAnim_InisAnim();

	void	BombFlowtAnim_SetAnimWait();
	void	BombFlowtAnim_SetAnimMove();
	
	void	BombFlowtAnim_UpdateHead(XMFLOAT3 playerPos);
	XMFLOAT3	BombFlowtAnim_Positiion(BOMBFLOWT_ANIMATION_STATE state, BOMBFLOWT_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame);
	XMFLOAT3	BombFlowtAnim_Rotation(BOMBFLOWT_ANIMATION_STATE state, BOMBFLOWT_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame);

	BOMBFLOWT_ANIMATION* GetBombFlowtAnim() { return m_BossAnim; }
	PARTS* GetBombFlowtParts() { return m_Parts; }

	void SetBombFlowtAnimState(BOMBFLOWT_ANIMATION_STATE state);
	BOMBFLOWT_ANIMATION_STATE GetBombFlowtAnimState() { return m_State; }

	void SetBombFlowtAnimFrameCnt(float frame) { m_FrameCnt = frame; }
	float GetBombFlowtAnimFrameCnt() { return m_FrameCnt; }

	void SetBombFlowtAnimLoop(bool bo) { m_AnimLoop = bo; }
	bool GetBombFlowtAnimLoop() { return m_AnimLoop; }

};




