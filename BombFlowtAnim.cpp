
#include "BombFlowtAnim.h"
#include	"Camera.h"
#include	"shader.h"


void BOMBFLOWTANIMATION::BombFlowtAnim_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{


	for (int i = 0; i < BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX; i++)
	{
		m_Parts[i].PartsInitialize(pDevice, pContext);
		m_Parts[i].SetPartsScaling(XMFLOAT3(1.0f, 1.0f, 1.0f));
	}

	for (int i = 0; i < BOMBFLOWT_STATE_MAX; i++)
	{
		for (int y = 0; y < BOMBFLOWT_PARTS_MAX; y++)
		{
			m_BossAnim[i].m_Anim[y].AnimInitialize();
		}
	}

	m_FrameCnt = 0.0f;
	m_State = BOMBFLOWT_STATE_NONE;
	m_AnimLoop = false;

	BombFlowtAnim_InisAnim();

}

void BOMBFLOWTANIMATION::BombFlowtAnim_Finalize()
{
	for (int i = 0; i < BOMBFLOWT_PARTS_MAX; i++)
	{
		m_Parts[i].PartsFinalize();
	}
	
}

void BOMBFLOWTANIMATION::BombFlowtAnim_Update(XMFLOAT3 bossPos, XMFLOAT3 bossRot)
{
	if (m_State != BOMBFLOWT_STATE_NONE)
	{
		float l = m_FrameCnt;

		for (int j = 0; j < BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX; j++)
		{
			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 plasRot = XMFLOAT3(0.0f, 0.0f, 0.0f);

			XMFLOAT3 pos = bossPos;
			XMFLOAT3 rot = bossRot;

			switch (j)
			{
			case BOMBFLOWT_PARTS_BODY:
				position = BombFlowtAnim_Positiion(m_State, BOMBFLOWT_PARTS_BODY, &m_Parts[j], pos, rot, (int)l);
				plasRot = BombFlowtAnim_Rotation(m_State, BOMBFLOWT_PARTS_BODY, &m_Parts[j], pos, rot, (int)l);
				break;
			case BOMBFLOWT_PARTS_ARM_RIGHT:
				position = BombFlowtAnim_Positiion(m_State, BOMBFLOWT_PARTS_ARM_RIGHT, &m_Parts[j], pos, rot, (int)l);
				plasRot = BombFlowtAnim_Rotation(m_State, BOMBFLOWT_PARTS_ARM_RIGHT, &m_Parts[j], pos, rot, (int)l);
				break;
			case BOMBFLOWT_PARTS_ARM_LEFT:
				position = BombFlowtAnim_Positiion(m_State, BOMBFLOWT_PARTS_ARM_LEFT, &m_Parts[j], pos, rot, (int)l);
				plasRot = BombFlowtAnim_Rotation(m_State, BOMBFLOWT_PARTS_ARM_LEFT, &m_Parts[j], pos, rot, (int)l);
				break;
			default:
				break;
			}


			rot.x += XMConvertToRadians(plasRot.x);
			rot.y += XMConvertToRadians(plasRot.y);
			rot.z += XMConvertToRadians(plasRot.z);


			switch (j)
			{
			case BOMBFLOWT_PARTS_BODY:
				//pos = Player_LeftLeg2(&m_Model[i], rot);
				m_Parts[j].PartsSet(position, rot);
				break;
			case BOMBFLOWT_PARTS_ARM_RIGHT:
				//pos = Player_LeftLeg2(&m_Model[i], rot);
				m_Parts[j].PartsSet(position, rot);
				break;
			case BOMBFLOWT_PARTS_ARM_LEFT:
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

	m_FrameCnt += BOMBFLOWT_ANIM_SPEED; //ボスのアニメーションだけスピードを変えるため
	if (m_FrameCnt >= 60.0f)
	{
		//一旦ループ前提
		m_FrameCnt = 0.0f;
		m_AnimLoop = false;
	}
}

void BOMBFLOWTANIMATION::BombFlowtAnim_Draw(MODEL* model,MODEL* model2,MODEL* model3)
{
	for (int i = 0; i < BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX; i++)
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
			m_Rotation.x + XMConvertToRadians(-45.0f),
			m_Rotation.y,
			m_Rotation.z);
		XMMATRIX	translation = XMMatrixTranslation(
			m_Position.x,
			m_Position.y + 0.5f,
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
		case BOMBFLOWT_PARTS_BODY:
			ModelDraw(model);
			break;
		case BOMBFLOWT_PARTS_ARM_RIGHT:
			ModelDraw(model2);
			break;
		case BOMBFLOWT_PARTS_ARM_LEFT:
			ModelDraw(model3);
			break;
		default:
			break;
		}
		

		/*if (m_State == BOMBFLOWT_STATE_MOVE) {
			switch (i)
			{
			case BOMBFLOWT_PARTS_BODY:
				
				break;
			case BOMBFLOWT_PARTS_ARM_RIGHT:
				ModelDraw(model);
				break;
			case BOMBFLOWT_PARTS_ARM_LEFT:
				ModelDraw(m_ModelDataBomb[i]);
				break;
			default:
				break;
			}
		}
		else {
			switch (i)
			{
			case BOMBFLOWT_PARTS_BODY:
				ModelDraw(m_ModelData[i]);
				break;
			case BOMBFLOWT_PARTS_ARM_RIGHT:
				ModelDraw(m_ModelData[i]);
				break;
			case BOMBFLOWT_PARTS_ARM_LEFT:
				ModelDraw(m_ModelData[i]);
				break;
			default:
				break;
			}
		}*/
	}
}

void BOMBFLOWTANIMATION::BombFlowtAnim_Hokan(BOMBFLOWT_ANIMATION_STATE state)
{
	for (int y = 0; y <BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX; y++)
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

void BOMBFLOWTANIMATION::BombFlowtAnim_InisAnim()
{
	for (int i = 0; i <BOMBFLOWT_PARTS::BOMBFLOWT_PARTS_MAX; i++)
	{
		switch (i)
		{
		case BOMBFLOWT_PARTS_BODY:
			m_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			break;
		case BOMBFLOWT_PARTS_ARM_RIGHT:
			m_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 0.11f, 0.0f));
			break;
		case BOMBFLOWT_PARTS_ARM_LEFT:
			m_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 0.11f, 0.0f));
			break;
		default:
			break;
		}

	}

	BombFlowtAnim_SetAnimWait();
	BombFlowtAnim_SetAnimMove();
}

void BOMBFLOWTANIMATION::BombFlowtAnim_SetAnimWait()
{
	{//body 
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(14, { 0.0f,0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(44, { 0.0f,0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 45.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 45.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//left_arm
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, -45.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, -45.0f });
		m_BossAnim[BOMBFLOWT_STATE_IDLE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}

	BombFlowtAnim_Hokan(BOMBFLOWT_STATE_IDLE);
}

void BOMBFLOWTANIMATION::BombFlowtAnim_SetAnimMove()
{
	{//body 
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(14, { 0.0f,0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(44, { 0.0f,0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 45.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 45.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	{//left_arm
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f,  0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f,  -45.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f,  0.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f,  -45.0f });
		m_BossAnim[BOMBFLOWT_STATE_MOVE].m_Anim[BOMBFLOWT_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.0f, 0.0f }, { 0.0f, 0.0f,  0.0f });
	}

	BombFlowtAnim_Hokan(BOMBFLOWT_STATE_MOVE);
}

void BOMBFLOWTANIMATION::BombFlowtAnim_UpdateHead(XMFLOAT3 playerPos)
{
}

XMFLOAT3 BOMBFLOWTANIMATION::BombFlowtAnim_Positiion(BOMBFLOWT_ANIMATION_STATE state, BOMBFLOWT_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame)
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

XMFLOAT3 BOMBFLOWTANIMATION::BombFlowtAnim_Rotation(BOMBFLOWT_ANIMATION_STATE state, BOMBFLOWT_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame)
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

void BOMBFLOWTANIMATION::SetBombFlowtAnimState(BOMBFLOWT_ANIMATION_STATE state)
{

	//60framesで一度ループしたら次のステート確認をする。アニメーション待機
	if (!m_AnimLoop)
	{
		m_FrameCnt = 0.0f;//フレームカウンターリセット
		m_AnimLoop = true;//1ループ開始
	}

	m_State = state;//ステート変更
}
