
//.h
#pragma once

#include "direct3d.h"
#include "fade.h"
#include "Manager.h"

class GAMEOVER
{
	FadeObject* m_Fade;
	MANAGER* m_Manager;

public:
	void GameOver_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade,MANAGER*manager);
	void GameOver_Finalize();
	void GameOver_Update();
	void GameOver_Draw();
};
