#pragma once

//Player.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"
#include    "bombSource.h"

#define PLAYER_SPEEDMAX (0.05f)      //æœ€é«˜é€Ÿåº¦
#define PLAYER_RADIUS (0.2f)
#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)
<<<<<<< HEAD
#define PLAYER_ACCELERATION (0.01f)  //‰Á‘¬“x
#define PLAYER_GRAVITY (0.01f)       //d—Í
<<<<<<< HEAD
#define PLAYER_JUMP (0.3f)           //ƒWƒƒƒ“ƒv—Í
=======
#define PLAYER_FALLMAX (-0.2f)        //—‰ºÅ‚‘¬“x
#define PLAYER_JUMP (0.2f)           //ƒWƒƒƒ“ƒv—Í
>>>>>>> e5e005faf78d341748218fb44b15d22f971cf71d
#define PLAYER_DEATH (-3.0f)         //‚±‚êˆÈã‰º‚És‚­‚Æ€
#define PLAYER_MAGICRANGE (0.03f)    //–‚–@”ÍˆÍ
=======
#define PLAYER_ACCELERATION (0.01f)  //åŠ é€Ÿåº¦
#define PLAYER_GRAVITY (0.01f)       //é‡åŠ›
#define PLAYER_FALLMAX (-0.2f)        //è½ä¸‹æœ€é«˜é€Ÿåº¦
#define PLAYER_JUMP (0.2f)           //ã‚¸ãƒ£ãƒ³ãƒ—åŠ›
#define PLAYER_DEATH (-3.0f)         //ã“ã‚Œä»¥ä¸Šä¸‹ã«è¡Œãã¨æ­»
#define PLAYER_MAGICRANGE (0.03f)    //é­”æ³•ç¯„å›²
>>>>>>> 6174eedcea92a7148fda2e2418c94a9a79d31790

//ãƒœãƒ¼ãƒ«ã®çŠ¶æ…‹
enum PLAYER_STATE
{
	PLAYER_STATE_IDLE = 0,	//ä½•ã‚‚ã—ãªã„
	PLAYER_STATE_MOVE,		//ç§»å‹•
	PLAYER_STATE_JUMP,
	PLAYER_STATE_RESPAWN,   //ãƒªã‚¹ãƒãƒ¼ãƒ³
	
};

//ãƒœãƒ¼ãƒ«æ§‹é€ ä½“
class PLAYER
{
private:
	XMFLOAT3	m_Position;	//è¡¨ç¤ºåº§æ¨™
	XMFLOAT3	m_Rotation;	//å›è»¢è§’
	XMFLOAT3	m_Scaling;	//æ‹¡å¤§ç‡
	XMFLOAT3	m_Velocity;	//é€Ÿåº¦
	XMFLOAT3	m_Acceleration;	// è½ä¸‹é€Ÿåº¦

	PLAYER_STATE	m_State;		//çŠ¶æ…‹
	MODEL* m_Model[2];		//ãƒ¢ãƒ‡ãƒ«ãƒ‡ãƒ¼ã‚¿

	 BOMBSOURCE* m_Bomb;

	 bool JumpCount;

private:
	void	Player_Idle();
	void	Player_Move();
	void    Player_Jump();
	void    Player_BombMagic(BOMBSOURCE* pBomb);
	void    Player_Respawn();

public:
	void	Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Player_Finalize();
	void	Player_Update();
	void	Player_Draw();

	void SetPlayerPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetPlayerPosition() { return m_Position; };

	void SetPlayerRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetPlayerRotation() { return m_Rotation; };

	void SetPlayerScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetPlayerScaling() { return m_Scaling; };

	void SetPlayerVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetPlayerVelocity() { return m_Velocity; };

	void SetPlayerJump(bool jump) { JumpCount = jump; };
	BOOL GetPlayerJump() { return JumpCount; };

	PLAYER* GetPlayer();
};







