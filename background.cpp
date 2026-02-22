// Background.cpp
#include "background.h"
#include "Camera.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;


//背景（天球）オブジェクト
BACKGROUND	g_Background;
static ID3D11Device* g_pDevice;
static ID3D11DeviceContext* g_pContext;

//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

#define BACKGROUND_ROTATION_SPEED		(2.0f / 60.0f)

void BACKGROUND::Background_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO fn)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	SetBackground(fn);
}

void BACKGROUND::Background_Finalize(void)
{
	ModelRelease(g_Background.Model);
}

void BACKGROUND::Background_Draw(void)
{
	// 平行移動行列作成
	XMMATRIX	TranslationMatrix =
		XMMatrixTranslation(
			g_Background.Position.x,
			g_Background.Position.y,
			g_Background.Position.z
		);
	// 回転行列作成
	XMMATRIX	RotationMatrix =
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_Background.Rotation.x),
			XMConvertToRadians(g_Background.Rotation.y),
			XMConvertToRadians(g_Background.Rotation.z)
		);
	// スケーリング行列作成
	XMMATRIX	ScalingMatrix =
		XMMatrixScaling(
			g_Background.Scaling.x,
			g_Background.Scaling.y,
			g_Background.Scaling.z
		);

	// ワールド行列作成 ※乗算の順番に注意
	XMMATRIX	world =
		ScalingMatrix * RotationMatrix * TranslationMatrix;

	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();
	XMMATRIX	wvp = world * view * projection;

	// wvp行列をセット
	Shader_SetMatrix(wvp);

	ModelDraw(g_Background.Model);

}

void BACKGROUND::Background_Update(void)
{
	// カメラの位置を取得して座標を決める
	// 今回はカメラと同じ座標で表示しておく
	g_Background.Position = GetCameraPosition();

	// 適当に回転させる
	//g_Background.Rotation.y += g_Background.Speed;
}

BACKGROUND* BACKGROUND::GetBackground()
{
	return &g_Background;
}

void BACKGROUND::SetBackground(FIELD_NO fn)
{
	switch (fn)
	{
	case FIELD_NO::NO_1: // きのこ
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 100.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_kinoko11.fbx");
		break;
	case FIELD_NO::NO_2: // ダンジョン
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 100.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_dungeon.fbx");
		break;
	case FIELD_NO::NO_3: // きのこ
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 100.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_dungeon.fbx");
		break;
	case FIELD_NO::NO_4: // お茶会
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 100.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_teaparty02.fbx");
	case FIELD_NO::NO_5:
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 50.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_castle12.fbx");
	case FIELD_NO::NO_6:
		g_Background.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Background.Rotation = XMFLOAT3(0.0f, -90.0f, 0.0f);
		g_Background.Scaling = XMFLOAT3(100.0f, 50.0f, 100.0f);

		g_Background.Speed = 0.0f;

		g_Background.Model = ModelLoad("asset\\model\\sky_castle12.fbx");
	default:
		break;
	}
}