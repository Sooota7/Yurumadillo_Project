#include	"BossObj.h"
#include	"shader.h"
#include	"collision.h"
#include	"Camera.h"

//敵攻撃オブジェクト
ID3D11Device* g_pDeviceBossObj;
ID3D11DeviceContext* g_pContextBossObj;

static ID3D11ShaderResourceView* g_Texture = NULL;

void BOSSOBJ::BossObj_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDeviceBossObj = pDevice;
	g_pContextBossObj = pContext;

	float downSize = 10.0f;

	m_Position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(5.0f, 5.0f, 5.0f);

	m_Model[0] = ModelLoad("asset\\model\\EnemyGround\\EnemyGroundWeapon.fbx");
	m_Model[1] = ModelLoad("asset\\model\\test.fbx");
	m_Model[2] = ModelLoad("asset\\model\\test.fbx");
	m_Model[3] = ModelLoad("asset\\model\\test.fbx");
}

void	BOSSOBJ::BossObj_Finalize()
{
	for(int i = 0; i < BOSSOBJ_MAX; i++) 
	{
			ModelRelease(m_Model[i]);
	}
}

void BOSSOBJ::BossObj_Update()
{
	if (!m_IsActive) return;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// 画面外に出たら消す
	if (m_Position.z < -BOSSATK_RANGE||m_Position.z>FIELD_WIDTH_Z / 4*3)
	{
		m_IsActive = false;
	}
}

void	BOSSOBJ::BossObj_Draw()
{
	if (!m_IsActive) return;

	//ワールド行列作成
	XMMATRIX	scale = XMMatrixScaling(
		m_Scaling.x,
		m_Scaling.y,
		m_Scaling.z);
	// --- ここを修正: m_Rotation は度 (°) でセットされることがあるため描画時にラジアンへ変換する ---
	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_Rotation.x),
		XMConvertToRadians(m_Rotation.y),
		XMConvertToRadians(m_Rotation.z));
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

	
	ModelDraw(m_Model[0]);


	{
		XMFLOAT3 pos = m_Position;
		pos.y += 1.0f;
		XMFLOAT2 size = XMFLOAT2(0.5f, 0.5f);
		XMFLOAT4 col = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		int bno = 1;
		int wc = 1;
		int hc = 1;

		Billboard* bb = new Billboard(pos, size, col, bno, wc, hc, BILLBOARD_TEXTURE::TEST);
	}

}

