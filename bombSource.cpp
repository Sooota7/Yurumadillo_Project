#include "bombSource.h"

void BOMBSOURCE::BombSource_Initialize(XMFLOAT3 pos, BOMB_STATE state)
{
	m_Position = pos;
	m_State = state;
	m_Count = 0;
}

void BOMBSOURCE::BombSource_Finalize(void)
{
}

<<<<<<< HEAD
=======
void BOMBSOURCE::BombSource_Update(void)
{
	switch (m_State)
	{
	case BOMB_NONE:
		break;
	case BOMB_SAFE:
		break;
	case BOMB_ACTIVE:
		break;
	case BOMB_EXPLOSION:
		break;
	default:
		break;
	}
}

>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
void BOMBSOURCE::BombSource_Safe()
{

}

void BOMBSOURCE::BombSource_Active()
{
<<<<<<< HEAD
	m_Count += 1.0f / 60.0f;
=======
	
>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
}

void BOMBSOURCE::BombSource_Explosion()
{

}

void BOMBSOURCE::BombSource_SetState()
{

}
