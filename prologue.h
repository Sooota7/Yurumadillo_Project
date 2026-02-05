
//Prologue.h
#pragma once

#include "direct3d.h"
#include "fade.h"

#define MAX_PROLOGUE_TEXTURE	(4)

class PROLOGUE
{
private:
	FadeObject* m_Fade;
	int	g_SeID{ NULL };

public:
	void Prologue_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Prologue_Finalize();
	void Prologue_Update();
	void Prologue_Draw();

};
