
//Pause.h
#pragma once

#include "direct3d.h"
#include "fade.h"

class PAUSE
{
//private:
//	FadeObject* m_Fade;

public:
	FadeObject* m_Fade;
	void Pause_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Pause_Finalize();
	void Pause_Update();
	void Pause_Draw();

};
