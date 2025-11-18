#pragma once

// collision.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"sprite.h"
#include	"shader.h"
using namespace DirectX;


#include	"field.h"
#include	"Ball.h"

#define COE		(0.5f)


enum COLLISION_HIT
{
	HIT_NONE = 0,	// “–‚½‚Á‚Ä‚¢‚È‚¢
	HIT_GROUND,		// ’n–Ê‚É“–‚½‚Á‚½
	HIT_WALL_0,		// •Ç‚É-Z‚©‚ç“–‚½‚Á‚½
	HIT_WALL_1,		// +X
	HIT_WALL_2,		// +Z
	HIT_WALL_3,		// -X

};

float	BallFieldCollision();