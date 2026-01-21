#pragma once


#include	"model.h"
#include	"bombSource.h"
#include	"gimmickButton.h"
#include	"gimmickField.h"
#include	"gimmickGate.h"
#include	"player.h"
// マクロ定義

#define		GIMMICK_NUM_MAX	(10)
#define		GIMMICK_TYPE_MAX	(2)

enum GIMMICK_STATE
{
	GIMMICK_STATE_NONE,
	GIMMICK_STATE_BUTTON,
	GIMMICK_STATE_FIELD,
	GIMMICK_STATE_GATE,
	GIMMICK_STATE_MAX
};


class GIMMICK_DATA
{
private:

	//5秒後爆発
	const int limitTime{ 5 };

	GIMMICK_BUTTON		m_GimmickButton[GIMMICK_NUM_MAX];
	GIMMICK_FIELD		m_GimmickField[GIMMICK_NUM_MAX];
	GIMMICK_GATE		m_GimmickGate[GIMMICK_NUM_MAX];

	MODEL* m_Model[GIMMICK_STATE_MAX] = { NULL };//デバッグ

public:

	void	Gimmick_Data_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
	void	Gimmick_Data_Finalize(void);
	void	Gimmick_Data_Draw(void);
	void	Gimmick_Data_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	
	GIMMICK_DATA* GimmickData_GetGimmickData() { return this; };
};