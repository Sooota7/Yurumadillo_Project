
// ダメージエフェクト

#pragma once

#include <cstdlib>
#include <random>
#include <DirectXMath.h>
using namespace DirectX;
#include "billboardManager.h"


class EffectDamage
{
private:
    struct PARTICLE
    {
        bool     active;    // 稼働中か
        XMFLOAT3 pos;
        XMFLOAT3 vel;
        XMFLOAT2 size;
        XMFLOAT4 color;     // rgba
        int      life;      // 経過フレーム
        int      lifeMax;   // 最大寿命フレーム
    };


    static const int MAX_PARTICLE = 64; // 1出力あたりの粒上限
    PARTICLE m_p[MAX_PARTICLE];

    // 乱数 0～1
    float Rand01()
    {
        static std::mt19937 mt{ std::random_device{}() };
        static std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };

        return dist(mt);
    }

    // 乱数 [a,b]
    float RandRange(float a, float b)
    {
        float t = Rand01();
        float d = b - a;
        return a + d * t;   // 最小値＋(範囲 * 割合)
    }


public:
    void EffectDamage_Initialize()
    {
        for (int i = 0; i < MAX_PARTICLE; i++)
        {
            m_p[i].active = false;
            m_p[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
            m_p[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
            m_p[i].size = XMFLOAT2(0.0f, 0.0f);
            m_p[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
            m_p[i].life = 0;
            m_p[i].lifeMax = 0;
        }
    }

    // 任意の位置にエフェクト
    void SpawnEffectDamage(XMFLOAT3& worldPos);
    // 各オブジェクトのUpdateから呼ぶ
    void EffectDamage_Update();
    // 各オブジェクトのDrawから呼ぶ
    void EffectDamage_Draw(BillboardManager* mgr);

};