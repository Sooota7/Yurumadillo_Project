// ====================================================================
// 
// PlyerUI.h             制作者: 杉森奏太
// 日付: 12/18
// 
// ====================================================================


#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "player.h"


class PlayerUI
{
private:
	static constexpr int P_UI_MAX = 2;
	static constexpr int HP_DIGIT_MAX = 5;


	// ▼ 追加: ウェーブ用パラメータ
		// 1frame = 1/60sec 固定
	float m_TimeSec = 0.0f;       // 経過時間[s]
	float m_FreqHz = 0.6f;       // 現在の周波数[Hz]
	float m_HpRatio = 1.0f;       // 0.0~1.0 (現在ハート数/最大ハート数)
	float m_AmpPx = 2.0f;       // 現在の振幅[px]
	float m_ScaleAmp = 0.06f;     // スケールの脈動量
	float m_PhasePerHeart = XM_2PI / 4.0f; // 1ハートごとの位相差(90deg)

	// チューニング可能な最小/最大値
	static constexpr float FREQ_MIN = 0.6f; // 高HP時の遅い脈動
	static constexpr float FREQ_MAX = 2.4f; // 低HP時の速い脈動
	static constexpr float AMP_MIN = 2.0f; // 高HP時の小さな上下
	static constexpr float AMP_MAX = 7.0f; // 低HP時の大きな上下

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	PLAYER* m_pPlayer = { NULL };

	int m_HpDigit;

	ID3D11ShaderResourceView* m_Texture[P_UI_MAX] = { NULL };
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLAYER* pPlayer);
	void Finalize();
	void Update();
	void Draw();

private:
	void Update_HpDigit();		//	現在の体力を更新
	void Draw_HpDigit();

	// ▼ 追加: ウェーブ用パラメータ更新
	void Update_WaveParams();

};

#endif  // PLAYERUI_H
