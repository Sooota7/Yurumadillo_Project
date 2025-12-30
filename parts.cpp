#include "parts.h"
#include	"Camera.h"
#include	"shader.h"

//ボールオブジェクト

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;



void PARTS::PartsSet(XMFLOAT3 position, XMFLOAT3 rotation)
{
	m_Position = position;
	m_Rotation = rotation;
	
}

void PARTS::PartsInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* model)
{

	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Model = ModelLoad(model);
}

void PARTS::PartsFinalize()
{
}

void PARTS::PartsUpdate()
{
}

void PARTS::PartsDraw()
{

	//ワールド行列作成
	XMMATRIX	scale = XMMatrixScaling(
		m_Scaling.x,
		m_Scaling.y,
		m_Scaling.z);
	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
		m_Rotation.x,
		m_Rotation.y,
		m_Rotation.z);
	XMMATRIX	translation = XMMatrixTranslation(
		m_Position.x,
		m_Position.y,
		m_Position.z);
	XMMATRIX	world = scale * rotation * translation;

	//変換行列作成
	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();
	XMMATRIX	wvp = world * view * projection;

	//シェーダーへ行列をセット
	Shader_SetWorldMatrix(world);
	Shader_SetMatrix(wvp);

	ModelDraw(m_Model);

}
