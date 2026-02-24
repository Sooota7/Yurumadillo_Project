#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "model.h"
#include "shader.h"
#include "Dictionary.h"

using namespace DirectX;

class GOAL
{
public:
    void Goal_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
    void Goal_Finalize();
    void Goal_Update();
    void Goal_Draw();

    // 追加: 当たり判定用にアイテム配列へアクセスする getter
    struct GoalItem
    {
        XMFLOAT3 pos;
        bool     active;
    };

    // 注意: 呼び出し側は nullptr チェックを行ってください
    GoalItem* GetGoalItems() { return m_pItems; }
    int       GetGoalCount() const { return m_Count; }

private:
    ID3D11Device*           m_pDevice = nullptr;
    ID3D11DeviceContext*    m_pContext = nullptr;
    MODEL*                  m_pModel = nullptr;
    GoalItem*               m_pItems = nullptr;
    int                     m_Count = 0;
};

