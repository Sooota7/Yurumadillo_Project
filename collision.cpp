
// collision.cpp

#include "collision.h"



float	BallFieldCollision()
{
	float		hit = 0.0f;				// ヒットした方向
	BALL*		Ball = GetBall();		// ボールの情報
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
		if (Map[i].pos.y - BOX_RADIUS < Ball->Position.y &&
			Ball->Position.y < BoxTop - 0.1f)
		{// -Y面< ボールのY座標　ボールのY座標< +Y面　
			if (Map[i].pos.z - BOX_RADIUS < Ball->Position.z &&
				Ball->Position.z < Map[i].pos.z + BOX_RADIUS)
			{// -Z面< ボールのZ座標　ボールのZ座標< +Z面
				if (Map[i].pos.x - BOX_RADIUS < Ball->Position.x + BALL_RADIUS &&
					Ball->Position.x < Map[i].pos.x - BOX_RADIUS)
				{// BOXの-X面にぶつかったので座標の補正

					Ball->Position.x += (Map[i].pos.x - BOX_RADIUS) -
						(Ball->Position.x + BALL_RADIUS);

					Ball->Velocity.x *= -COE; // 移動ベクトルの反転

					hit = (float)COLLISION_HIT::HIT_WALL_3;
				}
				else if (Map[i].pos.x + BOX_RADIUS > Ball->Position.x - BALL_RADIUS &&
					Ball->Position.x > Map[i].pos.x + BOX_RADIUS)
				{// BOXの+X面にぶつかったので座標の補正

					Ball->Position.x += (Map[i].pos.x + BOX_RADIUS) -
						(Ball->Position.x = BALL_RADIUS);

					Ball->Velocity.x *= -COE;

					hit = (float)COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if (Map[i].pos.x - BOX_RADIUS < Ball->Position.x &&
				Ball->Position.x < Map[i].pos.x + BOX_RADIUS)
			{
				if (Map[i].pos.z - BOX_RADIUS < Ball->Position.z + BALL_RADIUS &&
					Ball->Position.z < Map[i].pos.z - BOX_RADIUS)
				{// BOXの-Z面にぶつかったので座標の補正

					Ball->Position.z += (Map[i].pos.z - BOX_RADIUS) -
						(Ball->Position.z + BALL_RADIUS);

					Ball->Velocity.z *= -COE; // 移動ベクトルの反転

					hit = (float)COLLISION_HIT::HIT_WALL_0;
				}
				else if (Map[i].pos.z + BOX_RADIUS > Ball->Position.z - BALL_RADIUS &&
					Ball->Position.z > Map[i].pos.z + BOX_RADIUS)
				{// BOXの+Z面にぶつかったので座標の補正

					Ball->Position.z += (Map[i].pos.z + BOX_RADIUS) -
						(Ball->Position.z = BALL_RADIUS);

					Ball->Velocity.z *= -COE;

					hit = (float)COLLISION_HIT::HIT_WALL_2;
				}

			}

		}
		// 地面としての判定処理
		else
		{
			if (Map[i].pos.z - BOX_RADIUS < Ball->Position.z &&
				Ball->Position.z < Map[i].pos.z + BOX_RADIUS)
			{
				if (Map[i].pos.x - BOX_RADIUS < Ball->Position.x &&
					Ball->Position.x < Map[i].pos.x + BOX_RADIUS)
				{
					if (Map[i].pos.y - BOX_RADIUS < Ball->Position.y + BALL_RADIUS &&
						Ball->Position.y < Map[i].pos.y + BOX_RADIUS)
					{// 下から当たった
						Ball->Position.y += (Map[i].pos.y - BOX_RADIUS) -
							(Ball->Position.y + BOX_RADIUS);
						Ball->Velocity.y *= -COE;
						// hit = 
					}
					else if (BoxTop > Ball->Position.y - BALL_RADIUS &&
						Ball->Position.y > BoxTop)
					{// 地面に当たった
					 // 上から当たったので座標の補正
						Ball->Position.y += (BoxTop)-(Ball->Position.y - BALL_RADIUS);
						Ball->Velocity.y = Ball->Velocity.y * (-COE * 1.0f); // ベクトルの反転
						hit = COLLISION_HIT::HIT_GROUND;
					}

				}
			}
		}




		i++;
	}


	return hit;  // ぶつかったかどうかを示す
}