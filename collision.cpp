
// collision.cpp

#include "collision.h"



float	COLLISION::BallFieldCollision(BALL* pBall, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 BallPos = pBall->GetBallPosition();
	XMFLOAT3 BallVel = pBall->GetBallVelocity();
	MAP*	Map = pField->GetFieldMap();	// マップ
	int			i = 0;

	// 全てのブロックをチェック
	while (Map[i].MapData_GetNo() != FIELD_MAX)
	{
		float BoxTop;	// BOXの+Y面の座標

		XMFLOAT3 mapPos = Map[i].MapData_GetPosition();

		switch (Map[i].MapData_GetNo())
		{

		default:
			BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
			break;
		}

		// 壁としての判定処理
		if (mapPos.y - BOX_RADIUS < BallPos.y &&
			BallPos.y < BoxTop - 0.1f)
		{
			if (mapPos.z - BOX_RADIUS < BallPos.z &&
				BallPos.z < mapPos.z + BOX_RADIUS)
			{
				if (mapPos.x - BOX_RADIUS < BallPos.x + BALL_RADIUS &&
					BallPos.x < mapPos.x - BOX_RADIUS)
				{//BOXの-X面にぶつかったので座標の補正
					BallPos.x += (mapPos.x - BOX_RADIUS) - (BallPos.x + BALL_RADIUS);
					BallVel.x *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_3;
				}
				else if (mapPos.x + BOX_RADIUS > BallPos.x - BALL_RADIUS &&
					BallPos.x > mapPos.x + BOX_RADIUS)
				{//BOXの+X面にぶつかった
					BallPos.x += (mapPos.x + BOX_RADIUS) - (BallPos.x - BALL_RADIUS);
					BallVel.x *= -COE;
					hit = COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if (mapPos.x - BOX_RADIUS < BallPos.x &&
				BallPos.x < mapPos.x + BOX_RADIUS)
			{
				if (mapPos.z - BOX_RADIUS < BallPos.z + BALL_RADIUS &&
					BallPos.z < mapPos.z - BOX_RADIUS)
				{//BOXの-Z面にぶつかったので座標の補正
					BallPos.z += (mapPos.z - BOX_RADIUS) - (BallPos.z + BALL_RADIUS);
					BallVel.z *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_0;
				}
				else if (mapPos.z + BOX_RADIUS > BallPos.z - BALL_RADIUS &&
					BallPos.z > mapPos.z + BOX_RADIUS)
				{//BOXの+Z面にぶつかった
					BallPos.z += (mapPos.z + BOX_RADIUS) - (BallPos.z - BALL_RADIUS);
					BallVel.z *= -COE;
					hit = COLLISION_HIT::HIT_WALL_2;
				}
			}
		}
		//地面として判定処理
		else
		{
			if (mapPos.z - BOX_RADIUS < BallPos.z &&
				BallPos.z < mapPos.z + BOX_RADIUS)
			{
				if (mapPos.x - BOX_RADIUS < BallPos.x &&
					BallPos.x < mapPos.x + BOX_RADIUS)
				{
					if (mapPos.y - BOX_RADIUS < BallPos.y + BALL_RADIUS &&
						BallPos.y < mapPos.y - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						BallPos.y += (mapPos.y - BOX_RADIUS) - (BallPos.y + BALL_RADIUS);
						BallVel.y *= -COE; //移動ベクトルの反転
						//hit = 
					}
					else if (BoxTop > BallPos.y - BALL_RADIUS &&
						BallPos.y > BoxTop)
					{//BOXの+X面にぶつかった
						BallPos.y += (BoxTop)-(BallPos.y - BALL_RADIUS);
						BallVel.y = BallVel.y * (-COE * 1.0f);
						hit = COLLISION_HIT::HIT_GROUND;
					}
				}
			}
		}


		pBall->SetBallPosition(BallPos);
		pBall->SetBallVelocity(BallVel);

		i++;
	}


	return hit;  // ぶつかったかどうかを示す
}