// ダメージエフェクト

#pragma once

#include <cstdlib>
#include <random>
#include <DirectXMath.h>
using namespace DirectX;
#include "billboardManager.h"


class EffectSlash
{
private:
    struct PARTICLE
    {
        bool     active;    // 稼働中か
        XMFLOAT3 pos;
        XMFLOAT3 vel;
        XMFLOAT2 size;
        XMFLOAT4 color;     // rgba
        float      life;      // 経過フレーム
        int      bno;
    };


    static const int MAX_PARTICLE = 16; // 1出力あたりの粒上限
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
    void EffectSlash_Initialize();

    // 任意の位置にエフェクト
    void SpawnEffectSlash(XMFLOAT3& worldPos, float hit = 0.0f);
    // 各オブジェクトのUpdateから呼ぶ
    void EffectSlash_Update();
    // 各オブジェクトのDrawから呼ぶ
    void EffectSlash_Draw(BillboardManager* mgr);

};

