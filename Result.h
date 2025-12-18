
//.h
#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Manager.h"

class RESULT
{
	FadeObject* m_Fade;
	MANAGER* m_Manager;

public:
	void Result_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade,MANAGER*manager);
	void Result_Finalize();
	void Result_Update();
	void Result_Draw();
};
