#pragma once

#include <memory>
#include <DirectXMath.h>
#include "Model/ModelCommonData.h"

#include "Object\Object.h"

class EnemyConstant
{
public:
    enum ANIMATION : AnimeIndex
    {
        IDLE_CHEST = 0,
        IDLE_NORMAL,
        IDLE_BATTLE,
        ATTACK01,
        ATTACK02,
        TAUNTING,
        VICTORY,
        MOVE_FWD,
        MOVE_BWD,
        MOVE_LFT,
        MOVE_RGT,
        RUN,
        SENSESOMETHINGST,
        SENSESOMETHINGRPT,
        GETHIT,
        DIZZY,
        DIE,

        MAX,
    };

public:
    static const MyHash ATTACK_OBJECT_NAME;

    static constexpr float MIN_IDLE_TIME = 0.5f;
    static constexpr float MAX_IDLE_TIME = 5.0f;
    static constexpr float REMOVE_IDLE_TIME = 2.0f; // 死亡アニメーション再生終了から削除されるまでの時間

#ifdef _DEBUG
    static constexpr float DEFAULT_TERRITORY_RENGR = 185.0f;    // 敵の移動範囲
#else
    static constexpr float DEFAULT_TERRITORY_RENGR = 185.0f;    // 敵の移動範囲
#endif // _DEBUG
public:
    static const std::shared_ptr<Object>& CreateEnemy(const std::shared_ptr<Object>&);
    static const std::shared_ptr<Object>& CreateEnemy(const DirectX::XMFLOAT3 spawn_point, const std::shared_ptr<Object>&);

    // 円状のランダムな位置を取得
    static const DirectX::XMFLOAT3 GetRandomPointInRing(float inner_radius, float outer_radius);
    // 他オブジェクトと重ならない位置を取得
    static const DirectX::XMFLOAT3 GetNonOverlappingPointInRing(float inner_radius, float outer_radius);
};