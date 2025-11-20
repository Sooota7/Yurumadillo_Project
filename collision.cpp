
// collision.cpp

#include "collision.h"



float	COLLISION::BallFieldCollision(BALL* pBall/*, MAPDATA* pField*/)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 BallPos = pBall->GetBallPosition();
	XMFLOAT3 BallVel = pBall->GetBallVelocity();
	MAPDATA*	Map = GetFieldMap();	// マップ
	int			i = 0;

	// 全てのブロックをチェック
	while (Map[i].no != FIELD_MAX)
	{
		float BoxTop;	// BOXの+Y面の座標

		switch (Map[i].no)
		{

		default:
			BoxTop = Map[i].pos.y + BOX_RADIUS;	// 普通のBOX
			break;
		}

		// 壁としての判定処理
		if (Map[i].pos.y - BOX_RADIUS < BallPos.y &&
			BallPos.y < BoxTop - 0.1f)
		{// -Y面< ボールのY座標　ボールのY座標< +Y面　
			if (Map[i].pos.z - BOX_RADIUS < BallPos.z &&
				BallPos.z < Map[i].pos.z + BOX_RADIUS)
			{// -Z面< ボールのZ座標　ボールのZ座標< +Z面
				if (Map[i].pos.x - BOX_RADIUS < BallPos.x + BALL_RADIUS &&
					BallPos.x < Map[i].pos.x - BOX_RADIUS)
				{// BOXの-X面にぶつかったので座標の補正

					BallPos.x += (Map[i].pos.x - BOX_RADIUS) -
						(BallPos.x + BALL_RADIUS);

					BallVel.x *= -COE; // 移動ベクトルの反転

					hit = (float)COLLISION_HIT::HIT_WALL_3;
				}
				else if (Map[i].pos.x + BOX_RADIUS > BallPos.x - BALL_RADIUS &&
					BallPos.x > Map[i].pos.x + BOX_RADIUS)
				{// BOXの+X面にぶつかったので座標の補正

					BallPos.x += (Map[i].pos.x + BOX_RADIUS) -
						(BallPos.x = BALL_RADIUS);

					BallVel.x *= -COE;

					hit = (float)COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if (Map[i].pos.x - BOX_RADIUS < BallPos.x &&
				BallPos.x < Map[i].pos.x + BOX_RADIUS)
			{
				if (Map[i].pos.z - BOX_RADIUS < BallPos.z + BALL_RADIUS &&
					BallPos.z < Map[i].pos.z - BOX_RADIUS)
				{// BOXの-Z面にぶつかったので座標の補正

					BallPos.z += (Map[i].pos.z - BOX_RADIUS) -
						(BallPos.z + BALL_RADIUS);

					BallVel.z *= -COE; // 移動ベクトルの反転

					hit = (float)COLLISION_HIT::HIT_WALL_0;
				}
				else if (Map[i].pos.z + BOX_RADIUS > BallPos.z - BALL_RADIUS &&
					BallPos.z > Map[i].pos.z + BOX_RADIUS)
				{// BOXの+Z面にぶつかったので座標の補正

					BallPos.z += (Map[i].pos.z + BOX_RADIUS) -
						(BallPos.z = BALL_RADIUS);

					BallVel.z *= -COE;

					hit = (float)COLLISION_HIT::HIT_WALL_2;
				}

			}

		}
		// 地面としての判定処理
		else
		{
			if (Map[i].pos.z - BOX_RADIUS < BallPos.z &&
				BallPos.z < Map[i].pos.z + BOX_RADIUS)
			{
				if (Map[i].pos.x - BOX_RADIUS < BallPos.x &&
					BallPos.x < Map[i].pos.x + BOX_RADIUS)
				{
					if (Map[i].pos.y - BOX_RADIUS < BallPos.y + BALL_RADIUS &&
						BallPos.y < Map[i].pos.y + BOX_RADIUS)
					{// 下から当たった
						BallPos.y += (Map[i].pos.y - BOX_RADIUS) -
							(BallPos.y + BOX_RADIUS);
						BallVel.y *= -COE;
						// hit = 
					}
					else if (BoxTop > BallPos.y - BALL_RADIUS &&
						BallPos.y > BoxTop)
					{// 地面に当たった
					 // 上から当たったので座標の補正
						BallPos.y += (BoxTop)-(BallPos.y - BALL_RADIUS);
						BallVel.y = BallVel.y * (-COE * 1.0f); // ベクトルの反転
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