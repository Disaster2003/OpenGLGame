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
 
+#pragma region カメラの初期設定
+    camera.position = { 3, 1, 3 };
+    camera.rotation.y = 3.14159265f;
+#pragma endregion
 
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

[maze00.txt](https://github.com/tn-mai/OpenGL3D2023/tree/main/res)

maze00.txtを

プロジェクト名\Resにダウンロード

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

## 2.テクスチャの切り替え
プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Texture.hを入力

->追加をクリック

`Texture.h`
```diff
+/**
+* @file Texture.h
+*/
+#ifndef TEXTURE_H_INCLUDED
+#define TEXTURE_H_INCLUDED
+#include "glad/glad.h"
+#include <string>
+#include <memory>
+
+// 先行宣言
+class Texture;
+using TexturePtr = std::shared_ptr<Texture>;
+
+/// <summary>
+/// テクスチャ管理クラス
+/// </summary>
+class Texture
+{
+public:
+	explicit Texture(const char* filename);
+	~Texture();
+
+	// コピーと代入を禁止
+	Texture(const Texture&) = delete;
+	Texture& operator=(const Texture&) = delete;
+
+	// 管理番号を取得
+	operator GLuint() const
+	{
+		return id;
+	}
+
+	// 幅と高さを取得
+	int GetWidth() const
+	{
+		return width;
+	}
+	int GetHeight() const
+	{
+		return height;
+	}
+
+	// 名前を取得
+	const std::string& GetName() const
+	{
+		return name;
+	}
+
+private:
+	std::string name; // テクスチャ名(主にデバッグ用)
+	GLuint id = 0;    // オブジェクト管理番号
+	int width = 0;    // テクスチャの幅
+	int height = 0;   // テクスチャの高さ
+};
+
+#endif // TEXTURE_H_INCLUDED
```

`deleteによる関数定義の禁止`

管理すべき資源のコピーがあるとエラーになる場合,

「コピーコンストラクタ」と「コピー代入演算子」の

定義を禁止して,

コピー及び代入をできなくする必要あり

関数の定義を禁止するには,

メンバ関数宣言の末尾に= deleteを付ける

Texutreクラスの定義でコピーと代入を

禁止しているのは,

多重解放エラーが起きないようにするため

`Engine.cpp`
```diff
     // シェーダ文字列のコンパイル
     // 引数 : シェーダの管理番号
     glCompileShader(object);
     return object;
 }
 
-/// <summary>
-/// テクスチャの読み込み
-/// </summary>
-/// <param name="filename">テクスチャファイル名</param>
-/// <returns>テクスチャの管理番号</returns>
-GLuint LoadTexture(const char* filename)
-{
-    // ファイルを開く
-    // std::ios::binary : バイナリモードの指定
-    std::ifstream file
-    (
-        filename,
-        std::ios::binary
-    );
-    // ファイルが正常に開けない(false)時,
-    // エラーメッセージを出力して,
-    // 0(オブジェクトが存在しない)を返して終了
-    if (!file)
-    {
-        char s[256];
-        snprintf
-        (
-            s,
-            256,
-            "[ERROR] %s: %sを開けません\n",
-            __func__,
-            filename
-        );
-        OutputDebugStringA(s);
-        return 0;
-    }
-
-    // ファイルサイズの取得
-  // 引数 : ファイルパス
-    const size_t filesize = std::filesystem::file_size(filename);
-    // ファイルサイズ分の領域を用意
-    std::vector<uint8_t> buffer(filesize);
-    // ファイルの読み込み
-    file.read
-    (
-        reinterpret_cast<char*>(buffer.data()),   // バッファアドレス
-        filesize                                  // 読み込むバイト数
-    );
-    // ファイルを閉じる
-    file.close();
-
-    // ヘッダから情報を取得
-    const size_t tgaHeaderSize = 18; // ヘッダ情報のバイト数
-    const int width = buffer[12] + buffer[13] * 256;
-    const int height = buffer[14] + buffer[15] * 256;
-
-    GLuint object = 0; // テクスチャの管理番号
-    // テクスチャの作成
-    glCreateTextures
-    (
-        GL_TEXTURE_2D,    // テクスチャの種類
-        1,                // 作成する個数
-        &object           // 番号を格納する配列のアドレス
-    );
-    // 画像データ分のGPUメモリ領域の確保
-    glTextureStorage2D
-    (
-        object,   // テクスチャの管理番号
-        1,        // 作成するレベル数(ミップマップ)
-        GL_RGBA8, // ピクセル形式
-        width,    // 幅
-        height    // 高さ
-    );
-    // 画像データをGPUメモリにコピー
-    glTextureSubImage2D
-    (
-        object,                       // テクスチャの管理番号
-        0,                            // コピー先のレイヤー番号
-        0,                            // コピー先のX座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
-        0,                            // コピー先のY座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
-        width,                        // コピーする画像の幅
-        height,                       // コピーする画像の高さ
-        GL_BGRA,                      // ピクセルに含まれる要素と順序
-        GL_UNSIGNED_BYTE,             // 要素の型
-        buffer.data() + tgaHeaderSize // 画像データのアドレス
-    );
-    return object;
-}
 
 /// <summary>
 /// ゲームエンジンの実行
 /// </summary>
 /// <returns>0 : 正常に実行が完了,
 /// 0以外 : エラーが発生</returns>
 int Engine::Run()
 {
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Texture.cppを入力

->追加をクリック

`Texture.cpp`
```diff
+/**
+* @file Texture.cpp
+*/
+#include "Texture.h"
+#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
+#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
+#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル
+
+#pragma warning(push)
+#pragma warning(disable:4005)
+#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
+#pragma warning(pop)
+
+/// <summary>
+/// コンストラクタ
+/// </summary>
+/// <param name="filename">テクスチャファイル名</param>
+/// <returns>テクスチャの管理番号</returns>
+Texture::Texture(const char* filename)
+{
+    // ファイルを開く
+    // std::ios::binary : バイナリモードの指定
+    std::ifstream file
+    (
+        filename,
+        std::ios::binary
+    );
+    // ファイルが正常に開けない(false)時,
+    // エラーメッセージを出力して,
+    // 0(オブジェクトが存在しない)を返して終了
+    if (!file)
+    {
+        char s[256];
+        snprintf
+        (
+            s,
+            256,
+            "[ERROR] %s: %sを開けません\n",
+            __func__,
+            filename
+        );
+        OutputDebugStringA(s);
+        return;
+    }
+
+    // ファイルサイズの取得
+    // 引数 : ファイルパス
+    const size_t filesize = std::filesystem::file_size(filename);
+    // ファイルサイズ分の領域を用意
+    std::vector<uint8_t> buffer(filesize);
+    // ファイルの読み込み
+    file.read
+    (
+        reinterpret_cast<char*>(buffer.data()),   // バッファアドレス
+        filesize                                  // 読み込むバイト数
+    );
+    // ファイルを閉じる
+    file.close();
+
+    // ヘッダから情報を取得
+    const size_t tgaHeaderSize = 18; // ヘッダ情報のバイト数
+    width = buffer[12] + buffer[13] * 256;
+    height = buffer[14] + buffer[15] * 256;
+
+    GLuint object = 0; // テクスチャの管理番号
+    // テクスチャの作成
+    glCreateTextures
+    (
+        GL_TEXTURE_2D,    // テクスチャの種類
+        1,                // 作成する個数
+        &object           // 番号を格納する配列のアドレス
+    );
+    // 画像データ分のGPUメモリ領域の確保
+    glTextureStorage2D
+    (
+        object,   // テクスチャの管理番号
+        1,        // 作成するレベル数(ミップマップ)
+        GL_RGBA8, // ピクセル形式
+        width,    // 幅
+        height    // 高さ
+    );
+    // 画像データをGPUメモリにコピー
+    glTextureSubImage2D
+    (
+        object,                       // テクスチャの管理番号
+        0,                            // コピー先のレイヤー番号
+        0,                            // コピー先のX座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
+        0,                            // コピー先のY座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
+        width,                        // コピーする画像の幅
+        height,                       // コピーする画像の高さ
+        GL_BGRA,                      // ピクセルに含まれる要素と順序
+        GL_UNSIGNED_BYTE,             // 要素の型
+        buffer.data() + tgaHeaderSize // 画像データのアドレス
+    );
+
+    id = object;
+    name = filename;
+}
+
+/// <summary>
+/// デストラクタ
+/// </summary>
+Texture::~Texture()
+{
+    // テクスチャオブジェクトの削除
+    glDeleteTextures
+    (
+        1,
+        &id
+    );
+}
```

`GameObject.h`
```diff
 /**
 * @file GameObject.h
 */
 #ifndef GAMEOBJECT_H_INCLUDED
 #define GAMEOBJECT_H_INCLUDED
 #include "Component.h"
+#include "Texture.h"
 #include "VecMath.h"
 #include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
 #include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
 #include <memory>
```
```diff
     std::string name;                   // オブジェクト名
     vec3 position = { 0, 0, 0 };        // 物体の位置
     vec3 rotation = { 0, 0, 0 };        // 物体の回転角度
     vec3 scale = { 1,1,1 };             // 物体の拡大率
     float color[4] = { 1, 1, 1, 1 };    // 物体の色
+    TexturePtr texColor;                // 表示に使うカラーテクスチャ
 
 private:
     Engine* engine = nullptr;             // エンジンのアドレス
     bool isDestroyed = false;             // 死亡フラグ
     std::vector<ComponentPtr> components; // コンポーネント配列
 };
```

## 課題01
内容

下記のURLからfloor.pngと

wall.pngという画像ファイルを

ダウンロードしなさい.

ダウンロードした2つのファイルを

TGA形式に変換し,プロジェクトの

Resフォルダに保存しなさい.

[res](https://github.com/tn-mai/OpenGL3D2023/tree/main/res/)

```C++
floor.pngとwall.pngをダウンロード

->VisualStudioにドラッグ&ドロップ

->ファイル\名前を付けて保存

->ファイルの種類をTGAファイル(*.tga)にして

->プロジェクト名\Resに保存
```

`MainGameScene.cpp`
```diff
     // 床を作成
     auto floor = engine.Create<GameObject>("floor");
     floor->scale = {
       static_cast<float>(mapSizeX) * squareScale, 1,
       static_cast<float>(mapSizeY) * squareScale };
     floor->position = { floor->scale.x, -1, floor->scale.z };
+    floor->texColor = std::make_shared<Texture>("Res/floor.tga");
 
     // 壁を作成
+    auto texWall = std::make_shared<Texture>("Res/wall.tga");
     for (int y = 0; y < mapSizeY; ++y)
     {
         for (int x = 0; x < mapSizeX; ++x)
         {
             const float posX = static_cast<float>(x + 0.5) * squareSize;
             const float posZ = static_cast<float>(y + 0.5) * squareSize;
             if (GetMapData(x, y) == '#')
             {
                 auto wall = engine.Create<GameObject>(
                     "wall", { posX, 0.5f * squareSize, posZ });
                 wall->scale = { squareScale, squareScale, squareScale };
+                wall->texColor = texWall;
             }
         } // for x
     } // for y
 
     return true; // 初期化成功
 }
```

`Engine.cpp`
```diff
 );
 glProgramUniform2f
 (
     prog3D,
     2,
     sin(e->rotation.y),
     cos(e->rotation.y)
 );
 
+// 描画に使うテクスチャを
+// (テクスチャ・イメージ・ユニットに)割り当て
+if (e->texColor)
+{
+    const GLuint tex = *e->texColor;
+    glBindTextures
+    (
+        0,      // 割り当て開始インデックス
+        1,      // 割り当てる個数
+        &tex    // テクスチャ管理番号配列のアドレス
+    );
+}
 
 // 図形の描画
 glDrawElementsInstanced
 (
```

## 課題02
内容

Engineクラスのtexメンバ変数と,

このメンバ変数を使用しているプログラムを

全て削除しなさい.

`Engine.h`
```diff
 	ScenePtr scene;							// 実行中のシーン
 	ScenePtr nextScene;						// 次のシーン
-	GLuint tex = 0;							// テクスチャ
 
 	GameObject camera;						// カメラオブジェクト
 	GLsizei indexCount = 0;
 };
 
 #endif						// !ENGINE_H_INCLUDED(インクルードガード)
```

`Engine.cpp`
```diff
 #pragma region ゲームオブジェクト配列の容量を予約
     gameObjects.reserve(1000);
 #pragma endregion
 
-#pragma region テクスチャの作成
-    tex = LoadTexture("Res/box.tga");
-#pragma endregion
 
 #pragma region カメラの初期設定
     camera.position = { 3, 1, 3 };
     camera.rotation.y = 3.14159265f;
 #pragma endregion
```

## 3.複数の図形を表示