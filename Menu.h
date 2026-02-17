
//Menu.h

#pragma once

#include "direct3d.h"
#include "fade.h"

class MENU
{
private:
	FadeObject* m_Fade;

	int tutorialselect;

public:
	void Menu_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Menu_Finalize();
	void Menu_Update();
	void Menu_Draw();

};