
//Ending.h
#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Manager.h"

class ENDING
{
private:
	FadeObject* m_Fade;
	MANAGER* m_Manager;

public:
	void Ending_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade, MANAGER* manager);
	void Ending_Finalize();
	void Ending_Update();
	void Ending_Draw();

};
