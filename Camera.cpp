//Camera.cpp


#include	"Camera.h"
#include	"keyboard.h"
#include	"inputx.h"
#include    "mouse.h"
//#include	"Ball.h"
//#include	"billboard.h"

#include "player.h"
//グローバル変数
static	CAMERA	CameraObject;

XMFLOAT3		g_BallPosOld;

static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


// 調整可
static float rangeCamera = 4.5f;   // カメラとプレイヤーの距離
static float cameraHeight = 0.5f;      // カメラの高さ
static float cameraYoko = 180.0f;      // 横調整
static float cameraTate = 20.0f;     // 縦調整

static bool inputC = InputKeyKonCheck();

void	Camera_Initialize(XMFLOAT3 BallPos)
{
	CameraObject.Position = XMFLOAT3(0.0f, 3.0f, -4.0f);
	CameraObject.AtPosition = XMFLOAT3(0.0f, 1.0f, 0.0f);
	CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	CameraObject.Fov = 45.0f;
	float width = (float)Direct3D_GetBackBufferWidth();
	float height = (float)Direct3D_GetBackBufferHeight();
	CameraObject.Aspect = width / height;
	CameraObject.NearClip = 0.5f;
	CameraObject.FarClip = 1000.0f;

	g_BallPosOld = BallPos;


	g_pDevice = Direct3D_GetDevice();
	g_pContext = Direct3D_GetDeviceContext();

	
}

void	Camera_Finalize()
{
	return;
}
void Camera_Update(XMFLOAT3 BallPos)
{
	if (inputC) {
		// ================================
		// マウス入力の取得
		// ================================
		Mouse_State ms;
		Mouse_GetState(&ms);

		// 相対モード時だけカメラ回転
		if (ms.positionMode == MOUSE_POSITION_MODE_RELATIVE)
		{
			float sensitivity = CAMERA_SENSITIVITY;

			cameraYoko += ms.x * sensitivity; // 左右
			cameraTate += ms.y * sensitivity; // 上下

			// 上下移動限界
			if (cameraTate > CAMERA_UP_MAX) cameraTate = CAMERA_UP_MAX;
			if (cameraTate < CAMERA_DOWN_MAX)  cameraTate = CAMERA_DOWN_MAX;

			// 左右移動限界
			if (cameraYoko >= CAMERA_SIDE_MAX) cameraYoko -= CAMERA_SIDE_MAX;
			if (cameraYoko < 0.0f)    cameraYoko += CAMERA_SIDE_MAX;
		}

		//左右回転調整 
		if (Keyboard_IsKeyDown(KK_LEFT))
		{
			cameraYoko += 2.0f;
			if (cameraYoko >= CAMERA_SIDE_MAX) cameraYoko -= CAMERA_SIDE_MAX;
		}
		if (Keyboard_IsKeyDown(KK_RIGHT))
		{
			cameraYoko -= 2.0f;
			if (cameraYoko < 0.0f) cameraYoko += CAMERA_SIDE_MAX;
		}
		//ピッチ調整 
		if (Keyboard_IsKeyDown(KK_UP))
		{
			cameraTate += 2.0f;
			if (cameraTate > CAMERA_UP_MAX) cameraTate = CAMERA_UP_MAX; // 上限 
		}
		if (Keyboard_IsKeyDown(KK_DOWN))
		{
			cameraTate -= 2.0f;
			if (cameraTate < CAMERA_DOWN_MAX) cameraTate = CAMERA_DOWN_MAX; // 下限 
		}
	}
	else {
		//左右回転調整 
		if ((GetThumbRightX(0) >= 0.5f))
		{
			cameraYoko += 2.0f;
			if (cameraYoko >= CAMERA_SIDE_MAX) cameraYoko -= CAMERA_SIDE_MAX;
		}
		if ((GetThumbRightX(0) <= -0.5f))
		{
			cameraYoko -= 2.0f;
			if (cameraYoko < 0.0f) cameraYoko += CAMERA_SIDE_MAX;
		}
		//ピッチ調整 
		if ((GetThumbRightY(0) <= -0.5f))
		{
			cameraTate += 2.0f;
			if (cameraTate > CAMERA_UP_MAX) cameraTate = CAMERA_UP_MAX; // 上限 
		}
		if ((GetThumbRightY(0) >= 0.5f))
		{
			cameraTate -= 2.0f;
			if (cameraTate < CAMERA_DOWN_MAX) cameraTate = CAMERA_DOWN_MAX; // 下限 
		}
	}

	// ================================
	// 注視点（プレイヤー）
	// ================================
	CameraObject.AtPosition = BallPos;
	CameraObject.AtPosition.y += 0.4f;		// 改善の余地あり？

	// ================================
	// カメラ座標計算
	// ================================
	float radYoko = XMConvertToRadians(cameraYoko);
	float radTate = XMConvertToRadians(cameraTate);

	float horiz = cosf(radTate) * rangeCamera;

	float offsetX = sinf(radYoko) * horiz;
	float offsetZ = cosf(radYoko) * horiz;
	float offsetY = sinf(radTate) * rangeCamera;

	CameraObject.Position.x = BallPos.x + offsetX;
	CameraObject.Position.y = BallPos.y + offsetY + cameraHeight;
	CameraObject.Position.z = BallPos.z + offsetZ;

	CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// FOV キーボード調整はそのまま
	if (Keyboard_IsKeyDown(KK_Z))
	{
		CameraObject.Fov += 0.3f;
		if (CameraObject.Fov > 160.0f) CameraObject.Fov = 160.0f;
	}
	if (Keyboard_IsKeyDown(KK_X))
	{
		CameraObject.Fov -= 0.3f;
		if (CameraObject.Fov < 5.0f) CameraObject.Fov = 5.0f;
	}
}

void	Camera_Draw()
{ 
	//プロジェクション行列作成
	CameraObject.Projection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(CameraObject.Fov),
		CameraObject.Aspect,
		CameraObject.NearClip,
		CameraObject.FarClip
	);

	//ビュー行列作成
	XMVECTOR	vpos = XMVectorSet(
		CameraObject.Position.x,
		CameraObject.Position.y+1.5f,
		CameraObject.Position.z,
		0.0f);
	XMVECTOR	vAt = XMVectorSet(
		CameraObject.AtPosition.x,
		CameraObject.AtPosition.y+1.5f,
		CameraObject.AtPosition.z,
		0.0f
	);
	XMVECTOR	vUp = XMVectorSet(
		CameraObject.UpVector.x,
		CameraObject.UpVector.y,
		CameraObject.UpVector.z,
		0.0f
	);
	CameraObject.View = XMMatrixLookAtLH(
		vpos,
		vAt,
		vUp
	);

	//SetDepthTest(TRUE);
	//SetBlendState(BLENDSTATE_ALFA);
	////テクスチャのセット
	//g_pContext->PSSetShaderResources(0, 1, &g_Texture);
	//XMFLOAT3 pos = CameraObject.Position;
	//pos.z += 1.0f;
	//XMFLOAT2 size = XMFLOAT2(0.5f, 0.5f);
	//XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//int bno = 1;
	//int wc = 1;
	//int hc = 1;

	//Billboard bb(pos, size, col, bno, wc, hc);
	//bb.Billboard_Draw();

	
	return;

}

void	SetCameraFov(float fov)
{
	CameraObject.Fov = fov;
}
void	SetCameraAspect(float asp)
{ 
	CameraObject.Aspect = asp;
}
void	SetCameraClip(float n, float f)
{ 
	CameraObject.NearClip = n;
	CameraObject.FarClip = f;
}

void	SetCameraPosition(XMFLOAT3 pos)
{
	CameraObject.Position = pos;
}
void	SetCameraAtPosition(XMFLOAT3 at)
{
	CameraObject.AtPosition = at;
}
void	SetCameraUpVector(XMFLOAT3 up)
{ 
	CameraObject.UpVector = up;
}

XMMATRIX	GetViewMatrix()
{ 
	return	CameraObject.View;
}
XMMATRIX	GetProjectionMatrix()
{
	return	CameraObject.Projection;
}

XMFLOAT3	GetCameraPosition()
{
	return CameraObject.Position;
}

XMFLOAT3	GetCameraAtPosition()
{
	return CameraObject.AtPosition;
}

float GetCameraYoko()
{
	return cameraYoko;
}