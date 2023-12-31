/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED	// インクルードガード
#define ENGINE_H_INCLUDED	// インクルードガード
#include "glad/glad.h"		// GLADライブラリの関数が定義されているヘッダファイル
#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>		// GLFWライブラリの関数が定義されているヘッダファイル
#include <string>			// 文字列型や文字列操作関数などが定義されているヘッダファイル

/// <summary>
/// ゲームエンジン
/// </summary>
class Engine
{
public:
	Engine() = default;
	~Engine() = default;
	int Run();

	/// <summary>
	/// ゲームオブジェクトを作成する
	/// </summary>
	/// <param name="name">オブジェクトの名前</param>
	/// <param name="position">オブジェクトを配置する座標</param>
	/// <param name="rotation">オブジェクトの回転角度</param>
	template<typename T>
	std::shared_ptr<T> Create
	(
		const std::string& name,
		const vec3& position = { 0, 0, 0 },
		const vec3& rotation = { 0, 0, 0 }
	)
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		gameObjects.push_back(p); // エンジンに登録
		return p;
	}

	// すべてのゲームオブジェクトを削除する
	void ClearGameObjectAll();

	// 次のシーンを設定する
	template<typename T>
	void SetNextScene()
	{
		nextScene = std::make_shared<T>();
	}

	// カメラを取得する
	GameObject& GetMainCamera()
	{
		return camera;
	}
	const GameObject& GetMainCamera() const
	{
		return camera;
	}

	// キーが押されていたらtrue, 押されてなければfalseを返す
	bool GetKey(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

private:
	int Initialize();
	void Update();
	void Render();
	void UpdateGameObject(float deltaTime);
	void RemoveDestroyedGameObject();

	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
	const std::string title = "OpenGLGame"; // ウィンドウタイトル
	GLuint vs = 0;							// 頂点シェーダの管理番号
	GLuint fs = 0;							// フラグメントシェーダの管理番号
	GLuint prog3D = 0;						// シェーダプログラムの管理番号
	GLuint vbo = 0;							// 頂点バッファの管理番号
	GLuint ibo = 0;							// インデックスバッファの管理番号
	GLuint vao = 0;							// 頂点属性配列の管理番号
	GLintptr vboSize = 0;					// 頂点バッファの現在のサイズ
	GLintptr iboSize = 0;					// インデックスバッファの現在のサイズ
	std::vector<DrawParams> drawParamsList;	// 描画パラメータ配列
	GameObjectList gameObjects;				// ゲームオブジェクト配列
	double previousTime = 0;				// 前回更新時の時刻
	float deltaTime = 0;					// 前回更新からの経過時間
	ScenePtr scene;							// 実行中のシーン
	ScenePtr nextScene;						// 次のシーン

	GameObject camera;						// カメラオブジェクト
	GLsizei indexCount = 0;
};

#endif						// !ENGINE_H_INCLUDED(インクルードガード)