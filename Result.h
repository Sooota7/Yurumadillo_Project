
//.h
#pragma once

#include "direct3d.h"
#include "fade.h"


class RESULT
{
	FadeObject* m_Fade;


public:
	void Result_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Result_Finalize();
	void Result_Update();
	void Result_Draw();
};
