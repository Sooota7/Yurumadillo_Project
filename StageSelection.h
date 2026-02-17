
//StageSelection.h

#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Manager.h"

class STAGESELECTION
{
private:
	FadeObject* m_Fade;
	MANAGER* m_Manager;

	int stageselect;

public:
	void StageSelection_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade, MANAGER* manager);
	void StageSelection_Finalize();
	void StageSelection_Update();
	void StageSelection_Draw();

};