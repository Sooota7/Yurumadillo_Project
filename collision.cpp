
// collision.cpp

#include "collision.h"



float	COLLISION::PlayerFieldCollision(PLAYER* pPlayer, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	bool PlayerJump = pPlayer->GetPlayerJump();

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
		if (mapPos.y - BOX_RADIUS < PlayerPos.y &&
			PlayerPos.y < BoxTop - 0.1f)
		{
			if (mapPos.z - BOX_RADIUS < PlayerPos.z &&
				PlayerPos.z < mapPos.z + BOX_RADIUS)
			{
				if (mapPos.x - BOX_RADIUS < PlayerPos.x + PLAYER_RADIUS &&
					PlayerPos.x < mapPos.x - BOX_RADIUS)
				{//BOXの-X面にぶつかったので座標の補正
					PlayerPos.x += (mapPos.x - BOX_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
					PlayerVel.x *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_3;

					if (Map[i].MapData_GetNo() == FIELD_GOAL)
					{
						hit = COLLISION_HIT::HIT_WALL_CREAR;
					}
				}
				else if (mapPos.x + BOX_RADIUS > PlayerPos.x - PLAYER_RADIUS &&
					PlayerPos.x > mapPos.x + BOX_RADIUS)
				{//BOXの+X面にぶつかった
					PlayerPos.x += (mapPos.x + BOX_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
					PlayerVel.x *= -COE;
					hit = COLLISION_HIT::HIT_WALL_1;

					if (Map[i].MapData_GetNo() == FIELD_GOAL)
					{
						hit = COLLISION_HIT::HIT_WALL_CREAR;
					}
				}
			}
			else if (mapPos.x - BOX_RADIUS < PlayerPos.x &&
				PlayerPos.x < mapPos.x + BOX_RADIUS)
			{
				if (mapPos.z - BOX_RADIUS < PlayerPos.z + PLAYER_RADIUS &&
					PlayerPos.z < mapPos.z - BOX_RADIUS)
				{//BOXの-Z面にぶつかったので座標の補正
					PlayerPos.z += (mapPos.z - BOX_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
					PlayerVel.z *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_0;

					if (Map[i].MapData_GetNo() == FIELD_GOAL)
					{
						hit = COLLISION_HIT::HIT_WALL_CREAR;
					}
				}
				else if (mapPos.z + BOX_RADIUS > PlayerPos.z - PLAYER_RADIUS &&
					PlayerPos.z > mapPos.z + BOX_RADIUS)
				{//BOXの+Z面にぶつかった
					PlayerPos.z += (mapPos.z + BOX_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
					PlayerVel.z *= -COE;
					hit = COLLISION_HIT::HIT_WALL_2;

					if (Map[i].MapData_GetNo() == FIELD_GOAL)
					{
						hit = COLLISION_HIT::HIT_WALL_CREAR;
					}
				}
			}
		}
		//地面として判定処理
		else
		{
			if (mapPos.z - BOX_RADIUS < PlayerPos.z &&
				PlayerPos.z < mapPos.z + BOX_RADIUS)
			{
				if (mapPos.x - BOX_RADIUS < PlayerPos.x &&
					PlayerPos.x < mapPos.x + BOX_RADIUS)
				{
					if (mapPos.y - BOX_RADIUS < PlayerPos.y + PLAYER_RADIUS &&
						PlayerPos.y < mapPos.y - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.y += (mapPos.y - BOX_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
						PlayerVel.y *= -COE; //移動ベクトルの反転
						//hit = 
						if (Map[i].MapData_GetNo() == FIELD_GOAL)
						{
							hit = COLLISION_HIT::HIT_WALL_CREAR;
						}
					}
					else if (BoxTop > PlayerPos.y - PLAYER_RADIUS &&
						PlayerPos.y > BoxTop)
					{//BOXの+X面にぶつかった
						PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
						if (Map[i].MapData_GetNo() == FIELD_JUMP)
						{// ジャンプ台なら反発係数を大きくする
							PlayerVel.y *= -COE_JUMP;
						}
						else
						{
							PlayerVel.y = 0;
						}

						hit = COLLISION_HIT::HIT_GROUND;
						if (PlayerJump == false)
						{
							PlayerJump = true;
						}

						if (Map[i].MapData_GetNo() == FIELD_GOAL)
						{
							hit = COLLISION_HIT::HIT_WALL_CREAR;
						}
					}
				}
			}
		}

		pPlayer->SetPlayerJump(PlayerJump);
		pPlayer->SetPlayerPosition(PlayerPos);
		pPlayer->SetPlayerVelocity(PlayerVel);

		i++;
	}


	return hit;  // ぶつかったかどうかを示す
}

float	COLLISION::EnemyFieldCollision(ENEMYSPAWNER* pEnemy, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();


	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 EnemyPos = enemy[i].GetEnemyPosition();
			XMFLOAT3 EnemyVel = enemy[i].GetEnemyVelocity();
			MAP* Map = pField->GetFieldMap();	// マップ
			int			l = 0;

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < EnemyPos.y &&
					EnemyPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x + BALL_RADIUS &&
							EnemyPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							EnemyPos.x += (mapPos.x - BOX_RADIUS) - (EnemyPos.x + BALL_RADIUS);
							EnemyVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > EnemyPos.x - BALL_RADIUS &&
							EnemyPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							EnemyPos.x += (mapPos.x + BOX_RADIUS) - (EnemyPos.x - BALL_RADIUS);
							EnemyVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
						EnemyPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < EnemyPos.z + BALL_RADIUS &&
							EnemyPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							EnemyPos.z += (mapPos.z - BOX_RADIUS) - (EnemyPos.z + BALL_RADIUS);
							EnemyVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > EnemyPos.z - BALL_RADIUS &&
							EnemyPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							EnemyPos.z += (mapPos.z + BOX_RADIUS) - (EnemyPos.z - BALL_RADIUS);
							EnemyVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
							EnemyPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < EnemyPos.y + BALL_RADIUS &&
								EnemyPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								EnemyPos.y += (mapPos.y - BOX_RADIUS) - (EnemyPos.y + BALL_RADIUS);
								EnemyVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > EnemyPos.y - BALL_RADIUS &&
								EnemyPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								EnemyPos.y += (BoxTop)-(EnemyPos.y - BALL_RADIUS);
								EnemyVel.y = EnemyVel.y * (-COE * 1.0f);
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}


				enemy[i].SetEnemyPosition(EnemyPos);
				enemy[i].SetEnemyVelocity(EnemyVel);

				l++;
			}
		}
	}

	//=============================================================
	// EnemyButterflyの処理
	//=============================================================

	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemyB[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 EnemyPos = enemyB[i].GetEnemyPosition();
			XMFLOAT3 EnemyVel = enemyB[i].GetEnemyVelocity();
			MAP* Map = pField->GetFieldMap();	// マップ
			int			l = 0;

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < EnemyPos.y &&
					EnemyPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x + BALL_RADIUS &&
							EnemyPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							EnemyPos.x += (mapPos.x - BOX_RADIUS) - (EnemyPos.x + BALL_RADIUS);
							EnemyVel.x *= 0.0f; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > EnemyPos.x - BALL_RADIUS &&
							EnemyPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							EnemyPos.x += (mapPos.x + BOX_RADIUS) - (EnemyPos.x - BALL_RADIUS);
							EnemyVel.x *= 0.0f;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
						EnemyPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < EnemyPos.z + BALL_RADIUS &&
							EnemyPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							EnemyPos.z += (mapPos.z - BOX_RADIUS) - (EnemyPos.z + BALL_RADIUS);
							EnemyVel.z *= 0.0f; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > EnemyPos.z - BALL_RADIUS &&
							EnemyPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							EnemyPos.z += (mapPos.z + BOX_RADIUS) - (EnemyPos.z - BALL_RADIUS);
							EnemyVel.z *= 0.0f;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
							EnemyPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < EnemyPos.y + BALL_RADIUS &&
								EnemyPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								EnemyPos.y += (mapPos.y - BOX_RADIUS) - (EnemyPos.y + BALL_RADIUS);
								EnemyVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > EnemyPos.y - BALL_RADIUS &&
								EnemyPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								EnemyPos.y += (BoxTop)-(EnemyPos.y - BALL_RADIUS);
								EnemyVel.y = EnemyVel.y * (-COE * 1.0f);
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}


				enemyB[i].SetEnemyPosition(EnemyPos);
				enemyB[i].SetEnemyVelocity(EnemyVel);

				l++;
			}
		}
	}

	//=============================================================
	// EnemyGroundの処理
	//=============================================================

	ENEMY_GROUND* enemy_ground = pEnemy->EnemySpawner_GetEnemyGround();

	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemy_ground[i].GetEnemyGroundType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			if (enemy_ground[i].GetEnemyGroundState() == ENEMY_GROUND_STATE_MOVE)
			{
				enemy_ground[i].Enemy_Ground_SetDirection();//EnemyGroundの非接触時のVelocity
			}

			XMFLOAT3 EnemyPos = enemy_ground[i].GetEnemyPosition();
			XMFLOAT3 EnemyVel = enemy_ground[i].GetEnemyVelocity();
			MAP* Map = pField->GetFieldMap();	// マップ
			int			l = 0;

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < EnemyPos.y &&
					EnemyPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x + BALL_RADIUS &&
							EnemyPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							EnemyPos.x += (mapPos.x - BOX_RADIUS) - (EnemyPos.x + BALL_RADIUS) - 0.1f;
							EnemyVel.x = 0.0f; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > EnemyPos.x - BALL_RADIUS &&
							EnemyPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							EnemyPos.x += (mapPos.x + BOX_RADIUS) - (EnemyPos.x - BALL_RADIUS) + 0.1f;
							EnemyVel.x = 0.0f;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
						EnemyPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < EnemyPos.z + BALL_RADIUS &&
							EnemyPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							EnemyPos.z += (mapPos.z - BOX_RADIUS) - (EnemyPos.z + BALL_RADIUS) - 0.1f;
							EnemyVel.z = 0.0f; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > EnemyPos.z - BALL_RADIUS &&
							EnemyPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							EnemyPos.z += (mapPos.z + BOX_RADIUS) - (EnemyPos.z - BALL_RADIUS) + 0.1f;
							EnemyVel.z = 0.0f;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < EnemyPos.z &&
						EnemyPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < EnemyPos.x &&
							EnemyPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < EnemyPos.y + BALL_RADIUS &&
								EnemyPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								EnemyPos.y += (mapPos.y - BOX_RADIUS) - (EnemyPos.y + BALL_RADIUS) + 0.01f;
								EnemyVel.y = 0.0f; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > EnemyPos.y - BALL_RADIUS &&
								EnemyPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								EnemyPos.y += (BoxTop)-(EnemyPos.y - BALL_RADIUS) + 0.01f;
								EnemyVel.y = 0.0f;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}



				l++;
			}

			enemy_ground[i].SetEnemyPosition(EnemyPos);
			enemy_ground[i].SetEnemyVelocity(EnemyVel);

		}
	}

	return hit;
}


float	COLLISION::PlayerEnemyCollision(PLAYER* pPlayer, ENEMYSPAWNER* pEnemy)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();
	ENEMY_GROUND* enemyC = pEnemy->EnemySpawner_GetEnemyGround();

	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 EnemyPos = enemy[i].GetEnemyPosition();
			XMFLOAT3 EnemyVel = enemy[i].GetEnemyVelocity();
			float BoxTop;	// BOXの+Y面の座標

			BoxTop = EnemyPos.y + BOX_RADIUS;

			// 壁としての判定処理
			if (EnemyPos.y - BOX_RADIUS < PlayerPos.y &&
				PlayerPos.y < BoxTop - 0.1f)
			{
				if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
					PlayerPos.z < EnemyPos.z + BOX_RADIUS)
				{
					if (EnemyPos.x - BOX_RADIUS < PlayerPos.x + BALL_RADIUS &&
						PlayerPos.x < EnemyPos.x - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.x += (EnemyPos.x - BOX_RADIUS) - (PlayerPos.x + BALL_RADIUS);
						PlayerVel.x *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_3;
					}
					else if (EnemyPos.x + BOX_RADIUS > PlayerPos.x - BALL_RADIUS &&
						PlayerPos.x > EnemyPos.x + BOX_RADIUS)
					{//BOXの+X面にぶつかった
						PlayerPos.x += (EnemyPos.x + BOX_RADIUS) - (PlayerPos.x - BALL_RADIUS);
						PlayerVel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_1;
					}
				}
				else if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
					PlayerPos.x < EnemyPos.x + BOX_RADIUS)
				{
					if (EnemyPos.z - BOX_RADIUS < PlayerPos.z + BALL_RADIUS &&
						PlayerPos.z < EnemyPos.z - BOX_RADIUS)
					{//BOXの-Z面にぶつかったので座標の補正
						PlayerPos.z += (EnemyPos.z - BOX_RADIUS) - (PlayerPos.z + BALL_RADIUS);
						PlayerVel.z *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_0;
					}
					else if (EnemyPos.z + BOX_RADIUS > PlayerPos.z - BALL_RADIUS &&
						PlayerPos.z > EnemyPos.z + BOX_RADIUS)
					{//BOXの+Z面にぶつかった
						PlayerPos.z += (EnemyPos.z + BOX_RADIUS) - (PlayerPos.z - BALL_RADIUS);
						PlayerVel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_2;
					}
				}
			}
			//地面として判定処理
			else
			{
				if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
					PlayerPos.z < EnemyPos.z + BOX_RADIUS)
				{
					if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
						PlayerPos.x < EnemyPos.x + BOX_RADIUS)
					{
						if (EnemyPos.y - BOX_RADIUS < PlayerPos.y + BALL_RADIUS &&
							PlayerPos.y < EnemyPos.y - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.y += (EnemyPos.y - BOX_RADIUS) - (PlayerPos.y + BALL_RADIUS);
							EnemyVel.y *= -COE; //移動ベクトルの反転
							//hit = 
						}
						else if (BoxTop > PlayerPos.y - BALL_RADIUS &&
							PlayerPos.y > BoxTop)
						{//BOXの+X面にぶつかった
							PlayerPos.y += (BoxTop)-(PlayerPos.y - BALL_RADIUS);
							EnemyVel.y = EnemyVel.y * (-COE * 1.0f);
							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}


			pPlayer->SetPlayerPosition(PlayerPos);
			pPlayer->SetPlayerVelocity(PlayerVel);


			//return hit;
		}
	}

	// 浮いてる敵
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemyB[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 EnemyPos = enemyB[i].GetEnemyPosition();
			XMFLOAT3 EnemyVel = enemyB[i].GetEnemyVelocity();
			float BoxTop;	// BOXの+Y面の座標

			BoxTop = EnemyPos.y + BOX_RADIUS;

			// 壁としての判定処理
			if (EnemyPos.y - BOX_RADIUS < PlayerPos.y &&
				PlayerPos.y < BoxTop - 0.1f)
			{
				if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
					PlayerPos.z < EnemyPos.z + BOX_RADIUS)
				{
					if (EnemyPos.x - BOX_RADIUS < PlayerPos.x + BALL_RADIUS &&
						PlayerPos.x < EnemyPos.x - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.x += (EnemyPos.x - BOX_RADIUS) - (PlayerPos.x + BALL_RADIUS);
						PlayerVel.x *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_3;
					}
					else if (EnemyPos.x + BOX_RADIUS > PlayerPos.x - BALL_RADIUS &&
						PlayerPos.x > EnemyPos.x + BOX_RADIUS)
					{//BOXの+X面にぶつかった
						PlayerPos.x += (EnemyPos.x + BOX_RADIUS) - (PlayerPos.x - BALL_RADIUS);
						PlayerVel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_1;
					}
				}
				else if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
					PlayerPos.x < EnemyPos.x + BOX_RADIUS)
				{
					if (EnemyPos.z - BOX_RADIUS < PlayerPos.z + BALL_RADIUS &&
						PlayerPos.z < EnemyPos.z - BOX_RADIUS)
					{//BOXの-Z面にぶつかったので座標の補正
						PlayerPos.z += (EnemyPos.z - BOX_RADIUS) - (PlayerPos.z + BALL_RADIUS);
						PlayerVel.z *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_0;
					}
					else if (EnemyPos.z + BOX_RADIUS > PlayerPos.z - BALL_RADIUS &&
						PlayerPos.z > EnemyPos.z + BOX_RADIUS)
					{//BOXの+Z面にぶつかった
						PlayerPos.z += (EnemyPos.z + BOX_RADIUS) - (PlayerPos.z - BALL_RADIUS);
						PlayerVel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_2;
					}
				}
			}
			//地面として判定処理
			else
			{
				if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
					PlayerPos.z < EnemyPos.z + BOX_RADIUS)
				{
					if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
						PlayerPos.x < EnemyPos.x + BOX_RADIUS)
					{
						if (EnemyPos.y - BOX_RADIUS < PlayerPos.y + BALL_RADIUS &&
							PlayerPos.y < EnemyPos.y - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.y += (EnemyPos.y - BOX_RADIUS) - (PlayerPos.y + BALL_RADIUS);
							EnemyVel.y *= -COE; //移動ベクトルの反転
							//hit = 
						}
						else if (BoxTop > PlayerPos.y - BALL_RADIUS &&
							PlayerPos.y > BoxTop)
						{//BOXの+X面にぶつかった
							PlayerPos.y += (BoxTop)-(PlayerPos.y - BALL_RADIUS);
							EnemyVel.y = EnemyVel.y * (-COE * 1.0f);
							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}


			pPlayer->SetPlayerPosition(PlayerPos);
			pPlayer->SetPlayerVelocity(PlayerVel);


			//return hit;
		}
	}

	//==================================================================
	// EnemyGround
	//==================================================================
	{
		for (int i = 0; i < Enemy_Spawner_MAX; i++)
		{
			if (enemyC[i].GetEnemyGroundType() != ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_NONE)
			{
				XMFLOAT3 EnemyPos = enemyC[i].GetEnemyPosition();
				XMFLOAT3 EnemyVel = enemyC[i].GetEnemyVelocity();
				float BoxTop;	// BOXの+Y面の座標

				BoxTop = EnemyPos.y + BOX_RADIUS;

				// 壁としての判定処理
				if (EnemyPos.y - BOX_RADIUS < PlayerPos.y &&
					PlayerPos.y < BoxTop - 0.1f)
				{
					if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
						PlayerPos.z < EnemyPos.z + BOX_RADIUS)
					{
						if (EnemyPos.x - BOX_RADIUS < PlayerPos.x + BALL_RADIUS &&
							PlayerPos.x < EnemyPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.x += (EnemyPos.x - BOX_RADIUS) - (PlayerPos.x + BALL_RADIUS);
							PlayerVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (EnemyPos.x + BOX_RADIUS > PlayerPos.x - BALL_RADIUS &&
							PlayerPos.x > EnemyPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							PlayerPos.x += (EnemyPos.x + BOX_RADIUS) - (PlayerPos.x - BALL_RADIUS);
							PlayerVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
						PlayerPos.x < EnemyPos.x + BOX_RADIUS)
					{
						if (EnemyPos.z - BOX_RADIUS < PlayerPos.z + BALL_RADIUS &&
							PlayerPos.z < EnemyPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							PlayerPos.z += (EnemyPos.z - BOX_RADIUS) - (PlayerPos.z + BALL_RADIUS);
							PlayerVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (EnemyPos.z + BOX_RADIUS > PlayerPos.z - BALL_RADIUS &&
							PlayerPos.z > EnemyPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							PlayerPos.z += (EnemyPos.z + BOX_RADIUS) - (PlayerPos.z - BALL_RADIUS);
							PlayerVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (EnemyPos.z - BOX_RADIUS < PlayerPos.z &&
						PlayerPos.z < EnemyPos.z + BOX_RADIUS)
					{
						if (EnemyPos.x - BOX_RADIUS < PlayerPos.x &&
							PlayerPos.x < EnemyPos.x + BOX_RADIUS)
						{
							if (EnemyPos.y - BOX_RADIUS < PlayerPos.y + BALL_RADIUS &&
								PlayerPos.y < EnemyPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								PlayerPos.y += (EnemyPos.y - BOX_RADIUS) - (PlayerPos.y + BALL_RADIUS);
								EnemyVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > PlayerPos.y - BALL_RADIUS &&
								PlayerPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								PlayerPos.y += (BoxTop)-(PlayerPos.y - BALL_RADIUS);
								EnemyVel.y = EnemyVel.y * (-COE * 1.0f);
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}


				pPlayer->SetPlayerPosition(PlayerPos);
				pPlayer->SetPlayerVelocity(PlayerVel);


				return hit;
			}
		}
	}


}

float	COLLISION::PlayerBombCollision(PLAYER* pPlayer, BOMB* pBomb)
{
	float		hit = false;				// ヒットした方向
	
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	bool PlayerJump = pPlayer->GetPlayerJump();

	BOMBSOURCE* Bomb = pBomb->Bomb_GetBomb();	// マップ
	RUNBOMBSPAWNER* RunBombSpawner = pBomb->Bomb_GetRunBomb();// マップ
	FLOWTBOMBSOURCE* FlowtBomb = pBomb->Bomb_GetFlowtBomb();// マップ
	//int			i = 0;
	bool bombHave = false;

	// 全てのブロックをチェック
	//Have判定
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{//ボム
		if (Bomb[i].BombSource_GetState()==BOMB_STATE::BOMB_ACTIVE_HAVE)
		{
			bombHave = true;
			pPlayer->SetPlayerTransBombFlag(false);
			pPlayer->SetPlayerBalloon(false);
			break;
		}
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{//ランボム
		RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		if (RunBomb->Runbombsource_GetState()==RUNBOMB_STATE::RUNBOMB_ACTIVE_HAVE)
		{
			bombHave = true;
			pPlayer->SetPlayerTransBombFlag(false);
			pPlayer->SetPlayerBalloon(false);
			break;
		}
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{//浮きボム
		if (FlowtBomb[i].Flowtbombsource_GetState()==BOMB_STATE::BOMB_ACTIVE_HAVE)
		{
			bombHave = true;
			pPlayer->SetPlayerTransBombFlag(false);
			pPlayer->SetPlayerBalloon(true);
			break;
		}
	}

	if (bombHave)
	{
	}
	else
	{
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			/*switch (Bomb[i].BombSource_GetState())
			{
			case BOMB_NONE:
			case BOMB_MAX:
			default:
				continue;
				break;
			}*/

			bool	touch = false;
			float BoxTop;	// BOXの+Y面の座標

			XMFLOAT3 bombPos;


			bombPos = Bomb[i].BombSource_GetPosition();



			switch (Bomb[i].BombSource_GetState())
			{

			default:
				BoxTop = bombPos.y + BOMB_RADIUS;	// 普通のBOX
				break;
			}

			// 壁としての判定処理
			if (bombPos.y - BOMB_RADIUS <= PlayerPos.y &&
				PlayerPos.y <= BoxTop - 0.1f)
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x + PLAYER_RADIUS &&
						PlayerPos.x <= bombPos.x - BOMB_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.x += (bombPos.x - BOMB_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
						PlayerVel.x *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_3;
						touch = true;
					}
					else if (bombPos.x + BOMB_RADIUS >= PlayerPos.x - PLAYER_RADIUS &&
						PlayerPos.x >= bombPos.x + BOMB_RADIUS)
					{//BOXの+X面にぶつかった
						PlayerPos.x += (bombPos.x + BOMB_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
						PlayerVel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_1;
						touch = true;
					}
				}
				else if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
					PlayerPos.x <= bombPos.x + BOMB_RADIUS)
				{
					if (bombPos.z - BOMB_RADIUS <= PlayerPos.z + PLAYER_RADIUS &&
						PlayerPos.z <= bombPos.z - BOMB_RADIUS)
					{//BOXの-Z面にぶつかったので座標の補正
						PlayerPos.z += (bombPos.z - BOMB_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
						PlayerVel.z *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_0;
						touch = true;
					}
					else if (bombPos.z + BOMB_RADIUS >= PlayerPos.z - PLAYER_RADIUS &&
						PlayerPos.z >= bombPos.z + BOMB_RADIUS)
					{//BOXの+Z面にぶつかった
						PlayerPos.z += (bombPos.z + BOMB_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
						PlayerVel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_2;
						touch = true;
					}
				}
			}
			//地面として判定処理
			else
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
						PlayerPos.x <= bombPos.x + BOMB_RADIUS)
					{
						if (bombPos.y - BOMB_RADIUS <= PlayerPos.y + PLAYER_RADIUS &&
							PlayerPos.y <= bombPos.y - BOMB_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.y += (bombPos.y - BOMB_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
							PlayerVel.y *= -COE; //移動ベクトルの反転
							//hit = 
						}
						else if (BoxTop >= PlayerPos.y - PLAYER_RADIUS &&
							PlayerPos.y >= BoxTop)
						{//BOXの+X面にぶつかった
							PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
							PlayerVel.y = PlayerVel.y * (-COE * 1.0f);
							hit = COLLISION_HIT::HIT_GROUND;
							if (PlayerJump == false)
							{
								PlayerJump = true;
							}
						}
					}
				}
			}

			if (!pPlayer->GetPlayerTransBombFlag() || !touch)
			{
				touch = false;
			}

			if (pPlayer->GetPlayerTransBombFlag())
			{
				Bomb[i].BombSource_SetTouch(touch);
			}

			pPlayer->SetPlayerJump(PlayerJump);
			pPlayer->SetPlayerPosition(PlayerPos);
			pPlayer->SetPlayerVelocity(PlayerVel);

		}

		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			/*switch (Bomb[i].BombSource_GetState())
			{
			case BOMB_NONE:
			case BOMB_MAX:
			default:
				continue;
				break;
			}*/

			bool	touch = false;
			float BoxTop;	// BOXの+Y面の座標
			bool damageTouch = false;

			XMFLOAT3 bombPos;
			RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();


			bombPos = RunBomb->Runbombsource_GetPosition();



			switch (RunBomb->Runbombsource_GetState())
			{

			default:
				BoxTop = bombPos.y + BOMB_RADIUS;	// 普通のBOX
				break;
			}

			// 壁としての判定処理
			if (bombPos.y - BOMB_RADIUS <= PlayerPos.y &&
				PlayerPos.y <= BoxTop - 0.1f)
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x + PLAYER_RADIUS &&
						PlayerPos.x <= bombPos.x - BOMB_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						hit = COLLISION_HIT::HIT_WALL_3;
						touch = true;
						damageTouch = true;
					}
					else if (bombPos.x + BOMB_RADIUS >= PlayerPos.x - PLAYER_RADIUS &&
						PlayerPos.x >= bombPos.x + BOMB_RADIUS)
					{//BOXの+X面にぶつかった
						hit = COLLISION_HIT::HIT_WALL_1;
						touch = true;
						damageTouch = true;
					}
				}
				else if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
					PlayerPos.x <= bombPos.x + BOMB_RADIUS)
				{
					if (bombPos.z - BOMB_RADIUS <= PlayerPos.z + PLAYER_RADIUS &&
						PlayerPos.z <= bombPos.z - BOMB_RADIUS)
					{//BOXの-Z面にぶつかったので座標の補正
						hit = COLLISION_HIT::HIT_WALL_0;
						touch = true;
						damageTouch = true;
					}
					else if (bombPos.z + BOMB_RADIUS >= PlayerPos.z - PLAYER_RADIUS &&
						PlayerPos.z >= bombPos.z + BOMB_RADIUS)
					{//BOXの+Z面にぶつかった
						hit = COLLISION_HIT::HIT_WALL_2;
						touch = true;
						damageTouch = true;
					}
				}
			}
			//地面として判定処理
			else
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
						PlayerPos.x <= bombPos.x + BOMB_RADIUS)
					{
						if (bombPos.y - BOMB_RADIUS <= PlayerPos.y + PLAYER_RADIUS &&
							PlayerPos.y <= bombPos.y - BOMB_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							//hit = 

						}
						else if (BoxTop >= PlayerPos.y - PLAYER_RADIUS &&
							PlayerPos.y >= BoxTop)
						{//BOXの+X面にぶつかった
							PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
							PlayerVel.y = PlayerVel.y * (-COE * 1.0f);
							hit = COLLISION_HIT::HIT_GROUND;
							if (PlayerJump == false)
							{
								PlayerJump = true;
							}
							if (RunBomb->Runbombsource_GetState() == RUNBOMB_ENEMY) {
								RunBomb->Runbombsource_SetState(RUNBOMB_ITEM);
							}
						}
					}
				}
			}

			if (!pPlayer->GetPlayerTransBombFlag() || !touch)
			{
				touch = (false);
			}

			if (pPlayer->GetPlayerTransBombFlag())
			{
				RunBomb->Runbombsource_SetTouch(touch);
			}

			pPlayer->SetPlayerJump(PlayerJump);
			pPlayer->SetPlayerPosition(PlayerPos);
			pPlayer->SetPlayerVelocity(PlayerVel);

			// テスト
			if (hit > COLLISION_HIT::HIT_GROUND && !RunBomb->Runbombsource_GetDamage()
				&& (RunBomb->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ENEMY) &&
				damageTouch)
			{
				pPlayer->SetPlayerHp(pPlayer->GetPlayerHp() - 1);
				pPlayer->PlayDamageSE();  // ダメージSE再生
				RunBomb->Runbombsource_SetDamage(true);
				continue;
			}

		}

		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			/*switch (Bomb[i].BombSource_GetState())
			{
			case BOMB_NONE:
			case BOMB_MAX:
			default:
				continue;
				break;
			}*/

			bool	touch = false;
			float BoxTop;	// BOXの+Y面の座標

			XMFLOAT3 bombPos;


			bombPos = FlowtBomb[i].Flowtbombsource_GetPosition();



			switch (FlowtBomb[i].Flowtbombsource_GetState())
			{

			default:
				BoxTop = bombPos.y + BOMB_RADIUS;	// 普通のBOX
				break;
			}

			// 壁としての判定処理
			if (bombPos.y - BOMB_RADIUS <= PlayerPos.y &&
				PlayerPos.y <= BoxTop - 0.1f)
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x + PLAYER_RADIUS &&
						PlayerPos.x <= bombPos.x - BOMB_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.x += (bombPos.x - BOMB_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
						PlayerVel.x *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_3;
						touch = true;
					}
					else if (bombPos.x + BOMB_RADIUS >= PlayerPos.x - PLAYER_RADIUS &&
						PlayerPos.x >= bombPos.x + BOMB_RADIUS)
					{//BOXの+X面にぶつかった
						PlayerPos.x += (bombPos.x + BOMB_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
						PlayerVel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_1;
						touch = true;
					}
				}
				else if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
					PlayerPos.x <= bombPos.x + BOMB_RADIUS)
				{
					if (bombPos.z - BOMB_RADIUS <= PlayerPos.z + PLAYER_RADIUS &&
						PlayerPos.z <= bombPos.z - BOMB_RADIUS)
					{//BOXの-Z面にぶつかったので座標の補正
						PlayerPos.z += (bombPos.z - BOMB_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
						PlayerVel.z *= -COE; //移動ベクトルの反転
						hit = COLLISION_HIT::HIT_WALL_0;
						touch = true;
					}
					else if (bombPos.z + BOMB_RADIUS >= PlayerPos.z - PLAYER_RADIUS &&
						PlayerPos.z >= bombPos.z + BOMB_RADIUS)
					{//BOXの+Z面にぶつかった
						PlayerPos.z += (bombPos.z + BOMB_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
						PlayerVel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_2;
						touch = true;
					}
				}
			}
			//地面として判定処理
			else
			{
				if (bombPos.z - BOMB_RADIUS <= PlayerPos.z &&
					PlayerPos.z <= bombPos.z + BOMB_RADIUS)
				{
					if (bombPos.x - BOMB_RADIUS <= PlayerPos.x &&
						PlayerPos.x <= bombPos.x + BOMB_RADIUS)
					{
						if (bombPos.y - BOMB_RADIUS <= PlayerPos.y + PLAYER_RADIUS &&
							PlayerPos.y <= bombPos.y - BOMB_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.y += (bombPos.y - BOMB_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
							PlayerVel.y *= -COE; //移動ベクトルの反転
							//hit = 
						}
						else if (BoxTop >= PlayerPos.y - PLAYER_RADIUS &&
							PlayerPos.y >= BoxTop)
						{//BOXの+X面にぶつかった
							PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
							PlayerVel.y = PlayerVel.y * (-COE * 1.0f);
							hit = COLLISION_HIT::HIT_GROUND;
							if (PlayerJump == false)
							{
								PlayerJump = true;
							}
						}
					}
				}
			}

			if (!pPlayer->GetPlayerTransBombFlag() || !touch)
			{
				touch = (false);
			}

			if (pPlayer->GetPlayerTransBombFlag())
			{
				FlowtBomb[i].Flowtbombsource_SetTouch(touch);
			}

			pPlayer->SetPlayerJump(PlayerJump);
			pPlayer->SetPlayerPosition(PlayerPos);
			pPlayer->SetPlayerVelocity(PlayerVel);

		}

	}

	pPlayer->SetPlayerBombHave(bombHave);
	return hit;  // ぶつかったかどうかを示す
}

float	COLLISION::BombFieldCollision(BOMB* pBomb, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();

	RUNBOMBSPAWNER* pRunBombSpawner = pBomb->Bomb_GetRunBomb();

	FLOWTBOMBSOURCE* pFlowtBombSource = pBomb->Bomb_GetFlowtBomb();

	MAP* Map = pField->GetFieldMap();	// マップ

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int			l = 0;

		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ITEM ||
			pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
			XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < BombPos.y &&
					BombPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x + PLAYER_RADIUS &&
							BombPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							BombPos.x += (mapPos.x - BOX_RADIUS) - (BombPos.x + PLAYER_RADIUS);
							BombVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > BombPos.x - PLAYER_RADIUS &&
							BombPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							BombPos.x += (mapPos.x + BOX_RADIUS) - (BombPos.x - PLAYER_RADIUS);
							BombVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < BombPos.x &&
						BombPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < BombPos.z + PLAYER_RADIUS &&
							BombPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							BombPos.z += (mapPos.z - BOX_RADIUS) - (BombPos.z + PLAYER_RADIUS);
							BombVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > BombPos.z - PLAYER_RADIUS &&
							BombPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							BombPos.z += (mapPos.z + BOX_RADIUS) - (BombPos.z - PLAYER_RADIUS);
							BombVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x &&
							BombPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < BombPos.y + PLAYER_RADIUS &&
								BombPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								BombPos.y += (mapPos.y - BOX_RADIUS) - (BombPos.y + PLAYER_RADIUS);
								BombVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > BombPos.y - PLAYER_RADIUS &&
								BombPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								BombPos.y += (BoxTop)-(BombPos.y - PLAYER_RADIUS);
								BombVel.y = 0;//BombVel.y * (-COE * 1.0f);

								//とりあえずのストップ
								BombVel.x = 0.0f;
								BombVel.z = 0.0f;


								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}

				l++;
				pBombSource[i].BombSource_SetPosition(BombPos);
				pBombSource[i].BombSource_SetVelocity(BombVel);
			}



		}
	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int			l = 0;
		RUNBOMBSOURCE* pRunBombSource = pRunBombSpawner[i].GetRunBombSource__RunBombSpawner();
		bool Colision = false;

		XMFLOAT3 BombVel = pRunBombSource->Runbombsource_GetVelocity();

		if (pRunBombSource->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ENEMY && pRunBombSource->Runbombsource_GetType() == RUNBOMB_TYPE_FREE)
		{
			XMFLOAT3 Rotation = pRunBombSource->Runbombsource_GetRotation();

			float yaw = Rotation.y;

			float pVecX = sinf(yaw);
			float pVecZ = cosf(yaw);

			// 正規化
			float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
			if (len > 0.0f) {
				pVecX /= len;
				pVecZ /= len;
			}

			float speed = BOMB_SPEED_MAX * 0.05f;

			BombVel.x = (pVecX)*speed;
			//BombVel.y = 0;//BOMB_THROW_POWER;  // 上方向成分（好みで調整）
			BombVel.z = (pVecZ)*speed;


			/////////////////////////////////////////////////////////////////////////

			XMFLOAT3 BombPos = pRunBombSource->Runbombsource_GetPosition();

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < BombPos.y &&
					BombPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x + PLAYER_RADIUS &&
							BombPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							BombPos.x += (mapPos.x - BOX_RADIUS) - (BombPos.x + PLAYER_RADIUS) - 0.1f;
							BombVel.x = 0.0f;
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > BombPos.x - PLAYER_RADIUS &&
							BombPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							BombPos.x += (mapPos.x + BOX_RADIUS) - (BombPos.x - PLAYER_RADIUS) + 0.1f;
							BombVel.x = 0.0f;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < BombPos.x &&
						BombPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < BombPos.z + PLAYER_RADIUS &&
							BombPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							BombPos.z += (mapPos.z - BOX_RADIUS) - (BombPos.z + PLAYER_RADIUS) - 0.1f;
							BombVel.z = 0.0f; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > BombPos.z - PLAYER_RADIUS &&
							BombPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							BombPos.z += (mapPos.z + BOX_RADIUS) - (BombPos.z - PLAYER_RADIUS) + 0.1f;
							BombVel.z = 0.0f;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x &&
							BombPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < BombPos.y + PLAYER_RADIUS &&
								BombPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								BombPos.y += (mapPos.y - BOX_RADIUS) - (BombPos.y + PLAYER_RADIUS);
								BombVel.y = 0.0f; //移動ベクトルの反転
								Colision = true;
							}
							else if (BoxTop > BombPos.y - PLAYER_RADIUS &&
								BombPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								BombPos.y += (BoxTop)-(BombPos.y - PLAYER_RADIUS);
								BombVel.y = 0.0;//BombVel.y * (-COE * 1.0f);

								Colision = true;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}

				l++;
				pRunBombSource->Runbombsource_SetFieldCollision(Colision);
				pRunBombSource->Runbombsource_SetPosition(BombPos);
				pRunBombSource->Runbombsource_SetVelocity(BombVel);

			}
		}

		if (pRunBombSource->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ITEM ||
			pRunBombSource->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW||
			pRunBombSource->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ENEMY)
		{
			XMFLOAT3 BombPos = pRunBombSource->Runbombsource_GetPosition();

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < BombPos.y &&
					BombPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x + PLAYER_RADIUS &&
							BombPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							BombPos.x += (mapPos.x - BOX_RADIUS) - (BombPos.x + PLAYER_RADIUS);
							BombVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > BombPos.x - PLAYER_RADIUS &&
							BombPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							BombPos.x += (mapPos.x + BOX_RADIUS) - (BombPos.x - PLAYER_RADIUS);
							BombVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < BombPos.x &&
						BombPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < BombPos.z + PLAYER_RADIUS &&
							BombPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							BombPos.z += (mapPos.z - BOX_RADIUS) - (BombPos.z + PLAYER_RADIUS);
							BombVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > BombPos.z - PLAYER_RADIUS &&
							BombPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							BombPos.z += (mapPos.z + BOX_RADIUS) - (BombPos.z - PLAYER_RADIUS);
							BombVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x &&
							BombPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < BombPos.y + PLAYER_RADIUS &&
								BombPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								BombPos.y += (mapPos.y - BOX_RADIUS) - (BombPos.y + PLAYER_RADIUS);
								BombVel.y = 0.0f; //移動ベクトルの反転
								Colision = true;
							}
							else if (BoxTop > BombPos.y - PLAYER_RADIUS &&
								BombPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								BombPos.y += (BoxTop)-(BombPos.y - PLAYER_RADIUS);
								BombVel.y = 0.0;//BombVel.y * (-COE * 1.0f);

								Colision = true;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}

				l++;
				pRunBombSource->Runbombsource_SetPosition(BombPos);
				pRunBombSource->Runbombsource_SetVelocity(BombVel);
			}



		}
	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int			l = 0;

		if (pFlowtBombSource[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_ITEM ||
			pFlowtBombSource[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 BombPos = pFlowtBombSource[i].Flowtbombsource_GetPosition();
			XMFLOAT3 BombVel = pFlowtBombSource[i].Flowtbombsource_GetVelocity();

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < BombPos.y &&
					BombPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x + PLAYER_RADIUS &&
							BombPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							BombPos.x += (mapPos.x - BOX_RADIUS) - (BombPos.x + PLAYER_RADIUS);
							BombVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > BombPos.x - PLAYER_RADIUS &&
							BombPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							BombPos.x += (mapPos.x + BOX_RADIUS) - (BombPos.x - PLAYER_RADIUS);
							BombVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < BombPos.x &&
						BombPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < BombPos.z + PLAYER_RADIUS &&
							BombPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							BombPos.z += (mapPos.z - BOX_RADIUS) - (BombPos.z + PLAYER_RADIUS);
							BombVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > BombPos.z - PLAYER_RADIUS &&
							BombPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							BombPos.z += (mapPos.z + BOX_RADIUS) - (BombPos.z - PLAYER_RADIUS);
							BombVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < BombPos.z &&
						BombPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < BombPos.x &&
							BombPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < BombPos.y + PLAYER_RADIUS &&
								BombPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								BombPos.y += (mapPos.y - BOX_RADIUS) - (BombPos.y + PLAYER_RADIUS);
								BombVel.y = 0.0f; //�ړ��x�N�g���̒�~
								//hit = 
							}
							else if (BoxTop > BombPos.y - PLAYER_RADIUS &&
								BombPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								BombPos.y += (BoxTop)-(BombPos.y - PLAYER_RADIUS);
								BombVel.y = 0;//BombVel.y * (-COE * 1.0f);

								//とりあえずのストップ
								BombVel.x = 0.0f;
								BombVel.z = 0.0f;


								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}

				l++;
				pFlowtBombSource[i].Flowtbombsource_SetPosition(BombPos);
				pFlowtBombSource[i].Flowtbombsource_SetVelocity(BombVel);
			}



		}
	}


	return hit;  // ぶつかったかどうかを示す
}


float COLLISION::EXPLOSIONFieldCollision(BOMB* pBomb, MAPDATA* pField)
{
	float		hit = 0.0f;				// �q�b�g��������
	//BALL*		Ball = GetBall();		// �{�[���̏��

	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();
	RUNBOMBSPAWNER* pRunBombSpawner = pBomb->Bomb_GetRunBomb();
	FLOWTBOMBSOURCE* pFlowtBomb = pBomb->Bomb_GetFlowtBomb();

	MAP* Map = pField->GetFieldMap();

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		//�X���[�̂Ƃ��̂ݓ����蔻������
		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
		{
			int			l = 0;
			bool test = false;

			XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
			XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{

				if (Map[l].MapData_GetNo() == FIELD_BREAK)
				{//���鏰�����݂���Ƃ�

					XMFLOAT3 MapPos = Map[l].MapData_GetPosition();

					float BoxTop;	// BOX��+Y�ʂ̍��W

					BoxTop = MapPos.y + BOX_RADIUS;

					// �ǂƂ��Ă̔��菈��
					if (BombPos.x + 1 > MapPos.x &&
						BombPos.x - 1 < MapPos.x)
					{
						if (BombPos.y + 1 > MapPos.y &&
							BombPos.y - 1 < MapPos.y)
						{
							if (BombPos.z + 1 > MapPos.z &&
								BombPos.z - 1 < MapPos.z)
							{
								test = true;//��������
							}
						}
					}

					//�{���̃X�e�[�g�𔚔��ɕύX
					if (test)
					{
						Map[l].MapData_SetPosition(XMFLOAT3(-100.0f, -10000.0f, -100.0f));
						test = false;

					}
				}

				l++;
			}
		}
	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (!pRunBombSpawner[i].GetUse())
			continue;

		RUNBOMBSOURCE* pRunBomb = pRunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		//�X���[�̂Ƃ��̂ݓ����蔻������
		if (pRunBomb->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_EXPLOSION ||
			pRunBomb->Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
		{
			int			l = 0;
			bool test = false;

			XMFLOAT3 BombPos = pRunBomb->Runbombsource_GetPosition();
			XMFLOAT3 BombVel = pRunBomb->Runbombsource_GetVelocity();

			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{

				if (Map[l].MapData_GetNo() == FIELD_BREAK)
				{//���鏰�����݂���Ƃ�

					XMFLOAT3 MapPos = Map[l].MapData_GetPosition();

					float BoxTop;	// BOX��+Y�ʂ̍��W

					BoxTop = MapPos.y + BOX_RADIUS;

					// �ǂƂ��Ă̔��菈��
					if (BombPos.x + 1 > MapPos.x &&
						BombPos.x - 1 < MapPos.x)
					{
						if (BombPos.y + 1 > MapPos.y &&
							BombPos.y - 1 < MapPos.y)
						{
							if (BombPos.z + 1 > MapPos.z &&
								BombPos.z - 1 < MapPos.z)
							{
								test = true;//��������
							}
						}
					}

					//�{���̃X�e�[�g�𔚔��ɕύX
					if (test)
					{
						Map[l].MapData_SetPosition(XMFLOAT3(-100.0f, -10000.0f, -100.0f));
						test = false;
						pRunBomb->Runbombsource_SetCount(0.0f);
						pRunBomb->Runbombsource_SetState(RUNBOMB_STATE::RUNBOMB_EXPLOSION);
					}
				}

				l++;
			}
		}
	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		//�X���[�̂Ƃ��̂ݓ����蔻������
		if (pFlowtBomb[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
		{
			int			l = 0;
			bool test = false;

			XMFLOAT3 BombPos = pFlowtBomb[i].Flowtbombsource_GetPosition();
			XMFLOAT3 BombVel = pFlowtBomb[i].Flowtbombsource_GetVelocity();

			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{

				if (Map[l].MapData_GetNo() == FIELD_BREAK)
				{//���鏰�����݂���Ƃ�

					XMFLOAT3 MapPos = Map[l].MapData_GetPosition();

					float BoxTop;	// BOX��+Y�ʂ̍��W

					BoxTop = MapPos.y + BOX_RADIUS;

					// �ǂƂ��Ă̔��菈��
					if (BombPos.x + 1 > MapPos.x &&
						BombPos.x - 1 < MapPos.x)
					{
						if (BombPos.y + 1 > MapPos.y &&
							BombPos.y - 1 < MapPos.y)
						{
							if (BombPos.z + 1 > MapPos.z &&
								BombPos.z - 1 < MapPos.z)
							{
								test = true;//��������
							}
						}
					}

					//�{���̃X�e�[�g�𔚔��ɕύX
					if (test)
					{
						Map[l].MapData_SetPosition(XMFLOAT3(-100.0f, -100.0f, -100.0f));
						test = false;

					}
				}

				l++;
			}
		}
	}

	return hit;
}

//�{���ƃG�l�~�[�̓����蔻��̕s������̂ŏC������
float	COLLISION::BombEnemyCollision(BOMB* pBomb, ENEMYSPAWNER* pEnemy)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報

	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();
	RUNBOMBSPAWNER* RunBombSpawner = pBomb->Bomb_GetRunBomb();// マップ
	FLOWTBOMBSOURCE* FlowtBomb = pBomb->Bomb_GetFlowtBomb();// マップ


	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();
	ENEMY_GROUND* enemyG = pEnemy->EnemySpawner_GetEnemyGround();

	{
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemy[l].SetEnemyNormalState(ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_DEAD);
						}

					}
				}
			}
		}

		// 浮いてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemyB[l].SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_DEAD);
							test = false;
						}

					}
				}
			}
		}

		// GroundEnemy
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
			{
				
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyG[l].GetEnemyGroundType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyG[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyG[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemyG[l].SetEnemyGroundState(ENEMY_GROUND_STATE_DEAD);
							test = false;
						}

					}
				}
			}
		}
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//そくばく
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									pBombSource[i].BombSource_SetCount(0.0f);
									pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
								}
							}
						}



					}
				}
			}
		}

		// 浮いてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									pBombSource[i].BombSource_SetCount(0.0f);
									pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
								}
							}
						}


					}
				}
			}
		}

		// あるいてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyG[l].GetEnemyGroundType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyG[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyG[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS+0.5f;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									pBombSource[i].BombSource_SetCount(0.0f);
									pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
								}
							}
						}


					}
				}
			}
		}
	}

	{
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();// マップ
			bool test = false;

			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_EXPLOSION)
			{
				int			l = 0;
				
				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemy[l].SetEnemyNormalState(ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_DEAD);
						}

					}
				}
			}
		}

		// 浮いてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();// マップ
			bool test = false;

			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_EXPLOSION)
			{
				
				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemyB[l].SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_DEAD);
						}

					}
				}
			}
		}
		// 浮いてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();// マップ
			bool test = false;

			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_EXPLOSION)
			{
				
				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyG[l].GetEnemyGroundType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyG[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyG[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemyG[l].SetEnemyGroundState(ENEMY_GROUND_STATE_DEAD);
						}

					}
				}
			}

			//そくばく
			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
			{
				bool test = false;

				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									RunBomb[i].Runbombsource_SetCount(0.0f);
									RunBomb[i].Runbombsource_SetState(RUNBOMB_STATE::RUNBOMB_EXPLOSION);
								}
							}
						}



					}
				}
			}


			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
			{
				bool test = false;

				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									RunBomb[i].Runbombsource_SetCount(0.0f);
									RunBomb[i].Runbombsource_SetState(RUNBOMB_STATE::RUNBOMB_EXPLOSION);
								}
							}
						}


					}
				}
			}

			//爆発したときのみ当たり判定を取る
			if (RunBomb[i].Runbombsource_GetState() == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
			{
				bool test = false;

				XMFLOAT3 BombPos = RunBomb[i].Runbombsource_GetPosition();
				XMFLOAT3 BombVel = RunBomb[i].Runbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyG[l].GetEnemyGroundType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyG[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyG[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									RunBomb[i].Runbombsource_SetCount(0.0f);
									RunBomb[i].Runbombsource_SetState(RUNBOMB_STATE::RUNBOMB_EXPLOSION);
								}
							}
						}


					}
				}
			}
		}

	}


	///////////////////////////////////////////////////////////uki
	{
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (FlowtBomb[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = FlowtBomb[i].Flowtbombsource_GetPosition();
				XMFLOAT3 BombVel = FlowtBomb[i].Flowtbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemy[l].SetEnemyNormalState(ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_DEAD);
						}

					}
				}
			}
		}

		// 浮いてる敵
		for (int i = 0; i < BOMB_NUM_MAX; i++)
		{
			//爆発したときのみ当たり判定を取る
			if (FlowtBomb[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = FlowtBomb[i].Flowtbombsource_GetPosition();
				XMFLOAT3 BombVel = FlowtBomb[i].Flowtbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									test = true;//死亡フラグ
								}
							}
						}

						//ステートを死亡に移動
						if (test)
						{
							enemyB[l].SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_DEAD);
						}

					}
				}
			}

			//そくばく
			//爆発したときのみ当たり判定を取る
			if (FlowtBomb[i].Flowtbombsource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
			{
				bool test = false;

				XMFLOAT3 BombPos = FlowtBomb[i].Flowtbombsource_GetPosition();
				XMFLOAT3 BombVel = FlowtBomb[i].Flowtbombsource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemy[l].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemy[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemy[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									FlowtBomb[i].Flowtbombsource_SetCount(0.0f);
									FlowtBomb[i].Flowtbombsource_SetState(BOMB_STATE::BOMB_EXPLOSION);
								}
							}
						}



					}
				}
			}
			//爆発したときのみ当たり判定を取る
			if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
			{
				int			l = 0;
				bool test = false;

				XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
				XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

				for (int l = 0; l < Enemy_Spawner_MAX; l++)
				{

					if (enemyB[l].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
					{//エネミーが存在するとき

						XMFLOAT3 EnemyPos = enemyB[l].GetEnemyPosition();
						XMFLOAT3 EnemyVel = enemyB[l].GetEnemyVelocity();


						float BoxTop;	// BOXの+Y面の座標

						BoxTop = EnemyPos.y + BOX_RADIUS;

						// 壁としての判定処理
						if (BombPos.x + 1 > EnemyPos.x &&
							BombPos.x - 1 < EnemyPos.x)
						{
							if (BombPos.y + 1 > EnemyPos.y &&
								BombPos.y - 1 < EnemyPos.y)
							{
								if (BombPos.z + 1 > EnemyPos.z &&
									BombPos.z - 1 < EnemyPos.z)
								{
									pBombSource[i].BombSource_SetCount(0.0f);
									pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
								}
							}
						}


					}
				}
			}
		}
	}


	return hit;
}


////////////////////////////////////////////////////////////////////////////////////
// 要修正
////////////////////////////////////////////////////////////////////////////////////
float	COLLISION ::EXPLOSIONEnemyCollision(BOMB* pBomb, ENEMYSPAWNER* pEnemy)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報

	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();
	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();


	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		//スローのときのみ当たり判定を取る
		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			int			l = 0;
			bool test = false;

			XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
			XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

			for (int i = 0; i < Enemy_Spawner_MAX; i++)
			{

				if (enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
				{//エネミーが存在するとき

					XMFLOAT3 EnemyPos = enemy[i].GetEnemyPosition();
					XMFLOAT3 EnemyVel = enemy[i].GetEnemyVelocity();


					float BoxTop;	// BOXの+Y面の座標

					BoxTop = EnemyPos.y + BOX_RADIUS;

					// 壁としての判定処理
					if (BombPos.x + 1 > EnemyPos.x &&
						BombPos.x - 1 < EnemyPos.x)
					{
						if (BombPos.y + 1 > EnemyPos.y &&
							BombPos.y - 1 < EnemyPos.y)
						{
							if (BombPos.z + 1 > EnemyPos.z &&
								BombPos.z - 1 < EnemyPos.z)
							{
								test = true;//当たった
							}
						}
					}

					//ボムのステートを爆発に変更
					if (test)
					{
						pBombSource[i].BombSource_SetCount(0.0f);
						pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
						test = false;

					}

				}
			}
		}
	}


	// 浮いてる敵
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		//スローのときのみ当たり判定を取る
		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			int			l = 0;
			bool test = false;

			XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
			XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();

			for (int i = 0; i < Enemy_Spawner_MAX; i++)
			{

				if (enemyB[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
				{//エネミーが存在するとき

					XMFLOAT3 EnemyPos = enemyB[i].GetEnemyPosition();
					XMFLOAT3 EnemyVel = enemyB[i].GetEnemyVelocity();


					float BoxTop;	// BOXの+Y面の座標

					BoxTop = EnemyPos.y + BOX_RADIUS;

					// 壁としての判定処理
					if (BombPos.x + 1 > EnemyPos.x &&
						BombPos.x - 1 < EnemyPos.x)
					{
						if (BombPos.y + 1 > EnemyPos.y &&
							BombPos.y - 1 < EnemyPos.y)
						{
							if (BombPos.z + 1 > EnemyPos.z &&
								BombPos.z - 1 < EnemyPos.z)
							{
								test = true;//当たった
							}
						}
					}

					//ボムのステートを爆発に変更
					if (test)
					{
						pBombSource[i].BombSource_SetState(BOMB_STATE::BOMB_EXPLOSION);
						test = false;

					}

				}
			}
		}
	}

	return hit;
}

float COLLISION::WeaponFieldCollision(WEAPON* pWeapon, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	WEAPONSOURCE* pWeaponSource = pWeapon->Weapon_GetWeapon();

	MAP* Map = pField->GetFieldMap();	// マップ

	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		int			l = 0;

		if (pWeaponSource[i].WeaponSource_GetState() != WEAPON_STATE::WEAPON_NONE)
		{
			XMFLOAT3 WeaponPos = pWeaponSource[i].WeaponSource_GetPosition();
			XMFLOAT3 WeaponVel = pWeaponSource[i].WeaponSource_GetVelocity();

			// 全てのブロックをチェック
			while (Map[l].MapData_GetNo() != FIELD_MAX)
			{
				float BoxTop;	// BOXの+Y面の座標

				XMFLOAT3 mapPos = Map[l].MapData_GetPosition();

				switch (Map[l].MapData_GetNo())
				{

				default:
					BoxTop = mapPos.y + BOX_RADIUS;	// 普通のBOX
					break;
				}

				// 壁としての判定処理
				if (mapPos.y - BOX_RADIUS < WeaponPos.y &&
					WeaponPos.y < BoxTop - 0.1f)
				{
					if (mapPos.z - BOX_RADIUS < WeaponPos.z &&
						WeaponPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < WeaponPos.x + PLAYER_RADIUS &&
							WeaponPos.x < mapPos.x - BOX_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							WeaponPos.x += (mapPos.x - BOX_RADIUS) - (WeaponPos.x + PLAYER_RADIUS);
							WeaponVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (mapPos.x + BOX_RADIUS > WeaponPos.x - PLAYER_RADIUS &&
							WeaponPos.x > mapPos.x + BOX_RADIUS)
						{//BOXの+X面にぶつかった
							WeaponPos.x += (mapPos.x + BOX_RADIUS) - (WeaponPos.x - PLAYER_RADIUS);
							WeaponVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					else if (mapPos.x - BOX_RADIUS < WeaponPos.x &&
						WeaponPos.x < mapPos.x + BOX_RADIUS)
					{
						if (mapPos.z - BOX_RADIUS < WeaponPos.z + PLAYER_RADIUS &&
							WeaponPos.z < mapPos.z - BOX_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							WeaponPos.z += (mapPos.z - BOX_RADIUS) - (WeaponPos.z + PLAYER_RADIUS);
							WeaponVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (mapPos.z + BOX_RADIUS > WeaponPos.z - PLAYER_RADIUS &&
							WeaponPos.z > mapPos.z + BOX_RADIUS)
						{//BOXの+Z面にぶつかった
							WeaponPos.z += (mapPos.z + BOX_RADIUS) - (WeaponPos.z - PLAYER_RADIUS);
							WeaponVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (mapPos.z - BOX_RADIUS < WeaponPos.z &&
						WeaponPos.z < mapPos.z + BOX_RADIUS)
					{
						if (mapPos.x - BOX_RADIUS < WeaponPos.x &&
							WeaponPos.x < mapPos.x + BOX_RADIUS)
						{
							if (mapPos.y - BOX_RADIUS < WeaponPos.y + PLAYER_RADIUS &&
								WeaponPos.y < mapPos.y - BOX_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								WeaponPos.y += (mapPos.y - BOX_RADIUS) - (WeaponPos.y + PLAYER_RADIUS);
								WeaponVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop > WeaponPos.y - PLAYER_RADIUS &&
								WeaponPos.y > BoxTop)
							{//BOXの+X面にぶつかった
								WeaponPos.y += (BoxTop)-(WeaponPos.y - PLAYER_RADIUS);
								WeaponVel.y = 0;//WeaponVel.y * (-COE * 1.0f);

								//とりあえずのストップ
								WeaponVel.x = 0.0f;
								WeaponVel.z = 0.0f;


								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}

				l++;
				pWeaponSource[i].WeaponSource_SetPosition(WeaponPos);
				pWeaponSource[i].WeaponSource_SetVelocity(WeaponVel);
			}



		}
	}


	return hit;  // ぶつかったかどうかを示す
}


float COLLISION::PlayerWeaponCollision(PLAYER* pPlayer, WEAPON* pWeapon)
{
	float		hit = false;				// ヒットした方向


	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	bool PlayerJump = pPlayer->GetPlayerJump();

	WEAPONSOURCE* Weapon = pWeapon->Weapon_GetWeapon();	// マップ
	//int			i = 0;
	WEAPONENEMYGROUND* GroundWeapon = pWeapon->EG_Weapon_GetWeapon();

	// 全てのブロックをチェック
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		{
			/*switch (Weapon[i].WeaponSource_GetState())
			{
			case WEAPON_NONE:
			case WEAPON_MAX:
			default:
				continue;
				break;
			}*/

			bool	touch = false;
			float BoxTop;	// BOXの+Y面の座標

			XMFLOAT3 weaponPos = Weapon[i].WeaponSource_GetPosition();


			switch (Weapon[i].WeaponSource_GetState())
			{

			default:
				BoxTop = weaponPos.y + WEAPON_RADIUS;	// 普通のBOX
				break;
			}

			if (Weapon[i].WeaponSource_GetState() != WEAPON_NONE)
			{
				// 壁としての判定処理
				if (weaponPos.y - WEAPON_RADIUS <= PlayerPos.y &&
					PlayerPos.y <= BoxTop - 0.1f)
				{
					if (weaponPos.z - WEAPON_RADIUS <= PlayerPos.z &&
						PlayerPos.z <= weaponPos.z + WEAPON_RADIUS)
					{
						if (weaponPos.x - WEAPON_RADIUS <= PlayerPos.x + PLAYER_RADIUS &&
							PlayerPos.x <= weaponPos.x - WEAPON_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							PlayerPos.x += (weaponPos.x - WEAPON_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
							PlayerVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
							touch = true;
						}
						else if (weaponPos.x + WEAPON_RADIUS >= PlayerPos.x - PLAYER_RADIUS &&
							PlayerPos.x >= weaponPos.x + WEAPON_RADIUS)
						{//BOXの+X面にぶつかった
							PlayerPos.x += (weaponPos.x + WEAPON_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
							PlayerVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
							touch = true;
						}
					}
					else if (weaponPos.x - WEAPON_RADIUS <= PlayerPos.x &&
						PlayerPos.x <= weaponPos.x + WEAPON_RADIUS)
					{
						if (weaponPos.z - WEAPON_RADIUS <= PlayerPos.z + PLAYER_RADIUS &&
							PlayerPos.z <= weaponPos.z - WEAPON_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							PlayerPos.z += (weaponPos.z - WEAPON_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
							PlayerVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
							touch = true;
						}
						else if (weaponPos.z + WEAPON_RADIUS >= PlayerPos.z - PLAYER_RADIUS &&
							PlayerPos.z >= weaponPos.z + WEAPON_RADIUS)
						{//BOXの+Z面にぶつかった
							PlayerPos.z += (weaponPos.z + WEAPON_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
							PlayerVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
							touch = true;
						}
					}
				}
				//地面として判定処理
				else
				{
					if (weaponPos.z - WEAPON_RADIUS <= PlayerPos.z &&
						PlayerPos.z <= weaponPos.z + WEAPON_RADIUS)
					{
						if (weaponPos.x - WEAPON_RADIUS <= PlayerPos.x &&
							PlayerPos.x <= weaponPos.x + WEAPON_RADIUS)
						{
							if (weaponPos.y - WEAPON_RADIUS <= PlayerPos.y + PLAYER_RADIUS &&
								PlayerPos.y <= weaponPos.y - WEAPON_RADIUS)
							{//BOXの-X面にぶつかったので座標の補正
								PlayerPos.y += (weaponPos.y - WEAPON_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
								PlayerVel.y *= -COE; //移動ベクトルの反転
								//hit = 
							}
							else if (BoxTop >= PlayerPos.y - PLAYER_RADIUS &&
								PlayerPos.y >= BoxTop)
							{//BOXの+X面にぶつかった
								PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
								PlayerVel.y = PlayerVel.y * (-COE * 1.0f);
								hit = COLLISION_HIT::HIT_GROUND;
								if (PlayerJump == false)
								{
									PlayerJump = true;
								}
							}
						}
					}
				}

				pPlayer->SetPlayerJump(PlayerJump);
				pPlayer->SetPlayerPosition(PlayerPos);
				pPlayer->SetPlayerVelocity(PlayerVel);

				// テスト
				if (hit && !Weapon[i].WeaponSource_GetIsDamage()
					&& Weapon[i].WeaponSource_GetState() != WEAPON_NONE)
				{
					pPlayer->SetPlayerHp(pPlayer->GetPlayerHp() - 1);
					pPlayer->SetIsDamage(true);
					Weapon[i].WeaponSource_SetIsDamage(true);
				}
			}
		}
		//===============================================================
		// EnemyGround
		//===============================================================

		{

			bool	touch = false;
			float BoxTop;	// BOXの+Y面の座標

			XMFLOAT3 weaponPos = GroundWeapon[i].Weapon_EG_GetPosition();


			switch (GroundWeapon[i].Weapon_EG_GetState())
			{

			default:
				BoxTop = weaponPos.y + WEAPON_RADIUS;	// 普通のBOX
				break;
			}

			if (GroundWeapon[i].Weapon_EG_GetState() != EG_WEAPON_NONE)
			{
				// 壁としての判定処理
				if (weaponPos.y - WEAPON_RADIUS <= PlayerPos.y &&
					PlayerPos.y <= BoxTop - 0.1f)
				{
					if (weaponPos.z - WEAPON_RADIUS <= PlayerPos.z &&
						PlayerPos.z <= weaponPos.z + WEAPON_RADIUS)
					{
						if (weaponPos.x - WEAPON_RADIUS <= PlayerPos.x + PLAYER_RADIUS &&
							PlayerPos.x <= weaponPos.x - WEAPON_RADIUS)
						{//BOXの-X面にぶつかったので座標の補正
							//PlayerPos.x += (weaponPos.x - WEAPON_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
							//PlayerVel.x *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_3;
							touch = true;
						}
						else if (weaponPos.x + WEAPON_RADIUS >= PlayerPos.x - PLAYER_RADIUS &&
							PlayerPos.x >= weaponPos.x + WEAPON_RADIUS)
						{//BOXの+X面にぶつかった
							/*PlayerPos.x += (weaponPos.x + WEAPON_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
							PlayerVel.x *= -COE;*/
							hit = COLLISION_HIT::HIT_WALL_1;
							touch = true;
						}
					}
					else if (weaponPos.x - WEAPON_RADIUS <= PlayerPos.x &&
						PlayerPos.x <= weaponPos.x + WEAPON_RADIUS)
					{
						if (weaponPos.z - WEAPON_RADIUS <= PlayerPos.z + PLAYER_RADIUS &&
							PlayerPos.z <= weaponPos.z - WEAPON_RADIUS)
						{//BOXの-Z面にぶつかったので座標の補正
							//PlayerPos.z += (weaponPos.z - WEAPON_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
							//PlayerVel.z *= -COE; //移動ベクトルの反転
							hit = COLLISION_HIT::HIT_WALL_0;
							touch = true;
						}
						else if (weaponPos.z + WEAPON_RADIUS >= PlayerPos.z - PLAYER_RADIUS &&
							PlayerPos.z >= weaponPos.z + WEAPON_RADIUS)
						{//BOXの+Z面にぶつかった
							/*PlayerPos.z += (weaponPos.z + WEAPON_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
							PlayerVel.z *= -COE;*/
							hit = COLLISION_HIT::HIT_WALL_2;
							touch = true;
						}
					}
				}


				pPlayer->SetPlayerJump(PlayerJump);
				pPlayer->SetPlayerPosition(PlayerPos);
				pPlayer->SetPlayerVelocity(PlayerVel);

				// テスト
				if (hit && !GroundWeapon[i].Weapon_EG_GetIsDamage()
					&& GroundWeapon[i].Weapon_EG_GetState() != EG_WEAPON_NONE)
				{
					pPlayer->SetPlayerHp(pPlayer->GetPlayerHp() - 1);
					pPlayer->SetIsDamage(true);
					GroundWeapon[i].Weapon_EG_SetIsDamage(true);
				}

			}
		}



	}

	return hit; // ぶつかったかどうかを示す
}



float COLLISION::PlayerGimmickCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick)
{
	float		hit = 0.0f;				// ヒットした方向
	bool grounded_any = false;			// 何かに接地しているかどうか
	const float BUTTON_RADIUS = 2.0f;			// ボタンの当たり判定の半径

	pGimmick->Channels_Reset(); // チャンネルのカウントをリセット

	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	bool PlayerJump = pPlayer->GetPlayerJump();

	GIMMICK_BUTTON* Buttons = pGimmick->GetButtons();
	GIMMICK_FIELD* Fields = pGimmick->GetFields();
	GIMMICK_GATE* Gates = pGimmick->GetGates();

	// ボタン
	for (int i = 0;i < pGimmick->GetButtonCount();i++)
	{
		bool on_this = false; // ← このボタンに対して今フレーム接地したか

		float BoxTop;	// BOXの+Y面の座標

		XMFLOAT3 buttonsPos = Buttons[i].GimmickButton_GetPosition();

		
			BoxTop = buttonsPos.y + 0.2f;	// 普通のBOX

		

		// 壁としての判定処理
		if (buttonsPos.y - BUTTON_RADIUS < PlayerPos.y &&
			PlayerPos.y < BoxTop - 0.1f)
		{
			if (buttonsPos.z - BUTTON_RADIUS < PlayerPos.z &&
				PlayerPos.z < buttonsPos.z + BUTTON_RADIUS)
			{
				if (buttonsPos.x - BUTTON_RADIUS < PlayerPos.x + PLAYER_RADIUS &&
					PlayerPos.x < buttonsPos.x - BUTTON_RADIUS)
				{//BOXの-X面にぶつかったので座標の補正
					PlayerPos.x += (buttonsPos.x - BUTTON_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
					PlayerVel.x *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_3;
				}
				else if (buttonsPos.x + BUTTON_RADIUS > PlayerPos.x - PLAYER_RADIUS &&
					PlayerPos.x > buttonsPos.x + BUTTON_RADIUS)
				{//BOXの+X面にぶつかった
					PlayerPos.x += (buttonsPos.x + BUTTON_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
					PlayerVel.x *= -COE;
					hit = COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if (buttonsPos.x - BUTTON_RADIUS < PlayerPos.x &&
				PlayerPos.x < buttonsPos.x + BOX_RADIUS)
			{
				if (buttonsPos.z - BUTTON_RADIUS < PlayerPos.z + PLAYER_RADIUS &&
					PlayerPos.z < buttonsPos.z - BUTTON_RADIUS)
				{//BOXの-Z面にぶつかったので座標の補正
					PlayerPos.z += (buttonsPos.z - BUTTON_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
					PlayerVel.z *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_0;
				}
				else if (buttonsPos.z + BUTTON_RADIUS > PlayerPos.z - PLAYER_RADIUS &&
					PlayerPos.z > buttonsPos.z + BUTTON_RADIUS)
				{//BOXの+Z面にぶつかった
					PlayerPos.z += (buttonsPos.z + BUTTON_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
					PlayerVel.z *= -COE;
					hit = COLLISION_HIT::HIT_WALL_2;
				}
			}
		}
		//地面として判定処理
		else
		{
			if (buttonsPos.z - BUTTON_RADIUS < PlayerPos.z &&
				PlayerPos.z < buttonsPos.z + BUTTON_RADIUS)
			{
				if (buttonsPos.x - BUTTON_RADIUS < PlayerPos.x &&
					PlayerPos.x < buttonsPos.x + BUTTON_RADIUS)
				{
					if (buttonsPos.y - BOX_RADIUS < PlayerPos.y + PLAYER_RADIUS &&
						PlayerPos.y < buttonsPos.y - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.y += (buttonsPos.y - BOX_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
						PlayerVel.y *= -COE; //移動ベクトルの反転
					}
					else if (BoxTop > PlayerPos.y - PLAYER_RADIUS &&
						PlayerPos.y > BoxTop)
					{//BOXの+Y面にぶつかった
						PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
						PlayerVel.y = 0;
						hit = COLLISION_HIT::HIT_GROUND;
						grounded_any = true;
						on_this = true;

						if (PlayerJump == false)
						{
							PlayerJump = true;
						}

					}
				}
			}
		}


		if (on_this)
		{
			Buttons[i].GimmickButton_SetTouch(true);
			int ch = Buttons[i].GimmickButton_GetChannel();
			pGimmick->Channels_AddCount(ch);
		}
		else
		{
			// 今フレームこのボタンに接地していないなら OFF
			if (Buttons[i].GimmickButton_GetTouch())
			{
				Buttons[i].GimmickButton_SetTouch(false);
			}
		}


		pPlayer->SetPlayerJump(PlayerJump);
		pPlayer->SetPlayerPosition(PlayerPos);
		pPlayer->SetPlayerVelocity(PlayerVel);

	}

	//return hit;  // ぶつかったかどうかを示す

	for (int i = 0; i < pGimmick->GetFieldCount(); i++)
	{
		bool on_this = false; // ← このボタンに対して今フレーム接地したか

		float BoxTop;	// BOXの+Y面の座標

		XMFLOAT3 fieldsPos = Fields[i].GimmickField_GetPosition();


		BoxTop = fieldsPos.y + BOX_RADIUS;	// 普通のBOX



		// 壁としての判定処理
		if (fieldsPos.y - BOX_RADIUS < PlayerPos.y &&
			PlayerPos.y < BoxTop - 0.1f)
		{
			if (fieldsPos.z - BOX_RADIUS < PlayerPos.z &&
				PlayerPos.z < fieldsPos.z + BOX_RADIUS)
			{
				if (fieldsPos.x - BOX_RADIUS < PlayerPos.x + PLAYER_RADIUS &&
					PlayerPos.x < fieldsPos.x - BOX_RADIUS)
				{//BOXの-X面にぶつかったので座標の補正
					PlayerPos.x += (fieldsPos.x - BOX_RADIUS) - (PlayerPos.x + PLAYER_RADIUS);
					PlayerVel.x *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_3;
				}
				else if (fieldsPos.x + BOX_RADIUS > PlayerPos.x - PLAYER_RADIUS &&
					PlayerPos.x > fieldsPos.x + BOX_RADIUS)
				{//BOXの+X面にぶつかった
					PlayerPos.x += (fieldsPos.x + BOX_RADIUS) - (PlayerPos.x - PLAYER_RADIUS);
					PlayerVel.x *= -COE;
					hit = COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if (fieldsPos.x - BOX_RADIUS < PlayerPos.x &&
				PlayerPos.x < fieldsPos.x + BOX_RADIUS)
			{
				if (fieldsPos.z - BOX_RADIUS < PlayerPos.z + PLAYER_RADIUS &&
					PlayerPos.z < fieldsPos.z - BOX_RADIUS)
				{//BOXの-Z面にぶつかったので座標の補正
					PlayerPos.z += (fieldsPos.z - BOX_RADIUS) - (PlayerPos.z + PLAYER_RADIUS);
					PlayerVel.z *= -COE; //移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_0;
				}
				else if (fieldsPos.z + BOX_RADIUS > PlayerPos.z - PLAYER_RADIUS &&
					PlayerPos.z > fieldsPos.z + BOX_RADIUS)
				{//BOXの+Z面にぶつかった
					PlayerPos.z += (fieldsPos.z + BOX_RADIUS) - (PlayerPos.z - PLAYER_RADIUS);
					PlayerVel.z *= -COE;
					hit = COLLISION_HIT::HIT_WALL_2;
				}
			}
		}
		//地面として判定処理
		else
		{
			if (fieldsPos.z - BOX_RADIUS < PlayerPos.z &&
				PlayerPos.z < fieldsPos.z + BOX_RADIUS)
			{
				if (fieldsPos.x - BOX_RADIUS < PlayerPos.x &&
					PlayerPos.x < fieldsPos.x + BOX_RADIUS)
				{
					if (fieldsPos.y - BOX_RADIUS < PlayerPos.y + PLAYER_RADIUS &&
						PlayerPos.y < fieldsPos.y - BOX_RADIUS)
					{//BOXの-X面にぶつかったので座標の補正
						PlayerPos.y += (fieldsPos.y - BOX_RADIUS) - (PlayerPos.y + PLAYER_RADIUS);
						PlayerVel.y *= -COE; //移動ベクトルの反転
					}
					else if (BoxTop > PlayerPos.y - PLAYER_RADIUS &&
						PlayerPos.y > BoxTop)
					{//BOXの+Y面にぶつかった
						PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
						PlayerVel.y = 0;
						hit = COLLISION_HIT::HIT_GROUND;
						grounded_any = true;
						on_this = true;

						if (PlayerJump == false)
						{
							PlayerJump = true;
						}
					}
				}
			}
		}

		pPlayer->SetPlayerJump(PlayerJump);
		pPlayer->SetPlayerPosition(PlayerPos);
		pPlayer->SetPlayerVelocity(PlayerVel);

	}


	if (grounded_any) { hit = COLLISION_HIT::HIT_GROUND; } // ← 最終まとめ
	return hit;  // ぶつかったかどうかを示す
}

//////当たり判定分からん//////
float COLLISION::BossObjPlayerCollision(BOSSOBJ* bossObjs, PLAYER* pPlayer)
{
	bool hit = false;

	XMFLOAT3 playerPos = pPlayer->GetPlayerPosition();
	float playerRadius = PLAYER_RADIUS;

	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		if (!bossObjs[i].IsActive()) continue;

		XMFLOAT3 bossPos = bossObjs[i].GetBossObjPosition();
		float bossRadius = BOSSOBJ_RADIUS;

		float dx = playerPos.x - bossPos.x;
		float dy = playerPos.y - bossPos.y;
		float dz = playerPos.z - bossPos.z;

		float distSq = dx * dx + dy * dy + dz * dz;
		float r = playerRadius + bossRadius;

		if (distSq <= r * r)
		{
			hit = true;

			pPlayer->SetPlayerHp(pPlayer->GetPlayerHp() - BOSSOBJ_DAMAGE);
			pPlayer->PlayDamageSE();  // ダメージSE再生
			bossObjs[i].SetActive(false);
		}
	}

	return hit ? 1.0f : 0.0f;
}
	


float COLLISION::PlayerMovingFieldCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick)
{

	XMFLOAT3 playerPos = pPlayer->GetPlayerPosition();

	// 押下中ボタンのY追従（必要に応じてX/Zは追従しない）
	GIMMICK_BUTTON* buttons = pGimmick->GetButtons();

	for (int i = 0; i < pGimmick->GetButtonCount(); i++)
	{

		if (buttons[i].GimmickButton_GetTouch())
		{
			XMFLOAT3 bpos = buttons[i].GimmickButton_GetPosition();

			bool overlapXZ = false; // XZ平面で重なっているか
			if (bpos.z - BOX_RADIUS < playerPos.z)
			{
				if (playerPos.z < bpos.z + BOX_RADIUS)
				{
					if (bpos.x - BOX_RADIUS < playerPos.x)
					{
						if (playerPos.x < bpos.x + BOX_RADIUS)
						{
							overlapXZ = true;
						}
					}
				}
			}
			if (overlapXZ)
			{
				XMFLOAT3 bvel = buttons[i].GimmickButton_GetVelocity();
				playerPos.y += bvel.y; // 沈み／戻り分を追従
			}
		}
	}


	// 可動フィールドへの追従（X/Y/Z すべて追従する）
	GIMMICK_FIELD* fields = pGimmick->GetFields();
	int fldCount = pGimmick->GetFieldCount();
	for (int i = 0; i < fldCount; i++)
	{
		XMFLOAT3 fpos = fields[i].GimmickField_GetPosition();
		bool overlapXZ = false;
		if (fpos.z - BOX_RADIUS < playerPos.z)
		{
			if (playerPos.z < fpos.z + BOX_RADIUS)
			{
				if (fpos.x - BOX_RADIUS < playerPos.x)
				{
					if (playerPos.x < fpos.x + BOX_RADIUS)
					{
						overlapXZ = true;
					}
				}
			}
		}
		if (overlapXZ)
		{
			float top = fpos.y + BOX_RADIUS;
			if (top + GROUND_SLOP > playerPos.y - PLAYER_RADIUS)
			{
				playerPos.x += fields[i].m_Velocity.x;
				playerPos.y += fields[i].m_Velocity.y;
				playerPos.z += fields[i].m_Velocity.z;
			}
		}

	}

	pPlayer->SetPlayerPosition(playerPos);
	return 0.0f;

}

float COLLISION::PlayerGateCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick)
{
	float hit = 0.0f;

	XMFLOAT3 pos = pPlayer->GetPlayerPosition();
	XMFLOAT3 vel = pPlayer->GetPlayerVelocity();

	GIMMICK_GATE* gates = pGimmick->GetGates();
	int gateCount = pGimmick->GetGateCount();

	// AABB 半径（ハーフサイズ）
	const float hx = GATE_PANEL_HALF_W; // X半幅（＝厚み方向）
	const float hy = GATE_PANEL_HALF_H; // Y半幅（高さ）
	const float hz = GATE_PANEL_HALF_D; // Z半幅（奥行）  ※描画は 1x1x1

	for (int i = 0; i < gateCount; ++i)
	{
		// ゲート基準と開度
		XMFLOAT3 base = gates[i].GimmickGate_GetPosition();
		float open = gates[i].GimmickGate_GetOpen();
		float offset = GATE_MAX_OPEN_OFFSET * open;

		// 描画と同じ「中心：base.x ± offset」
		XMFLOAT3 centers[2] = {
			XMFLOAT3(base.x - offset, base.y, base.z), // Left panel center
			XMFLOAT3(base.x + offset, base.y, base.z), // Right panel center
		};

		// 2枚のパネルに対して個別に AABB 押し戻し
		for (int p = 0; p < 2; ++p)
		{
			const XMFLOAT3 center = centers[p];

			// ---------- 側面帯（YがAABBの中）なら X/Z 押し戻し ----------
			if ((center.y - hy) < pos.y && pos.y < (center.y + hy - 0.1f))
			{
				// Z帯の中 → X面で押し戻し
				if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
				{
					// -X面
					if ((center.x - hx) < pos.x + PLAYER_RADIUS && pos.x < (center.x - hx))
					{
						pos.x += (center.x - hx) - (pos.x + PLAYER_RADIUS);
						vel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_3;
					}
					// +X面
					else if ((center.x + hx) > pos.x - PLAYER_RADIUS && pos.x > (center.x + hx))
					{
						pos.x += (center.x + hx) - (pos.x - PLAYER_RADIUS);
						vel.x *= -COE;
						hit = COLLISION_HIT::HIT_WALL_1;
					}
				}
				// X帯の中 → Z面で押し戻し
				else if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
				{
					// -Z面
					if ((center.z - hz) < pos.z + PLAYER_RADIUS && pos.z < (center.z - hz))
					{
						pos.z += (center.z - hz) - (pos.z + PLAYER_RADIUS);
						vel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_0;
					}
					// +Z面
					else if ((center.z + hz) > pos.z - PLAYER_RADIUS && pos.z > (center.z + hz))
					{
						pos.z += (center.z + hz) - (pos.z - PLAYER_RADIUS);
						vel.z *= -COE;
						hit = COLLISION_HIT::HIT_WALL_2;
					}
				}
			}
			// ---------- 上下面（Yが外）なら Y 方向で押し戻し ----------
			else
			{
				if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
				{
					if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
					{
						// -Y面（下面）
						if ((center.y - hy) < pos.y + PLAYER_RADIUS && pos.y < (center.y - hy))
						{
							pos.y += (center.y - hy) - (pos.y + PLAYER_RADIUS);
							vel.y *= -COE;
							// 地面扱いにするなら HIT_GROUND
							hit = COLLISION_HIT::HIT_GROUND;
						}
						// +Y面（上面）
						else if ((center.y + hy) > pos.y - PLAYER_RADIUS && pos.y > (center.y + hy))
						{
							pos.y += (center.y + hy) - (pos.y - PLAYER_RADIUS);
							// 上面は停止させる（反射ではなく0も可）
							vel.y = 0.0f;
							hit = COLLISION_HIT::HIT_GROUND;
							// 着地フラグは PlayerFieldCollision と同等の取り扱いにするなら必要
							// （必要なら以下を有効化）
							// if (!pPlayer->GetPlayerJump()) { pPlayer->SetPlayerJump(true); }
						}
					}
				}
			}
		}
	}

	pPlayer->SetPlayerPosition(pos);
	pPlayer->SetPlayerVelocity(vel);
	return hit;
}

float COLLISION::EnemyGimmickCollision(ENEMYSPAWNER* pEnemy, GIMMICK_DATA* pGimmick)
{

	float hit = 0.0f;

	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();

	GIMMICK_BUTTON* Buttons = pGimmick->GetButtons();
	int btnCount = pGimmick->GetButtonCount();

	GIMMICK_FIELD* Fields = pGimmick->GetFields();
	int fldCount = pGimmick->GetFieldCount();

	// ---------------- 歩行系（ENEMY_NORMAL） ----------------
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 ePos = enemy[i].GetEnemyPosition();
			XMFLOAT3 eVel = enemy[i].GetEnemyVelocity();

			// ---- Buttons（押下判定：上面）----
			for (int b = 0; b < btnCount; b++)
			{
				float BoxTop;
				XMFLOAT3 bPos = Buttons[b].GimmickButton_GetPosition();
				BoxTop = bPos.y + 0.2f;

				// XZ 内側？
				if (bPos.z - BOX_RADIUS < ePos.z && ePos.z < bPos.z + BOX_RADIUS)
				{
					if (bPos.x - BOX_RADIUS < ePos.x && ePos.x < bPos.x + BOX_RADIUS)
					{
						// 上面接地
						if (BoxTop > ePos.y - BALL_RADIUS && ePos.y > BoxTop)
						{
							ePos.y += (BoxTop)-(ePos.y - BALL_RADIUS);
							eVel.y = 0.0f;

							Buttons[b].GimmickButton_SetTouch(true);
							int chBtn = Buttons[b].GimmickButton_GetChannel();
							pGimmick->Channels_AddCount(chBtn);

							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}

			// ---- Fields（AABB 押し戻し：側面・前後・上面）----
			for (int f = 0; f < fldCount; f++)
			{
				float BoxTop;
				XMFLOAT3 fieldsPos = Fields[f].GimmickField_GetPosition();
				BoxTop = fieldsPos.y + BOX_RADIUS;

				// 側面帯（Y 中）
				if (fieldsPos.y - BOX_RADIUS < ePos.y && ePos.y < BoxTop - 0.1f)
				{
					// ±X
					if (fieldsPos.z - BOX_RADIUS < ePos.z && ePos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < ePos.x + BALL_RADIUS && ePos.x < fieldsPos.x - BOX_RADIUS)
						{
							ePos.x += (fieldsPos.x - BOX_RADIUS) - (ePos.x + BALL_RADIUS);
							eVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (fieldsPos.x + BOX_RADIUS > ePos.x - BALL_RADIUS && ePos.x > fieldsPos.x + BOX_RADIUS)
						{
							ePos.x += (fieldsPos.x + BOX_RADIUS) - (ePos.x - BALL_RADIUS);
							eVel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}

					// ±Z
					if (fieldsPos.x - BOX_RADIUS < ePos.x && ePos.x < fieldsPos.x + BOX_RADIUS)
					{
						if (fieldsPos.z - BOX_RADIUS < ePos.z + BALL_RADIUS && ePos.z < fieldsPos.z - BOX_RADIUS)
						{
							ePos.z += (fieldsPos.z - BOX_RADIUS) - (ePos.z + BALL_RADIUS);
							eVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (fieldsPos.z + BOX_RADIUS > ePos.z - BALL_RADIUS && ePos.z > fieldsPos.z + BOX_RADIUS)
						{
							ePos.z += (fieldsPos.z + BOX_RADIUS) - (ePos.z - BALL_RADIUS);
							eVel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				// 上面（+Y）
				else
				{
					if (fieldsPos.z - BOX_RADIUS < ePos.z && ePos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < ePos.x && ePos.x < fieldsPos.x + BOX_RADIUS)
						{
							if (fieldsPos.y - BOX_RADIUS < ePos.y + BALL_RADIUS && ePos.y < fieldsPos.y - BOX_RADIUS)
							{
								ePos.y += (fieldsPos.y - BOX_RADIUS) - (ePos.y + BALL_RADIUS);
								eVel.y *= -COE;
								hit = COLLISION_HIT::HIT_GROUND;
							}
							else if (BoxTop > ePos.y - BALL_RADIUS && ePos.y > BoxTop)
							{
								ePos.y += (BoxTop)-(ePos.y - BALL_RADIUS);
								eVel.y = 0.0f;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}
			}

			enemy[i].SetEnemyPosition(ePos);
			enemy[i].SetEnemyVelocity(eVel);
		}
	}

	// ---------------- 浮遊系（ENEMY_BUTTERFLY） ----------------
	// 浮遊体は「押下のみ」扱い（可動面の押し戻しは不要なら省略）
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemyB[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 ePos = enemyB[i].GetEnemyPosition();
			XMFLOAT3 eVel = enemyB[i].GetEnemyVelocity();

			// Buttons：XZ内＋Y帯交差で押下扱い
			for (int b = 0; b < btnCount; b++)
			{
				float BoxTop;
				XMFLOAT3 bPos = Buttons[b].GimmickButton_GetPosition();
				BoxTop = bPos.y + 0.2f;

				if (bPos.z - BOX_RADIUS < ePos.z && ePos.z < bPos.z + BOX_RADIUS)
				{
					if (bPos.x - BOX_RADIUS < ePos.x && ePos.x < bPos.x + BOX_RADIUS)
					{
						if (bPos.y - BOX_RADIUS < ePos.y + BALL_RADIUS && ePos.y - BALL_RADIUS < BoxTop)
						{
							Buttons[b].GimmickButton_SetTouch(true);
							int chBtn = Buttons[b].GimmickButton_GetChannel();
							pGimmick->Channels_AddCount(chBtn);
							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}

			enemyB[i].SetEnemyPosition(ePos);
			enemyB[i].SetEnemyVelocity(eVel);
		}
	}

	return hit;

}

float COLLISION::EnemyMovingFieldCollision(ENEMYSPAWNER* pEnemy, GIMMICK_DATA* pGimmick)
{
	ENEMY_NORMAL* enemy = pEnemy->EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* enemyB = pEnemy->EnemySpawner_GetEnemyButterfly();

	// ---------------- 歩行系（ENEMY_NORMAL） ----------------
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 epos = enemy[i].GetEnemyPosition();

			// 押下中ボタンのY追従（XZ重なり時に vel.y を乗せる）
			GIMMICK_BUTTON* buttons = pGimmick->GetButtons();
			for (int b = 0; b < pGimmick->GetButtonCount(); b++)
			{
				if (buttons[b].GimmickButton_GetTouch())
				{
					XMFLOAT3 bpos = buttons[b].GimmickButton_GetPosition();
					if (bpos.z - BOX_RADIUS < epos.z && epos.z < bpos.z + BOX_RADIUS)
					{
						if (bpos.x - BOX_RADIUS < epos.x && epos.x < bpos.x + BOX_RADIUS)
						{
							XMFLOAT3 vel = buttons[b].GimmickButton_GetVelocity();
							epos.y += vel.y;
						}
					}
				}
			}

			// 可動フィールド追従（XZ重なり ＋ 天面近傍なら XYZ すべてを加算）
			GIMMICK_FIELD* fields = pGimmick->GetFields();
			int fcnt = pGimmick->GetFieldCount();
			for (int f = 0; f < fcnt; f++)
			{
				XMFLOAT3 fpos = fields[f].GimmickField_GetPosition();
				if (fpos.z - BOX_RADIUS < epos.z && epos.z < fpos.z + BOX_RADIUS)
				{
					if (fpos.x - BOX_RADIUS < epos.x && epos.x < fpos.x + BOX_RADIUS)
					{
						float top = fpos.y + BOX_RADIUS;
						if (top + GROUND_SLOP > epos.y - BALL_RADIUS)
						{
							epos.x += fields[f].m_Velocity.x;
							epos.y += fields[f].m_Velocity.y;
							epos.z += fields[f].m_Velocity.z;
						}
					}
				}
			}

			enemy[i].SetEnemyPosition(epos);
		}
	}


	// ---------------- 浮遊系（ENEMY_BUTTERFLY） ----------------
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (enemyB[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			XMFLOAT3 epos = enemyB[i].GetEnemyPosition();

			// 押下中ボタンのY追従（XZ重なり）
			GIMMICK_BUTTON* buttons = pGimmick->GetButtons();
			for (int b = 0; b < pGimmick->GetButtonCount(); b++)
			{
				if (buttons[b].GimmickButton_GetTouch())
				{
					XMFLOAT3 bpos = buttons[b].GimmickButton_GetPosition();
					if (bpos.z - BOX_RADIUS < epos.z && epos.z < bpos.z + BOX_RADIUS)
					{
						if (bpos.x - BOX_RADIUS < epos.x && epos.x < bpos.x + BOX_RADIUS)
						{
							XMFLOAT3 bvel = buttons[b].GimmickButton_GetVelocity();
							epos.y += bvel.y;
						}
					}
				}
			}

			// 可動フィールド追従（XZ重なり＋天面近傍）
			GIMMICK_FIELD* fields = pGimmick->GetFields();
			int fcnt = pGimmick->GetFieldCount();
			for (int f = 0; f < fcnt; f++)
			{
				XMFLOAT3 fpos = fields[f].GimmickField_GetPosition();
				if (fpos.z - BOX_RADIUS < epos.z && epos.z < fpos.z + BOX_RADIUS)
				{
					if (fpos.x - BOX_RADIUS < epos.x && epos.x < fpos.x + BOX_RADIUS)
					{
						float top = fpos.y + BOX_RADIUS;
						if (top + GROUND_SLOP > epos.y - BALL_RADIUS)
						{
							epos.x += fields[f].m_Velocity.x;
							epos.y += fields[f].m_Velocity.y;
							epos.z += fields[f].m_Velocity.z;
						}
					}
				}
			}

			enemyB[i].SetEnemyPosition(epos);
		}
	}

	return 0.0f;

}

float COLLISION::BombMovingFieldCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick)
{

	BOMBSOURCE* Bomb = pBomb->Bomb_GetBomb();	// マップ
	RUNBOMBSPAWNER* RunBombSpawner = pBomb->Bomb_GetRunBomb();// マップ
	FLOWTBOMBSOURCE* FlowtBomb = pBomb->Bomb_GetFlowtBomb();// マップ

	// --------- 共通：ボタンとフィールド配列 ---------
	GIMMICK_BUTTON* buttons = pGimmick->GetButtons();
	int btnCount = pGimmick->GetButtonCount();
	GIMMICK_FIELD* fields = pGimmick->GetFields();
	int fldCount = pGimmick->GetFieldCount();


	// --------- 通常ボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int bt = Bomb[i].BombSource_GetState();
		if (bt == BOMB_STATE::BOMB_ITEM ||
			bt == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			bt == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = Bomb[i].BombSource_GetPosition();

			// ボタンY追従（押下中かつXZ重なり）
			for (int b = 0; b < btnCount; b++)
			{
				if (buttons[b].GimmickButton_GetTouch())
				{
					XMFLOAT3 bpos = buttons[b].GimmickButton_GetPosition();
					if (bpos.z - BOX_RADIUS < pos.z && pos.z < bpos.z + BOX_RADIUS)
					{
						if (bpos.x - BOX_RADIUS < pos.x && pos.x < bpos.x + BOX_RADIUS)
						{
							XMFLOAT3 bvel = buttons[b].GimmickButton_GetVelocity();
							pos.y += bvel.y;
						}
					}
				}
			}

			// 可動フィールド追従（XZ重なり＋天面近傍）
			for (int f = 0; f < fldCount; f++)
			{
				XMFLOAT3 fpos = fields[f].GimmickField_GetPosition();
				if (fpos.z - BOX_RADIUS < pos.z && pos.z < fpos.z + BOX_RADIUS)
				{
					if (fpos.x - BOX_RADIUS < pos.x && pos.x < fpos.x + BOX_RADIUS)
					{
						float top = fpos.y + BOX_RADIUS;
						if (top + GROUND_SLOP > pos.y - BOMB_RADIUS)
						{
							pos.x += fields[f].m_Velocity.x;
							pos.y += fields[f].m_Velocity.y;
							pos.z += fields[f].m_Velocity.z;
						}
					}
				}
			}

			Bomb[i].BombSource_SetPosition(pos);
		}
	}
	// --------- 走るボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		int st = RunBomb->Runbombsource_GetState();
		if (st == RUNBOMB_STATE::RUNBOMB_ITEM ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_HAVE ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW ||
			st == RUNBOMB_STATE::RUNBOMB_ENEMY)
		{
			XMFLOAT3 pos = RunBomb->Runbombsource_GetPosition();

			for (int b = 0; b < btnCount; b++)
			{
				if (buttons[b].GimmickButton_GetTouch())
				{
					XMFLOAT3 bpos = buttons[b].GimmickButton_GetPosition();
					if (bpos.z - BOX_RADIUS < pos.z && pos.z < bpos.z + BOX_RADIUS)
					{
						if (bpos.x - BOX_RADIUS < pos.x && pos.x < bpos.x + BOX_RADIUS)
						{
							XMFLOAT3 bvel = buttons[b].GimmickButton_GetVelocity();
							pos.y += bvel.y;
						}
					}
				}
			}

			for (int f = 0; f < fldCount; f++)
			{
				XMFLOAT3 fpos = fields[f].GimmickField_GetPosition();
				if (fpos.z - BOX_RADIUS < pos.z && pos.z < fpos.z + BOX_RADIUS)
				{
					if (fpos.x - BOX_RADIUS < pos.x && pos.x < fpos.x + BOX_RADIUS)
					{
						float top = fpos.y + BOX_RADIUS;
						if (top + GROUND_SLOP > pos.y - BOMB_RADIUS)
						{
							pos.x += fields[f].m_Velocity.x;
							pos.y += fields[f].m_Velocity.y;
							pos.z += fields[f].m_Velocity.z;
						}
					}
				}
			}

			RunBomb->Runbombsource_SetPosition(pos);
		}
	}
	// --------- 浮遊ボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int st = FlowtBomb[i].Flowtbombsource_GetState();
		if (st == BOMB_STATE::BOMB_ITEM ||
			st == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			st == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = FlowtBomb[i].Flowtbombsource_GetPosition();

			for (int b = 0; b < btnCount; b++)
			{
				if (buttons[b].GimmickButton_GetTouch())
				{
					XMFLOAT3 bpos = buttons[b].GimmickButton_GetPosition();
					if (bpos.z - BOX_RADIUS < pos.z && pos.z < bpos.z + BOX_RADIUS)
					{
						if (bpos.x - BOX_RADIUS < pos.x && pos.x < bpos.x + BOX_RADIUS)
						{
							XMFLOAT3 bvel = buttons[b].GimmickButton_GetVelocity();
							pos.y += bvel.y;
						}
					}
				}
			}

			for (int f = 0; f < fldCount; f++)
			{
				XMFLOAT3 fpos = fields[f].GimmickField_GetPosition();
				if (fpos.z - BOX_RADIUS < pos.z && pos.z < fpos.z + BOX_RADIUS)
				{
					if (fpos.x - BOX_RADIUS < pos.x && pos.x < fpos.x + BOX_RADIUS)
					{
						float top = fpos.y + BOX_RADIUS;
						if (top + GROUND_SLOP > pos.y - BOMB_RADIUS)
						{
							pos.x += fields[f].m_Velocity.x;
							pos.y += fields[f].m_Velocity.y;
							pos.z += fields[f].m_Velocity.z;
						}
					}
				}
			}

			FlowtBomb[i].Flowtbombsource_SetPosition(pos);
		}
	}

	return 0.0f;

}


float COLLISION::BombGimmickCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick)
{
	float hit = 0.0f;
	const float BUTTON_RADIUS = 2.0f;			// ボタンの当たり判定の半径

	BOMBSOURCE* Bomb = pBomb->Bomb_GetBomb();	// マップ
	RUNBOMBSPAWNER* RunBombSpawner = pBomb->Bomb_GetRunBomb();// マップ
	FLOWTBOMBSOURCE* FlowtBomb = pBomb->Bomb_GetFlowtBomb();// マップ

	GIMMICK_BUTTON* Buttons = pGimmick->GetButtons();
	int btnCount = pGimmick->GetButtonCount();
	GIMMICK_FIELD* Fields = pGimmick->GetFields();
	int fldCount = pGimmick->GetFieldCount();

	// --------- 通常ボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int st = Bomb[i].BombSource_GetState();
		if (st == BOMB_STATE::BOMB_ITEM ||
			st == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			st == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = Bomb[i].BombSource_GetPosition();
			XMFLOAT3 vel = Bomb[i].BombSource_GetVelocity();

			// --- Buttons：押下（上面） ---
			for (int b = 0; b < btnCount; b++)
			{
				float BoxTop;
				XMFLOAT3 bpos = Buttons[b].GimmickButton_GetPosition();
				BoxTop = bpos.y + 0.2f;


				// XZ 内側？
				if (bpos.z - BUTTON_RADIUS < pos.z && pos.z < bpos.z + BUTTON_RADIUS)
				{
					if (bpos.x - BUTTON_RADIUS < pos.x && pos.x < bpos.x + BUTTON_RADIUS)
					{
						// 着地（交差）判定
						bool landing = false;
						if (BoxTop > pos.y - BOMB_RADIUS)
						{
							if (pos.y > BoxTop)
							{
								landing = true;
							}
						}

						// 静止（底面が上面に±スロープで一致）判定
						bool resting = false;
						float diff = (pos.y - BOMB_RADIUS) - BoxTop; // 底面 - 上面
						if (diff < GROUND_SLOP)
						{
							if (diff > -GROUND_SLOP)
							{
								resting = true;
							}
						}

						if (landing || resting)
						{
							// 押し戻しと滑り止め（着地時／静止時ともに安定させる）
							pos.y = BoxTop + BOMB_RADIUS;
							vel.y = 0.0f;
							vel.x = 0.0f;
							vel.z = 0.0f;

							// 押下
							Buttons[b].GimmickButton_SetTouch(true);
							int ch = Buttons[b].GimmickButton_GetChannel();
							pGimmick->Channels_AddCount(ch);

							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}

			}


			// --- Fields：AABB 押し戻し ---
			for (int f = 0; f < fldCount; f++)
			{
				float BoxTop;
				XMFLOAT3 fieldsPos = Fields[f].GimmickField_GetPosition();
				BoxTop = fieldsPos.y + BOX_RADIUS;

				// 側面帯
				if (fieldsPos.y - BOX_RADIUS < pos.y && pos.y < BoxTop - 0.1f)
				{
					// ±X
					if (fieldsPos.z - BOX_RADIUS < pos.z && pos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < pos.x + BOMB_RADIUS && pos.x < fieldsPos.x - BOX_RADIUS)
						{
							pos.x += (fieldsPos.x - BOX_RADIUS) - (pos.x + BOMB_RADIUS);
							vel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (fieldsPos.x + BOX_RADIUS > pos.x - BOMB_RADIUS && pos.x > fieldsPos.x + BOX_RADIUS)
						{
							pos.x += (fieldsPos.x + BOX_RADIUS) - (pos.x - BOMB_RADIUS);
							vel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					// ±Z
					if (fieldsPos.x - BOX_RADIUS < pos.x && pos.x < fieldsPos.x + BOX_RADIUS)
					{
						if (fieldsPos.z - BOX_RADIUS < pos.z + BOMB_RADIUS && pos.z < fieldsPos.z - BOX_RADIUS)
						{
							pos.z += (fieldsPos.z - BOX_RADIUS) - (pos.z + BOMB_RADIUS);
							vel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (fieldsPos.z + BOX_RADIUS > pos.z - BOMB_RADIUS && pos.z > fieldsPos.z + BOX_RADIUS)
						{
							pos.z += (fieldsPos.z + BOX_RADIUS) - (pos.z - BOMB_RADIUS);
							vel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				// 上面
				else
				{
					if (fieldsPos.z - BOX_RADIUS < pos.z && pos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < pos.x && pos.x < fieldsPos.x + BOX_RADIUS)
						{
							if (fieldsPos.y - BOX_RADIUS < pos.y + BOMB_RADIUS && pos.y < fieldsPos.y - BOX_RADIUS)
							{
								pos.y += (fieldsPos.y - BOX_RADIUS) - (pos.y + BOMB_RADIUS);
								vel.y *= -COE;
								hit = COLLISION_HIT::HIT_GROUND;
							}
							else if (BoxTop > pos.y - BOMB_RADIUS && pos.y > BoxTop)
							{
								pos.y += (BoxTop)-(pos.y - BOMB_RADIUS);
								vel.y = 0.0f;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}
			}

			Bomb[i].BombSource_SetPosition(pos);
			Bomb[i].BombSource_SetVelocity(vel);
		}
	}

	// --------- 走るボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		RUNBOMBSOURCE* RunBomb = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		int st = RunBomb->Runbombsource_GetState();
		if (st == RUNBOMB_STATE::RUNBOMB_ITEM ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_HAVE ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = RunBomb->Runbombsource_GetPosition();
			XMFLOAT3 vel = RunBomb->Runbombsource_GetVelocity();

			// Buttons
			for (int b = 0; b < btnCount; b++)
			{
				float BoxTop;
				XMFLOAT3 bpos = Buttons[b].GimmickButton_GetPosition();
				BoxTop = bpos.y + 0.2f;

				if (bpos.z - BOX_RADIUS < pos.z && pos.z < bpos.z + BOX_RADIUS)
				{
					if (bpos.x - BOX_RADIUS < pos.x && pos.x < bpos.x + BOX_RADIUS)
					{
						if (BoxTop > pos.y - BOMB_RADIUS && pos.y > BoxTop)
						{
							pos.y += (BoxTop)-(pos.y - BOMB_RADIUS);
							vel.y = 0.0f;
							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}
			// Fields
			for (int f = 0; f < fldCount; f++)
			{
				float BoxTop;
				XMFLOAT3 fieldsPos = Fields[f].GimmickField_GetPosition();
				BoxTop = fieldsPos.y + BOX_RADIUS;

				if (fieldsPos.y - BOX_RADIUS < pos.y && pos.y < BoxTop - 0.1f)
				{
					if (fieldsPos.z - BOX_RADIUS < pos.z && pos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < pos.x + BOMB_RADIUS && pos.x < fieldsPos.x - BOX_RADIUS)
						{
							pos.x += (fieldsPos.x - BOX_RADIUS) - (pos.x + BOMB_RADIUS);
							vel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_3;
						}
						else if (fieldsPos.x + BOX_RADIUS > pos.x - BOMB_RADIUS && pos.x > fieldsPos.x + BOX_RADIUS)
						{
							pos.x += (fieldsPos.x + BOX_RADIUS) - (pos.x - BOMB_RADIUS);
							vel.x *= -COE;
							hit = COLLISION_HIT::HIT_WALL_1;
						}
					}
					if (fieldsPos.x - BOX_RADIUS < pos.x && pos.x < fieldsPos.x + BOX_RADIUS)
					{
						if (fieldsPos.z - BOX_RADIUS < pos.z + BOMB_RADIUS && pos.z < fieldsPos.z - BOX_RADIUS)
						{
							pos.z += (fieldsPos.z - BOX_RADIUS) - (pos.z + BOMB_RADIUS);
							vel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_0;
						}
						else if (fieldsPos.z + BOX_RADIUS > pos.z - BOMB_RADIUS && pos.z > fieldsPos.z + BOX_RADIUS)
						{
							pos.z += (fieldsPos.z + BOX_RADIUS) - (pos.z - BOMB_RADIUS);
							vel.z *= -COE;
							hit = COLLISION_HIT::HIT_WALL_2;
						}
					}
				}
				else
				{
					if (fieldsPos.z - BOX_RADIUS < pos.z && pos.z < fieldsPos.z + BOX_RADIUS)
					{
						if (fieldsPos.x - BOX_RADIUS < pos.x && pos.x < fieldsPos.x + BOX_RADIUS)
						{
							if (fieldsPos.y - BOX_RADIUS < pos.y + BOMB_RADIUS && pos.y < fieldsPos.y - BOX_RADIUS)
							{
								pos.y += (fieldsPos.y - BOX_RADIUS) - (pos.y + BOMB_RADIUS);
								vel.y *= -COE;
								hit = COLLISION_HIT::HIT_GROUND;
							}
							else if (BoxTop > pos.y - BOMB_RADIUS && pos.y > BoxTop)
							{
								pos.y += (BoxTop)-(pos.y - BOMB_RADIUS);
								vel.y = 0.0f;
								hit = COLLISION_HIT::HIT_GROUND;
							}
						}
					}
				}
			}

			RunBomb->Runbombsource_SetPosition(pos);
			RunBomb->Runbombsource_SetVelocity(vel);
		}
	}

	// --------- 浮遊ボム ---------
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int st = FlowtBomb[i].Flowtbombsource_GetState();
		if (st == BOMB_STATE::BOMB_ITEM ||
			st == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			st == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = FlowtBomb[i].Flowtbombsource_GetPosition();
			XMFLOAT3 vel = FlowtBomb[i].Flowtbombsource_GetVelocity();

			// Buttons：XZ+Y帯で押下扱い（浮遊でもONにできる仕様）
			for (int b = 0; b < btnCount; b++)
			{
				float BoxTop;
				XMFLOAT3 bpos = Buttons[b].GimmickButton_GetPosition();
				BoxTop = bpos.y + 0.2f;

				if (bpos.z - BOX_RADIUS < pos.z && pos.z < bpos.z + BOX_RADIUS)
				{
					if (bpos.x - BOX_RADIUS < pos.x && pos.x < bpos.x + BOX_RADIUS)
					{
						if (bpos.y - BOX_RADIUS < pos.y + BOMB_RADIUS && pos.y - BOMB_RADIUS < BoxTop)
						{
							hit = COLLISION_HIT::HIT_GROUND;
						}
					}
				}
			}

			// 浮遊ボム：必要ならField押し戻しも追加可能（現状は省略 or 上面のみ）
			FlowtBomb[i].Flowtbombsource_SetPosition(pos);
			FlowtBomb[i].Flowtbombsource_SetVelocity(vel);
		}
	}

	return hit;

}

float COLLISION::BombGateCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick)
{

	float hit = 0.0f;

	BOMBSOURCE* Bomb = pBomb->Bomb_GetBomb();	// マップ
	RUNBOMBSPAWNER* RunBombs = pBomb->Bomb_GetRunBomb();// マップ
	FLOWTBOMBSOURCE* FlowtBomb = pBomb->Bomb_GetFlowtBomb();// マップ

	GIMMICK_GATE* gates = pGimmick->GetGates();
	int gateCount = pGimmick->GetGateCount();

	// AABB 半径（ハーフサイズ）
	const float hx = GATE_PANEL_HALF_W; // X半幅（＝厚み方向）
	const float hy = GATE_PANEL_HALF_H; // Y半幅（高さ）
	const float hz = GATE_PANEL_HALF_D; // Z半幅（奥行）  ※描画は 1x1x1

	// 通常 =====================================================================
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int st = Bomb[i].BombSource_GetState();
		if (st == BOMB_STATE::BOMB_ITEM ||
			st == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			st == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = Bomb[i].BombSource_GetPosition();
			XMFLOAT3 vel = Bomb[i].BombSource_GetVelocity();
			
			for (int i = 0; i < gateCount; ++i)
			{
				// ゲート基準と開度
				XMFLOAT3 base = gates[i].GimmickGate_GetPosition();
				float open = gates[i].GimmickGate_GetOpen();
				float offset = GATE_MAX_OPEN_OFFSET * open;

				// 描画と同じ「中心：base.x ± offset」
				XMFLOAT3 centers[2] = {
					XMFLOAT3(base.x - offset, base.y, base.z), // Left panel center
					XMFLOAT3(base.x + offset, base.y, base.z), // Right panel center
				};

				// 2枚のパネルに対して個別に AABB 押し戻し
				for (int p = 0; p < 2; ++p)
				{
					const XMFLOAT3 center = centers[p];

					// ---------- 側面帯（YがAABBの中）なら X/Z 押し戻し ----------
					if ((center.y - hy) < pos.y && pos.y < (center.y + hy - 0.1f))
					{
						// Z帯の中 → X面で押し戻し
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							// -X面
							if ((center.x - hx) < pos.x + PLAYER_RADIUS && pos.x < (center.x - hx))
							{
								pos.x += (center.x - hx) - (pos.x + PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_3;
							}
							// +X面
							else if ((center.x + hx) > pos.x - PLAYER_RADIUS && pos.x > (center.x + hx))
							{
								pos.x += (center.x + hx) - (pos.x - PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_1;
							}
						}
						// X帯の中 → Z面で押し戻し
						else if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
						{
							// -Z面
							if ((center.z - hz) < pos.z + PLAYER_RADIUS && pos.z < (center.z - hz))
							{
								pos.z += (center.z - hz) - (pos.z + PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_0;
							}
							// +Z面
							else if ((center.z + hz) > pos.z - PLAYER_RADIUS && pos.z > (center.z + hz))
							{
								pos.z += (center.z + hz) - (pos.z - PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_2;
							}
						}
					}
					// ---------- 上下面（Yが外）なら Y 方向で押し戻し ----------
					else
					{
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
							{
								// -Y面（下面）
								if ((center.y - hy) < pos.y + PLAYER_RADIUS && pos.y < (center.y - hy))
								{
									pos.y += (center.y - hy) - (pos.y + PLAYER_RADIUS);
									vel.y *= -COE;
									// 地面扱いにするなら HIT_GROUND
									hit = COLLISION_HIT::HIT_GROUND;
								}
								// +Y面（上面）
								else if ((center.y + hy) > pos.y - PLAYER_RADIUS && pos.y > (center.y + hy))
								{
									pos.y += (center.y + hy) - (pos.y - PLAYER_RADIUS);
									// 上面は停止させる（反射ではなく0も可）
									vel.y = 0.0f;
									hit = COLLISION_HIT::HIT_GROUND;
									// 着地フラグは PlayerFieldCollision と同等の取り扱いにするなら必要
									// （必要なら以下を有効化）
									// if (!pPlayer->GetPlayerJump()) { pPlayer->SetPlayerJump(true); }
								}
							}
						}
					}
				}
			}

			Bomb[i].BombSource_SetPosition(pos);
			Bomb[i].BombSource_SetVelocity(vel);
		}
	}
	
	// 走る ==================================================================
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		RUNBOMBSOURCE* RunBomb = RunBombs[i].GetRunBombSource__RunBombSpawner();

		int st = RunBomb->Runbombsource_GetState();
		if (st == RUNBOMB_STATE::RUNBOMB_ITEM ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_HAVE ||
			st == RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = RunBomb->Runbombsource_GetPosition();
			XMFLOAT3 vel = RunBomb->Runbombsource_GetVelocity();
			
			for (int i = 0; i < gateCount; ++i)
			{
				// ゲート基準と開度
				XMFLOAT3 base = gates[i].GimmickGate_GetPosition();
				float open = gates[i].GimmickGate_GetOpen();
				float offset = GATE_MAX_OPEN_OFFSET * open;

				// 描画と同じ「中心：base.x ± offset」
				XMFLOAT3 centers[2] = {
					XMFLOAT3(base.x - offset, base.y, base.z), // Left panel center
					XMFLOAT3(base.x + offset, base.y, base.z), // Right panel center
				};

				// 2枚のパネルに対して個別に AABB 押し戻し
				for (int p = 0; p < 2; ++p)
				{
					const XMFLOAT3 center = centers[p];

					// ---------- 側面帯（YがAABBの中）なら X/Z 押し戻し ----------
					if ((center.y - hy) < pos.y && pos.y < (center.y + hy - 0.1f))
					{
						// Z帯の中 → X面で押し戻し
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							// -X面
							if ((center.x - hx) < pos.x + PLAYER_RADIUS && pos.x < (center.x - hx))
							{
								pos.x += (center.x - hx) - (pos.x + PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_3;
							}
							// +X面
							else if ((center.x + hx) > pos.x - PLAYER_RADIUS && pos.x > (center.x + hx))
							{
								pos.x += (center.x + hx) - (pos.x - PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_1;
							}
						}
						// X帯の中 → Z面で押し戻し
						else if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
						{
							// -Z面
							if ((center.z - hz) < pos.z + PLAYER_RADIUS && pos.z < (center.z - hz))
							{
								pos.z += (center.z - hz) - (pos.z + PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_0;
							}
							// +Z面
							else if ((center.z + hz) > pos.z - PLAYER_RADIUS && pos.z > (center.z + hz))
							{
								pos.z += (center.z + hz) - (pos.z - PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_2;
							}
						}
					}
					// ---------- 上下面（Yが外）なら Y 方向で押し戻し ----------
					else
					{
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
							{
								// -Y面（下面）
								if ((center.y - hy) < pos.y + PLAYER_RADIUS && pos.y < (center.y - hy))
								{
									pos.y += (center.y - hy) - (pos.y + PLAYER_RADIUS);
									vel.y *= -COE;
									// 地面扱いにするなら HIT_GROUND
									hit = COLLISION_HIT::HIT_GROUND;
								}
								// +Y面（上面）
								else if ((center.y + hy) > pos.y - PLAYER_RADIUS && pos.y > (center.y + hy))
								{
									pos.y += (center.y + hy) - (pos.y - PLAYER_RADIUS);
									// 上面は停止させる（反射ではなく0も可）
									vel.y = 0.0f;
									hit = COLLISION_HIT::HIT_GROUND;
									// 着地フラグは PlayerFieldCollision と同等の取り扱いにするなら必要
									// （必要なら以下を有効化）
									// if (!pPlayer->GetPlayerJump()) { pPlayer->SetPlayerJump(true); }
								}
							}
						}
					}
				}
			}

			RunBomb->Runbombsource_SetPosition(pos);
			RunBomb->Runbombsource_SetVelocity(vel);
		}
	}

	// 浮遊
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int st = FlowtBomb[i].Flowtbombsource_GetState();
		if (st == BOMB_STATE::BOMB_ITEM ||
			st == BOMB_STATE::BOMB_ACTIVE_HAVE ||
			st == BOMB_STATE::BOMB_ACTIVE_THROW)
		{
			XMFLOAT3 pos = FlowtBomb[i].Flowtbombsource_GetPosition();
			XMFLOAT3 vel = FlowtBomb[i].Flowtbombsource_GetVelocity();

			for (int i = 0; i < gateCount; ++i)
			{
				// ゲート基準と開度
				XMFLOAT3 base = gates[i].GimmickGate_GetPosition();
				float open = gates[i].GimmickGate_GetOpen();
				float offset = GATE_MAX_OPEN_OFFSET * open;

				// 描画と同じ「中心：base.x ± offset」
				XMFLOAT3 centers[2] = {
					XMFLOAT3(base.x - offset, base.y, base.z), // Left panel center
					XMFLOAT3(base.x + offset, base.y, base.z), // Right panel center
				};

				// 2枚のパネルに対して個別に AABB 押し戻し
				for (int p = 0; p < 2; ++p)
				{
					const XMFLOAT3 center = centers[p];

					// ---------- 側面帯（YがAABBの中）なら X/Z 押し戻し ----------
					if ((center.y - hy) < pos.y && pos.y < (center.y + hy - 0.1f))
					{
						// Z帯の中 → X面で押し戻し
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							// -X面
							if ((center.x - hx) < pos.x + PLAYER_RADIUS && pos.x < (center.x - hx))
							{
								pos.x += (center.x - hx) - (pos.x + PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_3;
							}
							// +X面
							else if ((center.x + hx) > pos.x - PLAYER_RADIUS && pos.x > (center.x + hx))
							{
								pos.x += (center.x + hx) - (pos.x - PLAYER_RADIUS);
								vel.x *= -COE;
								hit = COLLISION_HIT::HIT_WALL_1;
							}
						}
						// X帯の中 → Z面で押し戻し
						else if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
						{
							// -Z面
							if ((center.z - hz) < pos.z + PLAYER_RADIUS && pos.z < (center.z - hz))
							{
								pos.z += (center.z - hz) - (pos.z + PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_0;
							}
							// +Z面
							else if ((center.z + hz) > pos.z - PLAYER_RADIUS && pos.z > (center.z + hz))
							{
								pos.z += (center.z + hz) - (pos.z - PLAYER_RADIUS);
								vel.z *= -COE;
								hit = COLLISION_HIT::HIT_WALL_2;
							}
						}
					}
					// ---------- 上下面（Yが外）なら Y 方向で押し戻し ----------
					else
					{
						if ((center.z - hz) < pos.z && pos.z < (center.z + hz))
						{
							if ((center.x - hx) < pos.x && pos.x < (center.x + hx))
							{
								// -Y面（下面）
								if ((center.y - hy) < pos.y + PLAYER_RADIUS && pos.y < (center.y - hy))
								{
									pos.y += (center.y - hy) - (pos.y + PLAYER_RADIUS);
									vel.y *= -COE;
									// 地面扱いにするなら HIT_GROUND
									hit = COLLISION_HIT::HIT_GROUND;
								}
								// +Y面（上面）
								else if ((center.y + hy) > pos.y - PLAYER_RADIUS && pos.y > (center.y + hy))
								{
									pos.y += (center.y + hy) - (pos.y - PLAYER_RADIUS);
									// 上面は停止させる（反射ではなく0も可）
									vel.y = 0.0f;
									hit = COLLISION_HIT::HIT_GROUND;
									// 着地フラグは PlayerFieldCollision と同等の取り扱いにするなら必要
									// （必要なら以下を有効化）
									// if (!pPlayer->GetPlayerJump()) { pPlayer->SetPlayerJump(true); }
								}
							}
						}
					}
				}
			}

			FlowtBomb[i].Flowtbombsource_SetPosition(pos);
			FlowtBomb[i].Flowtbombsource_SetVelocity(vel);
		}

	}

	return hit;
}

float COLLISION::PlayerGoalCollision(PLAYER* pPlayer, GOAL* pGoal)
{
	if (pPlayer == nullptr || pGoal == nullptr) return COLLISION_HIT::HIT_NONE;

	GOAL::GoalItem* items = pGoal->GetGoalItems();
	int count = pGoal->GetGoalCount();
	if (items == nullptr || count <= 0) return COLLISION_HIT::HIT_NONE;

	XMFLOAT3 playerPos = pPlayer->GetPlayerPosition();

	// 判定半径（プレイヤー半径 + ゴールの半径）
	const float GOAL_RADIUS = 0.6f;
	const float radius = (PLAYER_RADIUS + GOAL_RADIUS);
	const float radiusSq = radius * radius;

	for (int i = 0; i < count; ++i)
	{
		if (!items[i].active) continue;

		float dx = playerPos.x - items[i].pos.x;
		float dy = playerPos.y - items[i].pos.y;
		float dz = playerPos.z - items[i].pos.z;

		float distSq = dx * dx + dy * dy + dz * dz;
		if (distSq <= radiusSq)
		{
			// ゴール到達を表す値を返す（既存コードで使用されている値に合わせる）
			return COLLISION_HIT::HIT_WALL_CREAR;
		}
	}

	return COLLISION_HIT::HIT_NONE;
}
