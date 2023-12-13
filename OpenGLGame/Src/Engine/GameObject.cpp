/**
* @file GameObject.cpp
*/
#include "GameObject.h"
#include <algorithm>

/// <summary>
/// ゲームオブジェクトから削除予定のコンポーネントを削除する
/// </summary>
void GameObject::RemoveDestroyedComponent()
{
    if (components.empty())
    {
        return; // コンポーネントを持っていなければ何もしない
    }

    // 破棄予定の有無でコンポーネントを分ける
    const auto iter =
        std::stable_partition
        (
            components.begin(), // 範囲の先頭
            components.end(),   // 範囲の終端
            // 振り分け条件
            [](const auto& p)
            {
                return !p->IsDestroyed();
            });

    // 破棄予定のコンポーネントを別の配列に移動
    // 引数 : もとになるイテレータ
    std::vector<ComponentPtr> destroyList(
        std::move_iterator(iter),
        std::move_iterator(components.end()));

    // 配列から移動済みコンポーネントを削除
    components.erase(iter, components.end());

    // 破棄予定のコンポーネントのOnDestoryを実行
    for (auto& e : destroyList)
    {
        e->OnDestroy();
    }

    // ここで実際にコンポーネントが削除される(destoyListの寿命が終わるため)
}

/// <summary>
/// スタートイベント
/// </summary>
void GameObject::Start()
{
    for (auto& e : components)
    {
        if (!e->isStarted)
        {
            e->Start();
            e->isStarted = true;
        }
    }
}

/// <summary>
/// 更新イベント
/// </summary>
/// <param name="deltaTime">前回の更新からの経過時間(秒)</param>
void GameObject::Update(float deltaTime)
{
    for (auto& e : components)
    {
        e->Update(deltaTime);
    }
    RemoveDestroyedComponent();
}

/// <summary>
/// 衝突イベント
/// </summary>
/// <param name="self">衝突したコンポーネント(自分)</param>
/// <param name="other">衝突したコンポーネント(相手)</param>
void GameObject::OnCollision(
    const ComponentPtr& self, const ComponentPtr& other)
{
    for (auto& e : components)
    {
        e->OnCollision(self, other);
    }
}

/// <summary>
/// 削除イベント
/// </summary>
void GameObject::OnDestroy()
{
    for (auto& e : components)
    {
        e->OnDestroy();
    }
}