#pragma once
#ifndef _BOMB_H_
#define _BOMB_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include	"model.h"
#include	"bombSource.h"
#include	"RunBombSpawner.h"
#include	"FlowtBombSource.h"
#include	"Dictionary.h"
#include	 "player.h"
#include     "billboardManager.h"
#include	"BombFlowtAnim.h"


// マクロ定義

#define		BOMB_NUM_MAX	(100)
#define		BOMB_TYPE_MAX	(2)

#define		BOMB_TRAIL_MAX	(10)

class BOMBTRAIL
{
public:
	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Rotation;		//爆弾の座標
	XMFLOAT3	m_Scaling;		//爆弾の座標
};

class BOMB
{
private:

	//5秒後爆発
	const int limitTime{ 5 };


	BOMBSOURCE		m_Bomb[BOMB_NUM_MAX];
	int				m_Bbno[BOMB_NUM_MAX];
	RUNBOMBSPAWNER	m_RunBomb[BOMB_NUM_MAX];
	int				m_Rbno[BOMB_NUM_MAX];
	FLOWTBOMBSOURCE m_FlowtBomb[BOMB_NUM_MAX];
	int				m_Fbno[BOMB_NUM_MAX];

	// 追加: 前フレームの状態を保持して遷移を検出するための配列
	BOMB_STATE		m_PrevState[BOMB_NUM_MAX];

	MODEL*			m_Model[BOMB_MAX] = { NULL };//デバッグ
	MODEL* m_BombModel[BOMB_TYPE::TYPE_MAX];
	MODEL* m_ItemModel[BOMB_TYPE::TYPE_MAX];
	MODEL* m_NorBombModel;

	MODEL* m_ModelData[BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX];	//モデルデータ
	MODEL* m_ModelDataBomb[BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX];	//モデルデータ

	MODEL** GetBombFlowtModelData() { return m_ModelData; }
	MODEL** GetBombFlowtModelDataBomb() { return m_ModelDataBomb; }


	ID3D11ShaderResourceView* m_Texture = NULL;

public:

	void	Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,FIELD_NO no);
	void	Bomb_Finalize(void);
	void	Bomb_Draw(BillboardManager* billboardManager);
	void	Bomb_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	// 変更：Boss用の更新は PLAYER* を受け取る
	void	Bomb_Update_Boss(PLAYER* pPlayer,BOSSMONSTER* pBossmonster);
	BOMBSOURCE* Bomb_GetBomb();
	RUNBOMBSPAWNER* Bomb_GetRunBomb();
	FLOWTBOMBSOURCE* Bomb_GetFlowtBomb();
	void Bomb_SetBoss(BOSSMONSTER* boss);


	void	Bomb_Trail_Draw();
	
};

#endif // !_BOMB_H_



