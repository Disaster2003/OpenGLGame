/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <memory>

// 先行宣言
class Engine;

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual bool Initialize(Engine& engine)
	{
		return true;
	}
	virtual void Update(Engine& engine, float deltaTime)
	{}
	virtual void Finalize(Engine& engine)
	{}
};

using ScenePtr = std::shared_ptr<Scene>; // シーンポインタ型

#endif // SCENE_H_INCLUDED