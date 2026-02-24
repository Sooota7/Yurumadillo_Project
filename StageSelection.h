
//StageSelection.h

#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Manager.h"

#define STAGE_CHANGE_TIME (1.5f)

class STAGESELECTION
{
private:
	FadeObject* m_Fade;
	MANAGER* m_Manager;

	int stageselect;
	int stagechangecounter;
public:
	void StageSelection_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade, MANAGER* manager);
	void StageSelection_Finalize();
	void StageSelection_Update();
	void StageSelection_Draw();

};