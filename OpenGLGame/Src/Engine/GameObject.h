/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "Component.h"
#include "VecMath.h"
#include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
#include <memory>

// 先行宣言
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>; // ゲームオブジェクトのポインタ
using GameObjectList = std::vector<GameObjectPtr>; // ゲームオブジェクトポインタの配列

/// <summary>
/// ゲームに登場する様々なオブジェクトを表す基本クラス
/// </summary>
class GameObject
{
    friend Engine;
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    // ゲームエンジンを取得
    Engine* GetEngine() const
    {
        return engine;
    }

    // ゲームオブジェクトをエンジンから削除する
    void Destroy()
    {
        isDestroyed = true;
    }

    // ゲームオブジェクトが破壊されていたらtrueを返す
    bool IsDestroyed() const
    {
        return isDestroyed;
    }

    // ゲームオブジェクトにコンポーネントを追加する
    template<typename T>
    std::shared_ptr<T> AddComponent()
    {
        auto p = std::make_shared<T>();
        p->owner = this;
        components.push_back(p);
        p->Awake();
        return p;
    }

    // ゲームオブジェクトからコンポーネントを削除する
    void RemoveDestroyedComponent();

    // イベント制御
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void OnCollision(
        const ComponentPtr& self, const ComponentPtr& other);
    virtual void OnDestroy();

    std::string name;                   // オブジェクト名
    vec3 position = { 0, 0, 0 };        // 物体の位置
    vec3 rotation = { 0, 0, 0 };        // 物体の回転角度
    vec3 scale = { 1,1,1 };             // 物体の拡大率
    float color[4] = { 1, 1, 1, 1 };    // 物体の色

private:
    Engine* engine = nullptr;             // エンジンのアドレス
    bool isDestroyed = false;             // 死亡フラグ
    std::vector<ComponentPtr> components; // コンポーネント配列
};

#endif // !GAMEOBJECT_H_INCLUDED