
//Title.h
#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Audio.h"
#include "Manager.h"

class TITLE
{
private:
	FadeObject* m_Fade;
	int	g_SeID {NULL};

public:
	void Title_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade);
	void Title_Finalize();
	void Title_Update();
	void Title_Draw();

};
