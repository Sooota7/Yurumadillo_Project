
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

					if (Map[i].MapData_GetNo() == FIELD_OBT_1)
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

					if (Map[i].MapData_GetNo() == FIELD_OBT_1)
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

					if (Map[i].MapData_GetNo() == FIELD_OBT_1)
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

					if (Map[i].MapData_GetNo() == FIELD_OBT_1)
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
						if (Map[i].MapData_GetNo() == FIELD_OBT_1)
						{
							hit = COLLISION_HIT::HIT_WALL_CREAR;
						}
					}
					else if (BoxTop > PlayerPos.y - PLAYER_RADIUS &&
						PlayerPos.y > BoxTop)
					{//BOXの+X面にぶつかった
						PlayerPos.y += (BoxTop)-(PlayerPos.y - PLAYER_RADIUS);
						PlayerVel.y = 0;//PlayerVel.y * (-COE * 1.0f);
						hit = COLLISION_HIT::HIT_GROUND;
						if (PlayerJump == false)
						{
							PlayerJump = true;
						}

						if (Map[i].MapData_GetNo() == FIELD_OBT_1)
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

float	COLLISION::EnemyFieldCollision(ENEMY* pEnemy, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 EnemyPos = pEnemy->GetEnemyPosition();
	XMFLOAT3 EnemyVel = pEnemy->GetEnemyVelocity();
	MAP* Map = pField->GetFieldMap();	// マップ
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


		pEnemy->SetEnemyPosition(EnemyPos);
		pEnemy->SetEnemyVelocity(EnemyVel);

		i++;
	}

	return hit;
}


float	COLLISION::PlayerEnemyCollision(PLAYER* pPlayer, ENEMY* pEnemy)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	XMFLOAT3 EnemyPos = pEnemy->GetEnemyPosition();
	XMFLOAT3 EnemyVel = pEnemy->GetEnemyVelocity();

	
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

float	COLLISION::PlayerBombCollision(PLAYER* pPlayer, BOMB* pBomb)
{
	float		hit = false;				// ヒットした方向
	
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	XMFLOAT3 PlayerPos = pPlayer->GetPlayerPosition();
	XMFLOAT3 PlayerVel = pPlayer->GetPlayerVelocity();
	bool PlayerJump = pPlayer->GetPlayerJump();

	BOMBSOURCE* Bomb = pBomb->Bomb_GetBomb();	// マップ
	//int			i = 0;


	// 全てのブロックをチェック
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

		XMFLOAT3 bombPos = Bomb[i].BombSource_GetPosition();


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

		Bomb[i].BombSource_SetTouch(touch);
		pPlayer->SetPlayerJump(PlayerJump);
		pPlayer->SetPlayerPosition(PlayerPos);
		pPlayer->SetPlayerVelocity(PlayerVel);

	}


	return hit;  // ぶつかったかどうかを示す
}


float	COLLISION::BombFieldCollision(BOMB* pBomb, MAPDATA* pField)
{
	float		hit = 0.0f;				// ヒットした方向
	//PLAYER*		Player = GetPlayer();		// ボールの情報
	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();
	
	MAP* Map = pField->GetFieldMap();	// マップ
	
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int			l = 0;
		
		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_ITEM||
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


	return hit;  // ぶつかったかどうかを示す
}

float	COLLISION::BombEnemyCollision(BOMB* pBomb, ENEMY_NORMAL* pEnemy)
{
	float		hit = 0.0f;				// ヒットした方向
	//BALL*		Ball = GetBall();		// ボールの情報
	XMFLOAT3 EnemyPos = pEnemy->GetEnemyPosition();
	XMFLOAT3 EnemyVel = pEnemy->GetEnemyVelocity();
	BOMBSOURCE* pBombSource = pBomb->Bomb_GetBomb();
	

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		int			l = 0;
		bool test = false;

		if (pBombSource[i].BombSource_GetState() == BOMB_STATE::BOMB_EXPLOSION)
		{
			XMFLOAT3 BombPos = pBombSource[i].BombSource_GetPosition();
			XMFLOAT3 BombVel = pBombSource[i].BombSource_GetVelocity();
			
			float BoxTop;	// BOXの+Y面の座標

			BoxTop = EnemyPos.y + BOX_RADIUS;

			// 壁としての判定処理
			if (BombPos.x + 3 > EnemyPos.x &&
				BombPos.x - 3 < EnemyPos.x)
			{
				if (BombPos.y + 3 > EnemyPos.y &&
					BombPos.y - 3 < EnemyPos.y)
				{
					if (BombPos.z + 3 > EnemyPos.z &&
						BombPos.z - 3 < EnemyPos.z)
					{
						test = true;
					}
				}
			}


			if (test)
			{
				pEnemy->SetEnemyNormalState(ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_DEAD);
			}
			
		}
	}

	return hit;
}