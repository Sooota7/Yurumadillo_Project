#include "anim.h"

void ANIM::AnimInitialize()
{
	for (int i = 0; i < FLAME_MAX; i++)
	{
		fps[i].Position = { 0.0f,0.0f,0.0f };
		fps[i].Rotation = { 0.0f,0.0f,0.0f };
		fps[i].Fill = false;
		fps[i].SetPos = false;
	}

}
