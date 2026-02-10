#include "Dictionary.h"




static int GetMap(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return Field_pos_row[y][x];

		break;
	case(1):
		return Field_pos_nor[y][x];


		break;
	case(2):
		return Field_pos_high[y][x];


		break;
	case(3):
		return 0;
		break;
	case(4):
		return 0;
		break;
	default:
		break;
	}


}

static int GetMap2(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return Gimmick_pos_row[y][x];

		break;
	case(1):
		return Gimmick_pos_nor[y][x];


		break;
	case(2):
		return Gimmick_pos_high[y][x];

		break;
	case(3):
		return Gimmick_pos_high2[y][x];

		break;
	case(4):
		return Gimmick_pos_high3[y][x];

		break;
	default:
		break;
	}


}

static int GetMap3(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_pos_row[y][x];

		break;
	case(1):
		return tutorial_pos_nor[y][x];


		break;
	case(2):
		return tutorial_pos_high[y][x];


		break;
	case(3):
		return 0;
		break;
	case(4):
		return 0;
		break;
	default:
		break;
	}


}

static int GetMap4(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_balloon_pos_row[y][x];

		break;
	case(1):
		return tutorial_balloon_pos_nor[y][x];


		break;
	case(2):
		return tutorial_balloon_pos_high[y][x];


		break;
	case(3):
		return 0;
		break;
	case(4):
		return 0;
		break;
	default:
		break;
	}


}

static int GetMap5(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_mouse_pos_row[y][x];

		break;
	case(1):
		return tutorial_mouse_pos_nor[y][x];


		break;
	case(2):
		return tutorial_mouse_pos_high[y][x];


		break;
	case(3):
		return 0;
		break;
	case(4):
		return 0;
		break;
	default:
		break;
	}


}

int GetMap6(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return boss_pos_row[y][x];

		break;
	case(1):
		return boss_pos_nor[y][x];


		break;
	case(2):
		return boss_pos_high[y][x];


		break;
	case(3):
		return 0;
		break;
	case(4):
		return 0;
		break;
	default:
		break;
	}


}

int CheckMap(int x, int y, int z, FIELD_NO no)
{
	switch (no)
	{
	case NO_NONE:
		break;
	case NO_1:
		return GetMap(x, y, z);
		break;
	case NO_2:
		return GetMap2(x, y, z);
		break;
	case NO_3:
		return GetMap3(x, y, z);
		break;
	case NO_4:
		return GetMap4(x, y, z);
		break;
	case NO_5:
		return GetMap5(x, y, z);
		break;
	case NO_6:
		return GetMap6(x, y, z);
		break;
	default:
		break;
	}


}
