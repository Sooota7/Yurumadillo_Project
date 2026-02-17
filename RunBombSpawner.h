#pragma once



#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"RunBombSource.h"

using namespace DirectX;


class RUNBOMBSPAWNER
{
private:
	XMFLOAT3	m_Position;
	XMFLOAT3	m_Rotation;
	XMFLOAT3	m_Scaling;

	RUNBOMBSOURCE m_RunBomb;

	bool		m_Active;
	float		m_RCoolTime;

	bool		m_Use = false;

	RUNBOMB_TYPE m_Type;

	const float W = 60.0f;


public:


	void Initialize_RunBombSpawner(XMFLOAT3 pos, RUNBOMB_TYPE Type);
	void Finalize_RunBombSpawner();
	void Update_RunBombSpawner(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	void Draw_RunBombSpawner();

	void SetPosition_RunBombSpawner(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetPosition_RunBombSpawner() { return m_Position; };

	void SetUse(bool use) { m_Use = use; };
	bool GetUse() { return m_Use; };

	RUNBOMBSPAWNER* GetRunBombSpawner_RunBombSpawner() { return this; };
	RUNBOMBSOURCE* GetRunBombSource__RunBombSpawner() { return &m_RunBomb; };
};