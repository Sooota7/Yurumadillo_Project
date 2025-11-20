
//fade.h

#pragma once

#include "direct3d.h"

#include "sprite.h"
#include "dictionary.h"

enum FADE_STATE
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,

};

class FadeObject
{
private:
	FADE_STATE	state;			//フェード処理状態
	float		count;			//カウンター
	float		frame;			//フェード処理時間
	XMFLOAT4	fadecolor;		//フェード色
	SCENE		scene;
	
public:
	void Fade_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Fade_Finalize();
	void Fade_Update();
	void Fade_Draw();

	void	Fade_SetFade(int fadeframe, XMFLOAT4 color, FADE_STATE sta, SCENE sce);
	SCENE	Fade_GetScene() { return scene; };
	
	float Fade_GetColorW() { return fadecolor.w; };

	FADE_STATE	GetFadeState();
};




