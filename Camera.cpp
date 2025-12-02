//Camera.cpp


#include	"Camera.h"
#include	"keyboard.h"
//#include	"Ball.h"

#include "player.h"
//グローバル変数
static	CAMERA	CameraObject;

XMFLOAT3		g_BallPosOld;

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

}

void	Camera_Finalize()
{
	return;
}
void	Camera_Update(XMFLOAT3 BallPos)
{


	// 調整可
	static float rangeCamera = 4.5f;   // カメラとプレイヤーの距離
	static float cameraHeight = 0.5f;      // カメラの高さ
	static float cameraYoko = 180.0f;      // 横調整
	static float cameraTate = 20.0f;     // 縦調整

	//左右回転調整
	if (Keyboard_IsKeyDown(KK_LEFT)) {
		cameraYoko += 2.0f;
		if (cameraYoko >= 360.0f) cameraYoko -= 360.0f;
	}
	if (Keyboard_IsKeyDown(KK_RIGHT)) {
		cameraYoko -= 2.0f;
		if (cameraYoko < 0.0f) cameraYoko += 360.0f;
	}

	//ピッチ調整
	if (Keyboard_IsKeyDown(KK_UP)) {
		cameraTate += 2.0f;
		if (cameraTate > 80.0f) cameraTate = 80.0f; // 上限
	}
	if (Keyboard_IsKeyDown(KK_DOWN)) {
		cameraTate -= 2.0f;
		if (cameraTate < 5.0f) cameraTate = 5.0f; // 下限
	}

	// 注視点は常にプレイヤー
	CameraObject.AtPosition = BallPos;

	// カメラ座標の計算
	float radYoko = XMConvertToRadians(cameraYoko);
	float radTate = XMConvertToRadians(cameraTate);

	float horiz = cosf(radTate) * rangeCamera;
	float offsetX = sinf(radYoko) * horiz; // 左右
	float offsetZ = cosf(radYoko) * horiz; // 前後
	float offsetY = sinf(radTate) * rangeCamera; // 高さ

	CameraObject.Position.x = BallPos.x + offsetX;
	CameraObject.Position.y = BallPos.y + offsetY + cameraHeight;
	CameraObject.Position.z = BallPos.z + offsetZ;


	CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// FOVの変更
	if (Keyboard_IsKeyDown(KK_Z))
	{
		CameraObject.Fov += 0.3f;
		if (CameraObject.Fov > 160.0f)
		{
			CameraObject.Fov = 160.0f;
		}

	}
	if (Keyboard_IsKeyDown(KK_X))
	{
		CameraObject.Fov -= 0.3f;
		if (CameraObject.Fov < 5.0f)
		{
			CameraObject.Fov = 5.0f;
		}
	}

	return;
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
		CameraObject.Position.y,
		CameraObject.Position.z,
		0.0f);
	XMVECTOR	vAt = XMVectorSet(
		CameraObject.AtPosition.x,
		CameraObject.AtPosition.y,
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

