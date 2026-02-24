// goal.cpp
#include "goal.h"
#include "camera.h"
#include "shader.h"
#include "Dictionary.h"
#include "field.h" // FIELD_* enum (既存のプロジェクト構成に合わせて)

void GOAL::Goal_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
    m_pDevice = pDevice;
    m_pContext = pContext;
    m_Count = 0;

    // 最大数はマップ全セル数 (安全マージン)
    int maxItems = FIELD_WIDTH_X * FIELD_WIDTH_Z * FIELD_HEIGHT_Y;
    m_pItems = new GoalItem[maxItems];
    for (int i = 0; i < maxItems; ++i) { m_pItems[i].active = false; }

    // モデル読み込み（既存プロジェクトのゴールモデルパスに合わせる）
    m_pModel = ModelLoad("asset\\model\\test_goal.fbx");

    // マップを走査してゴール配置 (field.cpp と同じマップチェック値 5 を使用)
    int idx = 0;
    for (int y = 0; y < FIELD_HEIGHT_Y; ++y)
    {
        for (int z = 0; z < FIELD_WIDTH_Z; ++z)
        {
            for (int x = 0; x < FIELD_WIDTH_X; ++x)
            {
                int v = CheckMap(x, z, y, no); // 他ファイルと同スタイルの引数順に合わせる（Dictionary.cpp に準拠）
                if (v == 19) // ← ここを 9 から 5 に修正（FIELD_GOAL は 5）
                {
                    m_pItems[idx].pos = XMFLOAT3((float)x, (float)y, (float)z);
                    m_pItems[idx].active = true;
                    ++idx;
                }
            }
        }
    }

    m_Count = idx;
}

void GOAL::Goal_Finalize()
{
    if (m_pModel)
    {
        ModelRelease(m_pModel);
        m_pModel = nullptr;
    }

    delete[] m_pItems;
    m_pItems = nullptr;
    m_Count = 0;
    m_pDevice = nullptr;
    m_pContext = nullptr;
}

void GOAL::Goal_Update()
{
    // いまは静的表示のみ。将来的にアニメやフラグ管理をここで行う。
    // 例：アニメカウンタや点滅などを追加する場合はここに実装してください。
}

void GOAL::Goal_Draw()
{
    if (m_Count == 0 || m_pModel == nullptr) return;

    Shader_Begin();

    XMMATRIX Projection = GetProjectionMatrix();
    XMMATRIX View = GetViewMatrix();
    XMMATRIX VP = View * Projection;

    for (int i = 0; i < m_Count; ++i)
    {
        if (!m_pItems[i].active) continue;

        XMFLOAT3 pos = m_pItems[i].pos;

        XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
        XMMATRIX TranslationMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);
        XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(0.0f),
            XMConvertToRadians(0.0f),
            XMConvertToRadians(0.0f)
        );

        XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
        XMMATRIX WVP = World * VP;

        Shader_SetMatrix(WVP);

        // モデル描画
        ModelDraw(m_pModel);
    }
}