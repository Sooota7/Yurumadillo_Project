
//Title.h
#pragma once

#include "direct3d.h"
#include "fade.h"

class TITLE
{
private:
	FadeObject* m_Fade;

public:
	void Title_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Title_Finalize();
	void Title_Update();
	void Title_Draw();

};
