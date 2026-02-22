//=================================================================
// 
// BossAnimation.cpp
// 2026/02/19		藤原　海斗
// 
//=================================================================
#include "BossAnimation.h"
#include	"Camera.h"
#include	"shader.h"

void BOSSANIMATION::BossAnimation_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < BOSS_PARTS::BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].PartsInitialize(pDevice, pContext);
		m_Parts[i].SetPartsScaling(XMFLOAT3(10.0f, 10.0f, 10.0f));
		switch (i)
		{
		case BOSS_PARTS_HEAD:
			m_ModelData[i] = ModelLoad("asset\\model\\BossHead.fbx");
			break;
		case BOSS_PARTS_BODY:
			m_ModelData[i] = ModelLoad("asset\\model\\BossBody.fbx");
			break;
		case BOSS_PARTS_ARM_RIGHT:
			m_ModelData[i] = ModelLoad("asset\\model\\BossHandR.fbx");
			break;
		case BOSS_PARTS_ARM_LEFT:
			m_ModelData[i] = ModelLoad("asset\\model\\BossHandL.fbx");
			break;
		default:
			break;
		}	
	}

	for (int i = 0; i <BOSS_STATE_MAX; i++)
	{
		for (int y = 0; y <BOSS_PARTS_MAX; y++)
		{
			m_BossAnim[i].m_Anim[y].AnimInitialize();
		}
	}

	m_FrameCnt = 0.0f;
	m_State = BOSS_STATE_NONE;
	m_AnimLoop = false;

	BossAnimation_InisAnim();

}

void BOSSANIMATION::BossAnimation_Finalize()
{
	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].PartsFinalize();
		if (m_ModelData[i] != NULL)
		{
			ModelRelease(m_ModelData[i]);
			m_ModelData[i] = NULL;
		}
	}
}

void BOSSANIMATION::BossAnimation_Update(XMFLOAT3 bossPos,XMFLOAT3 bossRot)
{


	if (m_State != BOSS_STATE_NONE)
	{
		float l = m_FrameCnt;

		for (int j = 0; j < BOSS_PARTS::BOSS_PARTS_MAX; j++)
		{
			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 plasRot = XMFLOAT3(0.0f, 0.0f, 0.0f);

			XMFLOAT3 pos = bossPos;
			XMFLOAT3 rot = bossRot;

			switch (j)
			{
			case BOSS_PARTS_HEAD:
				position = BossAnimation_AnimationPositiion(m_State, BOSS_PARTS_HEAD, &m_Parts[j], pos, rot, (int)l);
				plasRot  = BossAnimation_AnimationRotation(m_State, BOSS_PARTS_HEAD, &m_Parts[j], pos, rot, (int)l);
				break;
			case BOSS_PARTS_BODY:
				position = BossAnimation_AnimationPositiion(m_State, BOSS_PARTS_BODY, &m_Parts[j], pos, rot, (int)l);
				plasRot = BossAnimation_AnimationRotation(m_State, BOSS_PARTS_BODY, &m_Parts[j], pos, rot, (int)l);
				break;
			case BOSS_PARTS_ARM_RIGHT:
				position = BossAnimation_AnimationPositiion(m_State, BOSS_PARTS_ARM_RIGHT, &m_Parts[j], pos, rot, (int)l);
				plasRot = BossAnimation_AnimationRotation(m_State, BOSS_PARTS_ARM_RIGHT, &m_Parts[j], pos, rot, (int)l);
				break;
			case BOSS_PARTS_ARM_LEFT:
				position = BossAnimation_AnimationPositiion(m_State, BOSS_PARTS_ARM_LEFT, &m_Parts[j], pos, rot, (int)l);
				plasRot = BossAnimation_AnimationRotation(m_State, BOSS_PARTS_ARM_LEFT, &m_Parts[j], pos, rot, (int)l);
				break;
			default:
				break;
			}


			rot.x += XMConvertToRadians(plasRot.x);
			rot.y += XMConvertToRadians(plasRot.y);
			rot.z += XMConvertToRadians(plasRot.z);


			switch (j)
			{
			case BOSS_PARTS_HEAD:
				m_Parts[j].PartsSet(position, rot);
				break;
			case BOSS_PARTS_BODY:
				//pos = Player_LeftLeg2(&m_Model[i], rot);
				m_Parts[j].PartsSet(position, rot);
				break;
			case BOSS_PARTS_ARM_RIGHT:
				//pos = Player_LeftLeg2(&m_Model[i], rot);
				m_Parts[j].PartsSet(position, rot);
				break;
			case BOSS_PARTS_ARM_LEFT:
				//pos = Player_LeftLeg2(&m_Model[i], rot);
				m_Parts[j].PartsSet(position, rot);
				break;
			default:
				break;
			}
		}
		/*l += (1.0f / 1.0f);
		if (l >= 60)
		{
			l = 0;
		}*/
	}

	m_FrameCnt += BOSS_ANIM_SPEED; //ボスのアニメーションだけスピードを変えるため
	if (m_FrameCnt >= 60.0f)
	{
		//一旦ループ前提
		m_FrameCnt = 0.0f;
		m_AnimLoop = false;
	}
}

void BOSSANIMATION::BossAnimation_Draw()
{

	for (int i = 0; i < BOSS_PARTS::BOSS_PARTS_MAX; i++)
	{
		XMFLOAT3 m_Position = m_Parts[i].GetPartsPosition();
		XMFLOAT3 m_Rotation = m_Parts[i].GetPartsRotation();	
		XMFLOAT3 m_Scaling = m_Parts[i].GetPartsScaling();

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

		switch (i)
		{
		case BOSS_PARTS_HEAD:
			ModelDraw(m_ModelData[i]);
			break;
		case BOSS_PARTS_BODY:
			ModelDraw(m_ModelData[i]);
			break;
		case BOSS_PARTS_ARM_RIGHT:
			ModelDraw(m_ModelData[i]);
			break;
		case BOSS_PARTS_ARM_LEFT:
			ModelDraw(m_ModelData[i]);
			break;
		default:
			break;
		}

	
	}
}


void BOSSANIMATION::BossAnimation_InisAnim()
{


	for (int i = 0; i < BOSS_PARTS::BOSS_PARTS_MAX; i++)
	{
		switch (i)
		{
		case BOSS_PARTS_HEAD:
			m_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 4.0f, 0.0f));
			break;
		case BOSS_PARTS_BODY:
			m_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			break;
		case BOSS_PARTS_ARM_RIGHT:
			m_Parts[i].SetInisPosition(XMFLOAT3(10.0f, 0.0f, 0.0f));
			break;
		case BOSS_PARTS_ARM_LEFT:
			m_Parts[i].SetInisPosition(XMFLOAT3(-10.0f, 0.0f, 0.0f));
			break;
		default:
			break;
		}

	}


	BossAnimation_SetAnimWait();
	BossAnimation_SetAnimMove();
	BossAnimation_SetAnimPhase01();
	BossAnimation_SetAnimPhase02_01();
	BossAnimation_SetAnimPhase02_02();
	BossAnimation_SetAnimPhase03();
	BossAnimation_SetAnimDamage();
	BossAnimation_SetAnimDeath();
}

void BOSSANIMATION::BossAnimation_Hokan(BOSS_ANIMATION_STATE state)
{
	for (int y = 0; y < BOSS_PARTS::BOSS_PARTS_MAX; y++)
	{
		for (int i = 0; i < FLAME_MAX; i++)
		{
			if (i != 0 && m_BossAnim[state].m_Anim[y].fps[i].SetPos)
			{
				int a = 0;

				for (int i = 0; i < FLAME_MAX; i++)
				{
					if (!m_BossAnim[state].m_Anim[y].fps[i].Fill)
					{
						a = i;
						break;
					}
				}

				XMFLOAT3 pos{ 0.0f,0.0f,0.0f };
				XMFLOAT3 rot{ 0.0f,0.0f,0.0f };

				int waru = (i - a);

				if (a == 0)
				{
					pos = {
						(m_BossAnim[state].m_Anim[y].fps[i].Position.x - m_BossAnim[state].m_Anim[y].fps[a].Position.x) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Position.y - m_BossAnim[state].m_Anim[y].fps[a].Position.y) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Position.z - m_BossAnim[state].m_Anim[y].fps[a].Position.z) / waru };



				}
				else
				{
					pos = {
						(m_BossAnim[state].m_Anim[y].fps[i].Position.x - m_BossAnim[state].m_Anim[y].fps[a - 1].Position.x) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Position.y - m_BossAnim[state].m_Anim[y].fps[a - 1].Position.y) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Position.z - m_BossAnim[state].m_Anim[y].fps[a - 1].Position.z) / waru };

				}

				if (a == 0)
				{
					rot = {
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.x - m_BossAnim[state].m_Anim[y].fps[a].Rotation.x) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.y - m_BossAnim[state].m_Anim[y].fps[a].Rotation.y) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.z - m_BossAnim[state].m_Anim[y].fps[a].Rotation.z) / waru };



				}
				else
				{
					rot = {
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.x - m_BossAnim[state].m_Anim[y].fps[a - 1].Rotation.x) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.y - m_BossAnim[state].m_Anim[y].fps[a - 1].Rotation.y) / waru,
						(m_BossAnim[state].m_Anim[y].fps[i].Rotation.z - m_BossAnim[state].m_Anim[y].fps[a - 1].Rotation.z) / waru };

				}

				for (int l = a; l < i; l++)
				{

					if (!m_BossAnim[state].m_Anim[y].fps[l].Fill)
					{
						if (l == 0)
						{
							m_BossAnim[state].m_Anim[y].fps[l].Position.x = (m_BossAnim[state].m_Anim[y].fps[l].Position.x + pos.x);
							m_BossAnim[state].m_Anim[y].fps[l].Position.y = (m_BossAnim[state].m_Anim[y].fps[l].Position.y + pos.y);
							m_BossAnim[state].m_Anim[y].fps[l].Position.z = (m_BossAnim[state].m_Anim[y].fps[l].Position.z + pos.z);

							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Position.z = 0.0f;
							}

							m_BossAnim[state].m_Anim[y].fps[l].Rotation.x = (m_BossAnim[state].m_Anim[y].fps[l].Rotation.x + rot.x);
							m_BossAnim[state].m_Anim[y].fps[l].Rotation.y = (m_BossAnim[state].m_Anim[y].fps[l].Rotation.y + rot.y);
							m_BossAnim[state].m_Anim[y].fps[l].Rotation.z = (m_BossAnim[state].m_Anim[y].fps[l].Rotation.z + rot.z);

							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_BossAnim[state].m_Anim[y].fps[l].Fill = true;
						}
						else
						{

							m_BossAnim[state].m_Anim[y].fps[l].Position.x = (m_BossAnim[state].m_Anim[y].fps[l - 1].Position.x + pos.x);
							m_BossAnim[state].m_Anim[y].fps[l].Position.y = (m_BossAnim[state].m_Anim[y].fps[l - 1].Position.y + pos.y);
							m_BossAnim[state].m_Anim[y].fps[l].Position.z = (m_BossAnim[state].m_Anim[y].fps[l - 1].Position.z + pos.z);

							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Position.z = 0.0f;
							}

							m_BossAnim[state].m_Anim[y].fps[l].Rotation.x = (m_BossAnim[state].m_Anim[y].fps[l - 1].Rotation.x + rot.x);
							m_BossAnim[state].m_Anim[y].fps[l].Rotation.y = (m_BossAnim[state].m_Anim[y].fps[l - 1].Rotation.y + rot.y);
							m_BossAnim[state].m_Anim[y].fps[l].Rotation.z = (m_BossAnim[state].m_Anim[y].fps[l - 1].Rotation.z + rot.z);

							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_BossAnim[state].m_Anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_BossAnim[state].m_Anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_BossAnim[state].m_Anim[y].fps[l].Fill = true;
						}
					}
				}
			}
			else
			{
				continue;
			}
		}
	}
}

void BOSSANIMATION::BossAnimation_SetAnimWait()
{
	{//head
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.15f, 0.0f }, { -2.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f, 0.3f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f, 0.15f, 0.0f }, { 2.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//body 
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f,-0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,-0.2f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,-2.1f, 0.0f }, { 0.0f, 0.0f, 38.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-2.2f, 0.0f }, { 0.0f, 0.0f, 42.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-2.1f, 0.0f }, { 0.0f, 0.0f, 38.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
	}
	{//left_arm
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,-2.1f, 0.0f }, { 0.0f, 0.0f,-38.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-2.2f, 0.0f }, { 0.0f, 0.0f,-42.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-2.1f, 0.0f }, { 0.0f, 0.0f,-38.0f });
		m_BossAnim[BOSS_STATE_IDLE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_IDLE);
}

void BOSSANIMATION::BossAnimation_SetAnimMove()
{

	{//head
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { -5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.3f, 0.0f }, { 5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f, 0.0f, 0.0f }, { -5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f, 0.3f, 0.0f }, { 5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { -5.0f, 0.0f, 0.0f });
	}
	{//body
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f,-3.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f,-0.3f, 0.0f }, { 0.0f, 0.0f, 3.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f,-3.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f,-0.3f, 0.0f }, { 0.0f, 0.0f, 3.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f,-3.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-1.5f,-0.3f }, { 0.0f, 0.0f, 30.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,-2.5f, 0.3f }, { 0.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-1.5f,-0.3f }, { 0.0f, 0.0f, 30.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-2.5f, 0.3f }, { 0.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-1.5f,-0.3f }, { 0.0f, 0.0f, 30.0f });
	}
	{//left_arm
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.5f, 0.3f }, { 0.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,-1.5f,-0.3f }, { 0.0f, 0.0f,-30.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-2.5f, 0.3f }, { 0.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-1.5f,-0.3f }, { 0.0f, 0.0f,-30.0f });
		m_BossAnim[BOSS_STATE_MOVE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.5f, 0.3f }, { 0.0f, 0.0f,-50.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_MOVE);
}

void BOSSANIMATION::BossAnimation_SetAnimPhase01()
{

	{//head
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_HEAD].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//body
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_BODY].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f,0.2f }, { 0.0f,-20.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f,-0.2f }, { 0.0f,20.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//left_arm
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f,-0.2f }, { 0.0f,-20.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f,0.2f }, { 0.0f,20.0f,0.0f });
		m_BossAnim[BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01].m_Anim[BOSS_PARTS::BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_PHASE01);
}

void BOSSANIMATION::BossAnimation_SetAnimPhase02_01()
{
	{//head
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.5f, 0.0f }, { 10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(24, { 0.0f, 0.8f, 0.0f }, { 15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f,-0.3f, 0.0f }, { -15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(35, { 0.0f,-0.1f, 0.0f }, { -5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//body
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f, 0.3f, 0.0f }, { 8.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(24, { 0.0f, 0.5f, 0.0f }, { 12.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,-0.4f, 0.0f }, { -10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(35, { 0.0f,-0.2f, 0.0f }, { -3.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm 
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f, 3.0f, 0.5f }, { 60.0f, 0.0f, 20.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(24, { 0.0f, 4.0f, 0.8f }, { 80.0f, 0.0f, 10.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-3.0f,-1.0f }, { -40.0f, 0.0f, 45.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(35, { 0.0f,-2.5f,-0.5f }, { -20.0f, 0.0f, 40.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-2.0f,-0.2f }, { -5.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
	}
	{//left_arm
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,-1.5f, 0.0f }, { 0.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(24, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f,-70.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-1.5f, 0.0f }, { 0.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_PHASE02_01].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_PHASE02_01);
}

void BOSSANIMATION::BossAnimation_SetAnimPhase02_02()
{
	{//head
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.5f, 0.0f }, { 10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(24, { 0.0f, 0.8f, 0.0f }, { 15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f,-0.3f, 0.0f }, { 15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(35, { 0.0f,-0.1f, 0.0f }, { 5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//body
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f, 0.3f, 0.0f }, { 8.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(24, { 0.0f, 0.5f, 0.0f }, { 12.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,-0.4f, 0.0f }, { -10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(35, { 0.0f,-0.2f, 0.0f }, { -3.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,-1.5f, 0.0f }, { 0.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f, 70.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-1.5f, 0.0f }, { 0.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
	}
	{//left_arm 
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f, 3.0f, 0.5f }, { 60.0f, 0.0f,-20.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(24, { 0.0f, 4.0f, 0.8f }, { 80.0f, 0.0f,-10.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-3.0f,-1.0f }, { -40.0f, 0.0f,-45.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(35, { 0.0f,-2.5f,-0.5f }, { -20.0f, 0.0f,-40.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-2.0f,-0.2f }, { -5.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_PHASE02_02].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_PHASE02_02);
}

void BOSSANIMATION::BossAnimation_SetAnimPhase03()
{
	{//head
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(7,  { 0.0f, 0.3f, 0.0f }, { 5.0f,-3.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.6f, 0.0f }, { 8.0f, 3.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(20, { 0.0f, 1.0f, 0.0f }, { 12.0f,-3.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(29, { 0.0f,-0.5f, 0.0f }, { -20.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(35, { 0.0f,-0.3f, 0.0f }, { -10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f,-0.1f, 0.0f }, { -3.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//body 
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f, 0.5f, 0.0f }, { 10.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(20, { 0.0f, 0.8f, 0.0f }, { 15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,-0.6f, 0.0f }, { -15.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(35, { 0.0f,-0.3f, 0.0f }, { -5.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f, 0.0f }, { -2.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(10, { 0.0f, 1.0f, 0.3f }, { 40.0f, 0.0f, 20.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(20, { 0.0f, 5.0f, 1.0f }, { 90.0f, 0.0f, 5.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-4.0f,-1.5f }, { -50.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(35, { 0.0f,-3.0f,-0.8f }, { -25.0f, 0.0f, 45.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-2.5f,-0.3f }, { -10.0f, 0.0f, 38.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
	}
	{//left_arm 
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(10, { 0.0f, 1.0f, 0.3f }, { 40.0f, 0.0f,-20.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(20, { 0.0f, 5.0f, 1.0f }, { 90.0f, 0.0f,-5.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-4.0f,-1.5f }, { -50.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(35, { 0.0f,-3.0f,-0.8f }, { -25.0f, 0.0f,-45.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-2.5f,-0.3f }, { -10.0f, 0.0f,-38.0f });
		m_BossAnim[BOSS_STATE_PHASE03].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_PHASE03);
}

void BOSSANIMATION::BossAnimation_SetAnimDamage()
{

	{//head
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(5,  { 0.0f, 0.5f, 0.0f }, { 15.0f,-10.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(14, { 0.0f, 0.3f, 0.0f }, { 10.0f, 12.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(24, { 0.0f, 0.1f, 0.0f }, { 3.0f,-8.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(35, { 0.0f, 0.05f, 0.0f }, { 1.0f, 4.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f, 0.0f, 0.0f }, { 0.0f,-2.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//body 
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(5,  { 0.0f, 0.0f, 0.0f }, { 12.0f, 0.0f,-5.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f, 0.0f, 0.0f }, { 8.0f, 0.0f, 5.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f, 0.0f, 0.0f }, { 3.0f, 0.0f,-3.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(5,  { 0.0f, 0.0f, 0.3f }, { 20.0f, 0.0f, 80.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,-0.5f, 0.1f }, { 10.0f, 0.0f, 60.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-1.5f, 0.0f }, { 3.0f, 0.0f, 45.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-1.8f, 0.0f }, { 1.0f, 0.0f, 38.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
	}
	{//left_arm
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(5,  { 0.0f, 0.0f,-0.3f }, { 20.0f, 0.0f,-80.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,-0.5f, 0.1f }, { 10.0f, 0.0f,-60.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-1.5f, 0.0f }, { 3.0f, 0.0f,-45.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-1.8f, 0.0f }, { 1.0f, 0.0f,-38.0f });
		m_BossAnim[BOSS_STATE_DAMAGE].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_DAMAGE);
}

void BOSSANIMATION::BossAnimation_SetAnimDeath()
{

	{//head 
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(10, { 0.0f, 2.0f, 0.0f }, { 0.0f,-15.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(20, { 0.0f, 3.5f, 0.0f }, { 0.0f,-40.0f,-10.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(35, { 0.0f, 4.0f, 0.0f }, { 0.0f,-80.0f,-15.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(44, { 0.0f, 2.0f, 0.0f }, { -10.0f,-120.0f,-20.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(52, { 0.0f, 0.0f, 0.0f }, { -15.0f,-160.0f,-25.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_HEAD].SetInisFlame(59, { 0.0f,-1.0f, 0.0f }, { -20.0f,-200.0f,-30.0f });
	}
	{//body
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_BODY].SetInisFlame(14, { 0.0f,-0.3f, 0.0f }, { -5.0f, 0.0f,-3.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_BODY].SetInisFlame(29, { 0.0f,-0.8f, 0.0f }, { -12.0f, 0.0f,-5.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_BODY].SetInisFlame(44, { 0.0f,-1.5f, 0.0f }, { -20.0f, 0.0f,-8.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_BODY].SetInisFlame(59, { 0.0f,-2.5f, 0.0f }, { -30.0f, 0.0f,-10.0f });
	}
	{//right_arm 
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f, 35.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,-3.0f,-0.1f }, { -5.0f, 0.0f, 50.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,-4.5f,-0.2f }, { -10.0f, 0.0f, 70.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,-5.5f,-0.3f }, { -15.0f, 0.0f, 85.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,-6.0f,-0.4f }, { -20.0f, 0.0f, 90.0f });
	}
	{//left_arm 
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,-2.0f, 0.0f }, { 0.0f, 0.0f,-35.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,-3.0f, 0.1f }, { -5.0f, 0.0f,-50.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,-4.5f, 0.2f }, { -10.0f, 0.0f,-70.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,-5.5f, 0.3f }, { -15.0f, 0.0f,-85.0f });
		m_BossAnim[BOSS_STATE_DEATH].m_Anim[BOSS_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,-6.0f, 0.4f }, { -20.0f, 0.0f,-90.0f });
	}

	BossAnimation_Hokan(BOSS_STATE_DEATH);
}

void BOSSANIMATION::BossAnimation_UpdateHead(XMFLOAT3 playerPos)
{

	if (m_State == BOSS_STATE_IDLE)
	{
		// 敵の向きをプレイヤーに向ける（Y軸に加えX軸回転＝ピッチを追加）
		XMFLOAT3 direction;
		XMFLOAT3 m_Position = m_Parts[BOSS_PARTS::BOSS_PARTS_HEAD].GetPartsPosition();
		XMFLOAT3 m_Rotation = m_Parts[BOSS_PARTS::BOSS_PARTS_HEAD].GetPartsRotation();

		direction.x = playerPos.x - m_Position.x;
		direction.y = playerPos.y - m_Position.y; // プレイヤーの高さ差を使う
		direction.z = playerPos.z - m_Position.z;

		// 水平成分の長さ（XZ 平面）
		float horiz = sqrtf((direction.x * direction.x) + (direction.z * direction.z));

		// Yaw（Y軸回転）: 既存ロジックを保持
		if (horiz != 0.0f)
		{
			// 正規化して atan2 をとる (X,Z の順に atan2f を使っている既存の向きと一致)
			float dirx = direction.x / horiz;
			float dirz = direction.z / horiz;
			// atan2f の引数は (x, z) なので既存の挙動をそのまま利用
			float yaw = atan2f(dirx, dirz);

			// [-π, π] に正規化
			if (yaw > XM_PI) yaw -= XM_2PI;
			if (yaw < -XM_PI) yaw += XM_2PI;

			m_Rotation.y = yaw;

			// Pitch（X軸回転）: プレイヤーの上下位置に応じて上下を向く
			// Pitch
			float pitch = atan2f(direction.y, horiz);
			m_Rotation.x = -pitch;   // 上下逆なら外す

		}

		// 更新を反映
		m_Parts[BOSS_PARTS::BOSS_PARTS_HEAD].SetPartsRotation(m_Rotation);
	}
}

XMFLOAT3 BOSSANIMATION::BossAnimation_AnimationPositiion(BOSS_ANIMATION_STATE state, BOSS_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame)
{
	XMFLOAT3 position = pos;
	XMFLOAT3 InisPos = parts->GetInisPosition();
	XMFLOAT3 nowPos = parts->GetNowPosition();
	XMFLOAT3 lastPos = m_BossAnim[state].m_Anim[part].fps[frame].Position;
	XMFLOAT3 lastRot = m_BossAnim[state].m_Anim[part].fps[frame].Rotation;
	bool	 loop = parts->GetAnimLoop();
	/*parts->GetAnimLastPosition(parts->GetNowPos());*/
	//x蛟､
	position.x -= InisPos.x * cosf(rot.y);
	position.z += InisPos.x * sinf(rot.y);

	//z蛟､
	position.x -= InisPos.z * cosf(rot.y);
	position.z += InisPos.z * sinf(rot.y);

	//・吝､
	position.y += InisPos.y;

	//x蛟､
	position.x -= lastPos.x * cosf(rot.y);
	position.z += lastPos.x * sinf(rot.y);

	//z蛟､
	position.z += lastPos.z * cosf(rot.y);
	position.x += lastPos.z * sinf(rot.y);

	//・吝､
	position.y += lastPos.y;

	return position;
}

XMFLOAT3 BOSSANIMATION::BossAnimation_AnimationRotation(BOSS_ANIMATION_STATE state, BOSS_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame)
{
	XMFLOAT3 Rot = XMFLOAT3{ 0,0,0 };
	XMFLOAT3 lastRot = m_BossAnim[state].m_Anim[part].fps[frame].Rotation;

	//x値
	Rot.x = lastRot.x;

	//z値
	Rot.z = lastRot.z;

	//ｙ値
	Rot.y = lastRot.y;


	return Rot;
}

void BOSSANIMATION::SetBossAnimState(BOSS_ANIMATION_STATE state)
{

	//60framesで一度ループしたら次のステート確認をする。アニメーション待機
	if (!m_AnimLoop)
	{
		m_FrameCnt = 0.0f;//フレームカウンターリセット
		m_AnimLoop = true;//1ループ開始
		m_State = state;//ステート変更
		
	}
}

