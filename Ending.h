
//Ending.h
#pragma once

#include "direct3d.h"
#include "fade.h"

class ENDING
{
private:
	FadeObject* m_Fade;

public:
	void Ending_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Ending_Finalize();
	void Ending_Update();
	void Ending_Draw();

};
