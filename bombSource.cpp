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

void BOMBSOURCE::BombSource_Safe()
{

}

void BOMBSOURCE::BombSource_Active()
{
	m_Count += 1.0f / 60.0f;
}

void BOMBSOURCE::BombSource_Explosion()
{

}

void BOMBSOURCE::BombSource_SetState()
{

}
