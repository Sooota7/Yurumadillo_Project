#pragma once

//BossObj.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"model.h"
using namespace DirectX;

#define BOSSOBJ_MAX (4)
#define BOSSATK_RANGE (200.0f)
#define BOSSOBJ_RADIUS (1.0f)
#define BOSSOBJ_DAMAGE (50)

//ボスの攻撃オブジェクトのクラス
class BOSSOBJ
{
private:
	XMFLOAT3	m_Position;	//ポジション
	XMFLOAT3	m_Rotation;	//回転
	XMFLOAT3	m_Scaling;	//大きさ
	XMFLOAT3	m_Velocity;	//進行方向
	XMFLOAT3	m_Acceleration;	// 加速

	MODEL* m_Model[BOSSOBJ_MAX];		//モデルデータ

	bool m_IsActive = false;


	int randmodel;

private:
	

public:
	void	BossObj_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	BossObj_Finalize();
	void	BossObj_Update();
	void	BossObj_Draw();

	void SetBossObjPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetBossObjPosition() { return m_Position; };

	void SetBossObjRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetBossObjRotation() { return m_Rotation; };

	void SetBossObjScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetBossObjScaling() { return m_Scaling; };

	void SetBossObjVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetBossObjVelocity() { return m_Velocity; };

	void SetActive(bool a) { m_IsActive = a; }
	bool IsActive() const { return m_IsActive; }

	

	BOSSOBJ* GetBossObj() { return this; };

};


