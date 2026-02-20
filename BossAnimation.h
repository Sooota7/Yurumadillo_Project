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

#define BOSS_ANIM_SPEED (1.0f / 3.0f)  // 1.0fで60f=1秒、1/3で180f=3秒

enum BOSS_ANIMATION_STATE
{
	BOSS_STATE_NONE = 0,	//通常
	BOSS_STATE_IDLE,	//通常
	BOSS_STATE_MOVE,		//ムーブ
	BOSS_STATE_PHASE01,		//攻撃
	BOSS_STATE_PHASE02_01,		//攻撃
	BOSS_STATE_PHASE02_02,		//攻撃
	BOSS_STATE_PHASE03,		//攻撃
	BOSS_STATE_DAMAGE,		//ダメージ
	BOSS_STATE_DEATH,		//死
	BOSS_STATE_MAX
};

enum BOSS_PARTS
{
	BOSS_PARTS_HEAD = 0,
	BOSS_PARTS_BODY,
	BOSS_PARTS_ARM_RIGHT,
	BOSS_PARTS_ARM_LEFT,
	BOSS_PARTS_MAX,
};

class BOSS_ANIMATION
{
public:
	ANIM m_Anim[BOSS_PARTS::BOSS_PARTS_MAX];
};

class BOSSANIMATION
{
	private:
		MODEL* m_ModelData[BOSS_PARTS::BOSS_PARTS_MAX];	//モデルデータ
		PARTS m_Parts[BOSS_PARTS::BOSS_PARTS_MAX];		//パーツ(ポジション、ローテーション、スケーリング)
		BOSS_ANIMATION m_BossAnim[BOSS_STATE_MAX];						//アニメーションデータ(モーションデータ)

		float m_FrameCnt;	//フレームカウント
		BOSS_ANIMATION_STATE m_State;	//ステート
		bool m_AnimLoop;	//ループするかどうか

public:
	void	BossAnimation_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	BossAnimation_Finalize();
	void	BossAnimation_Update(XMFLOAT3 bossPos,XMFLOAT3 bossRot);
	void	BossAnimation_Draw();

	void	BossAnimation_Hokan(BOSS_ANIMATION_STATE state);
	void	BossAnimation_InisAnim();

	void	BossAnimation_SetAnimWait();
	void	BossAnimation_SetAnimMove();
	void	BossAnimation_SetAnimPhase01();
	void	BossAnimation_SetAnimPhase02_01();
	void	BossAnimation_SetAnimPhase02_02();
	void	BossAnimation_SetAnimPhase03();
	void	BossAnimation_SetAnimDamage();
	void	BossAnimation_SetAnimDeath();

	void	BossAnimation_UpdateHead(XMFLOAT3 playerPos);
	XMFLOAT3	BossAnimation_AnimationPositiion(BOSS_ANIMATION_STATE state, BOSS_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame);
	XMFLOAT3	BossAnimation_AnimationRotation(BOSS_ANIMATION_STATE state, BOSS_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame);

	BOSS_ANIMATION* GetBossAnim() { return m_BossAnim; }
	PARTS* GetBossParts() { return m_Parts; }
	MODEL** GetBossModelData() { return m_ModelData; }

	void SetBossAnimState(BOSS_ANIMATION_STATE state);
	BOSS_ANIMATION_STATE GetBossAnimState() { return m_State; }

	void SetBossAnimFrameCnt(float frame) { m_FrameCnt = frame; }
	float GetBossAnimFrameCnt() { return m_FrameCnt; }

	void SetBossAnimLoop(bool bo) { m_AnimLoop = bo; }
	bool GetBossAnimLoop() { return m_AnimLoop; }

};




