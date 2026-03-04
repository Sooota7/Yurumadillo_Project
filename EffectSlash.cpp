#include "EffectSlash.h"
#include "collision.h"

void EffectSlash::EffectSlash_Initialize()
{
    for (int i = 0; i < MAX_PARTICLE; i++)
    {
        m_p[i].active = false;
        m_p[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
        m_p[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
        m_p[i].size = XMFLOAT2(0.0f, 0.0f);
        m_p[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
        m_p[i].bno = 0;
        m_p[i].life = 0.0f;
    }
}

void EffectSlash::SpawnEffectSlash(XMFLOAT3& worldPos, float hit)
{
    for (int i = 0; i < MAX_PARTICLE; i++)
    {
        if (!m_p[i].active)
        {
            PARTICLE* p = &m_p[i];
            p->active = true;

            // 発生位置
            XMFLOAT3 pos = worldPos;
            pos.y += 0.5f;

            switch ((int)hit)
            {
            case COLLISION_HIT::HIT_WALL_0:

                break;
            case COLLISION_HIT::HIT_WALL_1:

                break;
            case COLLISION_HIT::HIT_WALL_2:

                break;
            case COLLISION_HIT::HIT_WALL_3:

                break;
            case COLLISION_HIT::HIT_GROUND:

                break;
            default:
                break;
            }

            p->pos = pos;

            // サイズ：0.20〜0.8
            float s = RandRange(2.5f, 2.8f);
            p->size = XMFLOAT2(s, s);

            p->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

            p->bno = 0;

            // 寿命：
            p->life = 0.0f;

            break;
        }
    }
}

void EffectSlash::EffectSlash_Update()
{
    for (int i = 0; i < MAX_PARTICLE; i++)
    {
        if (!m_p[i].active)
        {
            continue;
        }
        if (m_p[i].bno >= 3 * 2 - 1)
        {// 寿命が切れたら
            m_p[i].life = 0.0f;
            m_p[i].active = false;
        }

        m_p[i].life += 1 / 60.0f;
    }
}

void EffectSlash::EffectSlash_Draw(BillboardManager* mgr)
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

        int wc = 3;
        int hc = 2;

        if (m_p[i].life > (0.4f / (wc * hc)) * (m_p[i].bno + 1))
        {
            m_p[i].bno++;
        }

        Billboard* bb = new Billboard(pos, size, col, m_p[i].bno, wc, hc, BILLBOARD_TEXTURE::SLASH);
        mgr->Register(bb);

    }
}
