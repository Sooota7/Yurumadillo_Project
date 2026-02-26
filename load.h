#pragma once


#include "direct3d.h"

enum LOAD_ANIM
{
	LOAD_ANIM_01,
	LOAD_ANIM_02,
	LOAD_ANIM_03,
	LOAD_ANIM_04,
	LOAD_ANIM_MAX
};

class LOAD_MANAGER
{
private:
	int m_LoadCount;
	float m_LoadTime;
	bool m_LoadActive;
	bool m_LoadComplete;

	LOAD_ANIM m_LoadAnim[LOAD_ANIM_MAX];
	float m_LoadAnimCount;
	bool m_NextLoad;

public:

	void Load_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Load_Finalize();
	void Load_Update();
	void Load_Draw();

	int GetLoadCount() { return m_LoadCount; }
	float GetLoadTime() { return m_LoadTime; }

	void SetLoadActive(bool active) { m_LoadActive = active; }
	bool GetLoadActive() { return m_LoadActive; }

	void SetLoadComplete(bool complete) { m_LoadComplete = complete; }
	bool GetLoadComplete() { return m_LoadComplete; }

	void SetLoadAnim();

};

//
//switch (m_Load.GetLoadCount())
//{
//case 0:
//	break;
//case 1:
//	break;
//case 2:
//	break;
//case 3:
//	break;
//case 4:
//	break;
//case 5:
//	break;
//case 6:
//	break;
//case 7:
//	break;
//case 8:
//	break;
//case 9:
//	break;
//case 10:
//	break;
//case 11:
//	break;
//case 12:
//	break;
//case 13:
//	break;
//case 14:
//	break;
//case 15:
//	break;
//case 16:
//	break;
//default:
//	break;
//}