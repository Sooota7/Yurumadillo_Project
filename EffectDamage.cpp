#include "EffectDamage.h"

void EffectDamage::SpawnEffectDamage(XMFLOAT3& worldPos)
{

    int emit = (int)RandRange(8.0f, 10.0f); // 8〜14 個
    if (emit < 1) { emit = 1; }
    if (emit > 14) { emit = 14; }

    for (int n = 0; n < emit; n++)
    {
        // 空き検索
        int slot = -1;
        for (int i = 0; i < MAX_PARTICLE; i++)
        {
            if (!m_p[i].active)
            {
                slot = i;
                break;
            }
        }
        if (slot < 0)
        {
            break;
        }

        PARTICLE* p = &m_p[slot];
        p->active = true;

        // 発生位置：基準位置の少し上・前にバラす（オブジェクト種別に応じて調整可）
        XMFLOAT3 pos = worldPos;
        pos.y += RandRange(0.20f, 0.60f);
        pos.z += RandRange(0.10f, 0.40f);
        p->pos = pos;

        // 初速：
        p->vel.x = RandRange(-0.1f, 0.1f);
        p->vel.y = RandRange(0.05f, 0.1f);
        p->vel.z = RandRange(-0.1f, 0.1f);

        // サイズ：0.20〜0.8
        float s = RandRange(0.20f, 0.8f);
        p->size = XMFLOAT2(s, s);

        // 色：（主張しすぎない）＋ アルファ控えめ
        float g = RandRange(0.75f, 1.00f);
        float b = RandRange(0.75f, 1.00f);
        float a = RandRange(0.50f, 0.80f);
        p->color = XMFLOAT4(1.0f, g, b, a);

        // 寿命：
        p->life = 0;
        p->lifeMax = (int)RandRange(30.0f, 60.0f);
    }

}

void EffectDamage::EffectDamage_Update()
{
    const float gravityY = -0.0020f; // 弱い落下。ふわっとさせるなら 0
    const float damping = 0.92f;    // 速度減衰

    for (int i = 0; i < MAX_PARTICLE; i++)
    {
        if (!m_p[i].active)
        {
            continue;
        }

        if (m_p[i].life >= m_p[i].lifeMax)
        {// 寿命が切れたら
            m_p[i].active = false;
            continue;
        }

        // 速度・位置
        m_p[i].vel.y += gravityY;
        m_p[i].pos.x += m_p[i].vel.x;
        m_p[i].pos.y += m_p[i].vel.y;
        m_p[i].pos.z += m_p[i].vel.z;

        m_p[i].vel.x *= damping;
        m_p[i].vel.y *= damping;
        m_p[i].vel.z *= damping;

        // サイズ・アルファを緩やかに下げる
        float t = 0.0f;
        if (m_p[i].lifeMax > 0)
        {
            t = (float)m_p[i].life / (float)m_p[i].lifeMax; // 現在の寿命 / 最大寿命
        }
        if (t < 0.0f) { t = 0.0f; }
        if (t > 1.0f) { t = 1.0f; }

        float s0 = m_p[i].size.x;
        float s = s0 * (1.0f - 0.7f * t); // 30% まで縮小
        if (s < 0.01f) { s = 0.01f; }
        m_p[i].size.x = s;
        m_p[i].size.y = s;

        float a0 = m_p[i].color.w;
        float a = a0 * (1.0f - t);
        if (a < 0.0f) { a = 0.0f; }
        m_p[i].color.w = a;

        m_p[i].life += 1;
    }

}

void EffectDamage::EffectDamage_Draw(BillboardManager* mgr)
{
    for (int i = 0; i < MAX_PARTICLE; i++)
    {
        if (!m_p[i].active)
        {
            continue;
        }

        // ビルボード設定
        XMFLOAT3 pos = m_p[i].pos;
        XMFLOAT2 size = m_p[i].size;
        XMFLOAT4 col = m_p[i].color;

        int bno = 1;
        int wc = 1;
        int hc = 1;

        Billboard* bb = new Billboard(pos, size, col, bno, wc, hc, BILLBOARD_TEXTURE::DAMAGE);
        mgr->Register(bb);

    }
}
