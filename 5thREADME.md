# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.シーン管理
「シーン」(Unreal Engineでは「レベル」)は

ゲームプレイの管理方法のひとつ.

`ゲームプレイを定義するための`

`一連のゲームオブジェクトを配置し,`

`制御するシステム.`

※ ゲームプレイ

背景の設定,自機の操作,

敵の移動・攻撃,といった部分のこと


プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Scene.hを入力

->追加をクリック

`Scene.h`
```diff
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
```

`Engine.h`
```diff
 /**
 * @file Engine.h
 */
 #ifndef ENGINE_H_INCLUDED	// インクルードガード
 #define ENGINE_H_INCLUDED	// インクルードガード
 #include "glad/glad.h"		// GLADライブラリの関数が定義されているヘッダファイル
 #include "GameObject.h"
+#include "Scene.h"
 #include <GLFW/glfw3.h>		// GLFWライブラリの関数が定義されているヘッダファイル
 #include <string>			// 文字列型や文字列操作関数などが定義されているヘッダファイル
 
 /// <summary>
 /// ゲームエンジン
 /// </summary>
 class Engine
 {
```
```diff
 // すべてのゲームオブジェクトを削除する
 void ClearGameObjectAll();
 
+// 次のシーンを設定する
+template<typename T>
+void SetNextScene()
+{
+	nextScene = std::make_shared<T>();
+}
 
 // カメラを取得する
 GameObject& GetMainCamera()
 {
```
```diff
 GameObjectList gameObjects;				// ゲームオブジェクト配列
 double previousTime = 0;					// 前回更新時の時刻
 float deltaTime = 0;						// 前回更新からの経過時間
+ScenePtr scene;							// 実行中のシーン
+ScenePtr nextScene;						// 次のシーン
 GLuint tex = 0;							// テクスチャ
 
 GameObject camera;							// カメラオブジェクト
 ```

`Engine.cpp`
```diff
 #pragma region テクスチャの作成
     tex = LoadTexture("Res/box.tga");
 #pragma endregion
 
+    // カメラの初期設定
+    camera.position = { 3, 1, 3 };
+    camera.rotation.y = 3.14159265f;
 
     return 0; // 正常に初期化された
 }
 
 /// <summary>
 /// ゲームエンジンの状態を更新
 /// </summary>
 void Engine::Update()
 {
     // デルタタイム(前回の更新からの経過時間)の計算
     // glfwGetTime : プログラムが起動してからの経過時間の取得
     const double currentTime = glfwGetTime(); // 現在時刻
     deltaTime = static_cast<float>(currentTime - previousTime);
     previousTime = currentTime;
 
     // 経過時間が長すぎる場合は適当に短くする(主にデバッグ対策)
     if (deltaTime >= 0.5f)
     {
         deltaTime = 1.0f / 60.0f;
     }
 
+    // シーンの切り替え
+    if (nextScene)
+    {
+        if (scene)
+        {
+            scene->Finalize(*this);
+        }
+        nextScene->Initialize(*this);
+        scene = std::move(nextScene);
+    }
+
+    // シーンを更新
+    if (scene)
+    {
+        scene->Update(*this, deltaTime);
+    }
 
     UpdateGameObject(deltaTime);
 }
 
 /// <summary>
 /// ゲームエンジンの状態を描画
 /// </summary>
 void Engine::Render()
 {
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\MainGameScene.hを入力

->追加をクリック

`MainGameScene.h`
```diff
+/**
+* @file MainGameScene.h
+*/
+#ifndef MAINGAMESCENE_H_INCLUDED
+#define MAINGAMESCENE_H_INCLUDED
+#include "Engine/Scene.h"
+#include <vector>
+
+/// <summary>
+/// メインゲーム画面
+/// </summary>
+class MainGameScene : public Scene
+{
+public:
+	MainGameScene() = default;
+	virtual ~MainGameScene() = default;
+	virtual bool Initialize(Engine& engine) override;
+
+private:
+	char GetMapData(int x, int y) const
+	{
+		return map[y * mapSizeX + x];
+	}
+	char& GetMapData(int x, int y)
+	{
+		return map[y * mapSizeX + x];
+	}
+
+	std::vector<char> map; // マップデータ
+	int mapSizeX = 11;     // マップの広さ(横)
+	int mapSizeY = 11;     // マップの広さ(縦)
+	float squareSize = 2;  // マップの1マスの大きさ
+	float squareScale = squareSize / 2; // 壁・床の拡大率
+};
+
+#endif // MAINGAMESCENE_H_INCLUDED
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\MainGameScene.cppを入力

->追加をクリック

`MainGameScene.cpp`
```diff
+/**
+* @file MainGameScene.cpp
+*/
+#include "MainGameScene.h"
+#include "Engine/Engine.h"
+#include <string>
+#include <fstream>
+
+/// <summary>
+/// シーンの初期化
+/// </summary>
+/// <returns>true : 初期化成功
+/// false : 初期化失敗</returns>
+bool MainGameScene::Initialize(Engine& engine)
+{
+    // ファイルからマップデータを作成
+    std::ifstream ifs("Res/maze00.txt");
+    if (!ifs)
+    {
+        return false; // マップファイルの読み込みに失敗
+    }
+
+    ifs >> mapSizeX >> mapSizeY; // マップの大きさを読み込む
+    ifs.ignore(); // 改行を飛ばす
+
+    // マップデータを読み込む
+    map.resize(mapSizeX * mapSizeY);
+    for (int y = 0; y < mapSizeY; ++y)
+    {
+        std::string s;
+        std::getline(ifs, s);
+        for (int x = 0; x < mapSizeX; ++x)
+        {
+            GetMapData(x, y) = s[x];
+        } // for x
+    } // for y
+
+   // 床を作成
+    auto floor = engine.Create<GameObject>("floor");
+    floor->scale = {
+      static_cast<float>(mapSizeX) * squareScale, 1,
+      static_cast<float>(mapSizeY) * squareScale };
+    floor->position = { floor->scale.x, -1, floor->scale.z };
+    floor->color[0] = 0.4f;
+    floor->color[2] = 0.2f;
+
+    // 壁を作成
+    for (int y = 0; y < mapSizeY; ++y)
+    {
+        for (int x = 0; x < mapSizeX; ++x)
+        {
+            const float posX = static_cast<float>(x + 0.5) * squareSize;
+            const float posZ = static_cast<float>(y + 0.5) * squareSize;
+            if (GetMapData(x, y) == '#')
+            {
+                auto wall = engine.Create<GameObject>(
+                    "wall", { posX, 0.5f * squareSize, posZ });
+                wall->scale = { squareScale, squareScale, squareScale };
+            }
+        } // for x
+    } // for y
+
+    return true; // 初期化成功
+}
```

`Main.cpp`
```diff
 /**
 * @file Main.cpp
 */
 #include "Engine/Engine.h"
+#include "MainGameScene.h"
 #pragma warning(push)
 #pragma warning(disable:4005)
 #include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
 #pragma warning(pop)
```
```diff
 int WINAPI WinMain
 (
     _In_ HINSTANCE hInstnce,
     _In_opt_ HINSTANCE hPrevInstance,
     _In_ LPSTR lpCmdLine,
     _In_ int nShowCmd
 )
 {
     Engine engine;
+    engine.SetNextScene<MainGameScene>();
     return engine.Run();
 }
```

### 2.テクスチャの切り替え
