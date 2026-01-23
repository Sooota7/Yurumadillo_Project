#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;


#define BUTTON_PRESS_DEPTH (0.40f) // 押下で沈む距離(Y-)
#define BUTTON_MOVE_SPEED (5.0f)  // unit/sec


class GIMMICK_BUTTON
{
public:


	XMFLOAT3	m_FirstPosition;		//最初の座標
	XMFLOAT3	m_Position;				//ボタンの座標
	XMFLOAT3	m_Velocity;				//速度
	
	bool		m_Touch;				// 接触フラグ
	int 	    m_Channel = 0;				// チャンネル番号

public:

	void		GimmickButton_Initialize(XMFLOAT3 pos);
	void		GimmickButton_Finalize(void);
	void		GimmickButton_Update(void);



	//セッター及びゲッター
	void		GimmickButton_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	GimmickButton_GetPosition() { return m_Position; };

	void		GimmickButton_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	GimmickButton_GetVelocity() { return m_Velocity; };

	void		GimmickButton_SetTouch(bool check) { m_Touch = check; };
	bool		GimmickButton_GetTouch() { return m_Touch; };
	
	void        GimmickButton_SetChannel(int ch) { m_Channel = ch; }
	int         GimmickButton_GetChannel() const { return m_Channel; }

	GIMMICK_BUTTON* GetGimmickButton() { return this; };
};

