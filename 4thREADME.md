# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.プログラムの構造化
`構造化の思考基盤`
>1. プログラムのすべての構成要素を洗い出し,<br>
正しさ(意図したとおりに動くこと)を<br>
容易に証明できる程度に小さな単位へと<br>
分解する(分割,モジュール化).
>2. 分解した構成要素を階層化して,<br>
プログラム全体の正しさを段階的に<br>
証明できるようにする(可読性,テスト容易性).

`ゲームプログラムの二大構成`
>* どのゲームでも必要な要素(Engine)<br>
(入出力制御,画面描画,ゲームオブジェクト管理など)
>* ゲーム固有の要素<br>
(プレイヤーの操作方法,敵の行動,ゲームルールの管理など)

### 1-1.WinMain() -> Engine.Run()への移行
`Engineの要素分解`
>* ゲームループの制御
>* OpenGLの制御
>* シーン(レベル)の管理
>* ゲームオブジェクトの管理
>* アセット(モデル, テクスチャ, シェーダなど)の管理

`目標`

「WinMain関数の全てのプログラムを

エンジンクラスのメンバにする」こと

`分解したものを細分化,階層化すると...`
```C++
ゲームエンジン
+-+-- ゲームループ
| +---- 初期化
| +---- 更新
| +---- 描画
|
+----- OpenGL制御
|
+-+-- ゲームオブジェクト管理
| +---- ゲームオブジェクトリスト
| +---- ゲームオブジェクト
| +---- ゲームオブジェクトの追加
| +---- ゲームオブジェクトの削除
|
+-+-- アセット管理
   +---- モデルの追加
   +---- テクスチャの追加
   +---- シェーダの追加
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Engine.hを入力

->追加をクリック

`Engine.h`
```diff
+/**
+* @file Engine.h
+*/
+#ifndef ENGINE_H_INCLUDED	// インクルードガード
+#define ENGINE_H_INCLUDED	// インクルードガード
+#include "glad/glad.h"		// GLADライブラリの関数が定義されているヘッダファイル
+#include <GLFW/glfw3.h>	// GLFWライブラリの関数が定義されているヘッダファイル
+#include <string>			// 文字列型や文字列操作関数などが定義されているヘッダファイル
+
+/// <summary>
+/// ゲームエンジン
+/// </summary>
+class Engine
+{
+public:
+	Engine() = default;
+	~Engine() = default;
+	int Run();
+
+private:
+};
+
+#endif	// ENGINE_H_INCLUDED(インクルードガード)
```

`Main.cpp`
```diff
 /**
 * @file Main.cpp
 */
 #include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
+#include "Engine/Engine.h"
 #include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル
 
 #pragma warning(push)
```
```diff
 /// <summary>
 /// エントリーポイント
 /// </summary>
 int WINAPI WinMain
 (
     _In_ HINSTANCE hInstnce,
     _In_opt_ HINSTANCE hPrevInstance,
     _In_ LPSTR lpCmdLine,
     _In_ int nShowCmd
 )
 {
+    Engine engine;
+    return engine.Run();
+}
+
+/// <summary>
+/// ゲームエンジンを実行する
+/// </summary>
+/// <returns>0 : 正常に実行が完了,
+/// 0以外 : エラーが発生</returns>
+int Engine::Run()
+{
 #pragma region GLFWライブラリの初期化
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Engine.cppを入力

->追加をクリック

`Engine.cpp`
```diff
+/**
+* @file Engine.cpp
+*/
+#include "Engine.h"
```

### 1-2.ローカル変数 -> メンバ変数
`Runメンバ関数内のプログラムを3つに分解`
>* 初期化
>* 更新
>* 描画

`ローカル変数をメンバ変数に変換する手順`
>1. ローカル変数の宣言を切り取る.<br>
 変数の宣言と初期化を同時に行っている場合,<br>
 まず宣言と初期化に分ける.<br>
 その後,宣言だけを切り取る.
>2. 切り取った変数宣言を,<br>
 移動先のクラス定義に貼り付ける.
>3. プログラムをビルドして,<br>
 正しく動作することを確認する.

`現時点で「要素をまたいで利用している」,`

`あるいは「今後,要素をまたいで利用する可能性がある」変数`
>* GLFWウィンドウオブジェクト(`window`)
>* ウィンドウタイトル文字列
>* OpenGLのオブジェクト管理番号<br>
(`vs`,`fs`,`prog3D`,`vbo`,`ibo`,`vao`)

`Main.cpp`
```diff
 #pragma region 描画ウィンドウの作成
-    GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
-    const std::string title = "OpenGLGame"; // ウィンドウタイトル
     // GLFWライブラリを使ってコンテキストの種類の設定
     // GLFW_TRUE : デバッグコンテキストの作成
     // GLFW_FALSE : 通常のコンテキストの作成
     glfwWindowHint
     (
```
```diff
 #pragma region シェーダのコンパイルとリンク
     // シェーダを読み込んでコンパイル
+    vs = 
         CompileShader
         (
             GL_VERTEX_SHADER,
             "Res/standard.vert"
         );
+    fs = 
         CompileShader
         (
             GL_FRAGMENT_SHADER,
             "Res/standard.frag"
         );
 
     // プログラムオブジェクトの作成
+    prog3D = glCreateProgram();
     // 頂点シェーダの指定
     glAttachShader
     (
```

`Engine.h`
```diff
 	int Run();
 
 private:
+	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
+	const std::string title = "OpenGLGame"; // ウィンドウタイトル
+	GLuint vs = 0;							// 頂点シェーダの管理番号
+	GLuint fs = 0;							// フラグメントシェーダの管理番号
+	GLuint prog3D = 0;						// シェーダプログラムの管理番号
 };
 
 #endif						// ENGINE_H_INCLUDED(インクルードガード)
```

## 課題01
内容

Main.cppの

頂点バッファのvbo,ibo,vao

テクスチャのtexを

ローカル変数 -> メンバ変数にしなさい.

`Main.cpp`
```diff
     // -X(左の面)
     { {-1, 1, 1 }, { 0, 0 } },
     { {-1, 1,-1 }, { 1, 0 } },
     { {-1,-1,-1 }, { 1, 1 } },
     { {-1,-1, 1 }, { 0, 1 } },
 };
-GLuint vbo = 0; // 頂点バッファの管理番号
 // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
 glCreateBuffers
 (
```
```diff
     20,21,22,22,23,20,
 };
-GLuint ibo = 0; // インデックスバッファの管理番号
 // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
 glCreateBuffers
 (
```
```diff
 #pragma region 頂点データ形式の設定
-    GLuint vao = 0; // 頂点属性配列の管理番号
     // 頂点属性オブジェクトの作成
     glCreateVertexArrays
     (
```
```diff
 #pragma endregion
 
 #pragma region テクスチャの作成
+    tex = LoadTexture("Res/box.tga");
 #pragma endregion
 
 #pragma region メインループの定義
```

`Engine.h`
```diff
 	int Run();
 
 private:
 	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
 	const std::string title = "OpenGLGame"; // ウィンドウタイトル
 	GLuint vs = 0;							// 頂点シェーダの管理番号
 	GLuint fs = 0;							// フラグメントシェーダの管理番号
 	GLuint prog3D = 0;						// シェーダプログラムの管理番号
+	GLuint vbo = 0;							// 頂点バッファの管理番号
+	GLuint ibo = 0;							// インデックスバッファの管理番号
+	GLuint vao = 0;							// 頂点属性配列の管理番号
+	GLuint tex = 0;                         // テクスチャの作成
 };
 
 #endif						// ENGINE_H_INCLUDED(インクルードガード)
```

### 1-3.GameObjectクラスの独立化
`Main.cpp`
```diff
 #pragma region 頂点データをGPUメモリにコピー
     // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
-    struct vec2 { float x, y; };
-    struct vec3 { float x, y, z; };
     struct Vertex
     {
```
```diff
 #pragma region 物体のパラメータ
-    class GameObject
-    {
-    public:
-        vec3 position = { 0, 0, 0 };    // 物体の位置
-        vec3 rotation = { 0, 0, 0 };    // 物体の回転角度
-        vec3 scale = { 1,1,1 };         // 物体の拡大率
-        float color[4] = { 1, 1, 1, 1 };// 物体の色
-    };
 
     // カメラオブジェクト
     GameObject camera;
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\VecMath.hを入力

->追加をクリック

`VecMath.h`
```diff
+/**
+* @file VecMath.h
+*/
+#ifndef VECMATH_H_INCLUDED
+#define VECMATH_H_INCLUDED
+#include <cmath>
+
+struct vec2
+{
+    float x, y;
+};
+struct vec3
+{
+    float x, y, z;
+};
+
+#endif // !VECMATH_H_INCLUDED
```

`GameObject.h`
```diff
+/**
+* @file GameObject.h
+*/
+#ifndef GAMEOBJECT_H_INCLUDED
+#define GAMEOBJECT_H_INCLUDED
+#include "VecMath.h"
+
+class GameObject
+{
+public:
+    vec3 position = { 0, 0, 0 };    // 物体の位置
+    vec3 rotation = { 0, 0, 0 };    // 物体の回転角度
+    vec3 scale = { 1,1,1 };         // 物体の拡大率
+    float color[4] = { 1, 1, 1, 1 };// 物体の色
+};
+
+#endif // !GAMEOBJECT_H_INCLUDED
```

`Engine.h`
```diff
 /**
 * @file Engine.h
 */
 #ifndef ENGINE_H_INCLUDED	// インクルードガード
 #define ENGINE_H_INCLUDED	// インクルードガード
 #include "glad/glad.h"		// GLADライブラリの関数が定義されているヘッダファイル
+#include "GameObject.h"
 #include <GLFW/glfw3.h>		// GLFWライブラリの関数が定義されているヘッダファイル
 #include <string>			// 文字列型や文字列操作関数などが定義されているヘッダファイル
```

## 課題02
内容

Main.cppの

camera,box0,box1を

メンバ変数にしなさい

`Main.cpp`
```diff
+#pragma region boxのパラメータ
-    // カメラオブジェクト
-	 GameObject camera;						// カメラオブジェクト
-	 GameObject box0;
-	 GameObject box1;
-
     box0.scale = { 0.2f,0.2f,0.2f };
     box0.position = { 0.6f,0.6f,-1 };
 
     box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
     box1.scale = { 0.2f, 0.2f, 0.2f };
     box1.position = { 0, 0, -1 };
 #pragma endregion
```

`Engine.h`
```diff
 	GLuint tex = 0                          // テクスチャの作成
 
+	GameObject camera;						// カメラオブジェクト
+	GameObject box0;
+	GameObject box1;
 };
 
 #endif						// !ENGINE_H_INCLUDED(インクルードガード)
```

### 1-4.Run関数を3つのパートに分割後,Main.cpp -> Engine.cpp
`Engine.h`
```diff
 	int Run();
 
 private:
+	int Initialize();
+	void Update();
+	void Render();
 
 	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
```
```diff
 	GameObject box1;
+	GLsizei indexCount = 0;
 };
 
 #endif						// !ENGINE_H_INCLUDED(インクルードガード)
```

`Main.cpp`
```diff
 /// <summary>
 /// ゲームエンジンを実行する
 /// </summary>
 /// <returns>0 : 正常に実行が完了,
 /// 0以外 : エラーが発生</returns>
 int Engine::Run()
 {
+    const int result = Initialize();
+    if (result)
+    {
+        return result;
+    }
+    // ウィンドウの終了要求が来ていなかった(0)時,
+    // メインループ処理を続ける
+    // 引数 : GLFWwindowへのポインタ
+    while (!glfwWindowShouldClose(window))
+    {
+        Update();
+        Render();
+    }
+    // GLFWライブラリの終了
+    glfwTerminate();
+    return 0;
+}
+
+/// <summary>
+/// ゲームエンジンの初期化
+/// </summary>
+/// <returns>0 : 正常に初期化,
+/// 0以外 : エラーが発生した</returns>
+int Engine::Initialize()
+{
 #pragma region GLFWライブラリの初期化
     // 初期化に成功しなかった(!GLFW_TRUE)時,
     // 1を返して終了
     if (glfwInit() != GLFW_TRUE)
     {
```
```diff
     20,21,22,22,23,20,
 };
+indexCount = static_cast<GLsizei>(std::size(indexData));  // インデックス数
 // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
 glCreateBuffers
 (
```
```diff
     tex = LoadTexture("Res/box.tga");
 #pragma endregion
 
+    return 0; // 正常に初期化された
+}
+
+/// <summary>
+/// ゲームエンジンの状態の更新
+/// </summary>
+void Engine::Update()
+{
     // box0の回転
     box0.rotation.y += 0.0001f;
```
```diff
         camera.rotation.y += 0.0005f;
     }
+}
+
+/// <summary>
+/// ゲームエンジンの状態を描画
+/// </summary>
+void Engine::Render()
+{
         // バックバッファを消去するときに使用する色の指定
         glClearColor
         (
```
```diff
     &tex    // テクスチャ管理番号配列のアドレス
 );

 // 図形の描画
-const GLsizei indexCount = static_cast<GLsizei>(std::size(indexData));    // インデックス数
 glDrawElementsInstanced
 (
```
```diff
         // 「OSからの要求」の処理
         // (キーボードやマウスなどの状態を取得するなど)
         glfwPollEvents();
     }
-#pragma endregion
-
-#pragma region GLFWライブラリの終了
-    glfwTerminate();
-#pragma endregion
-
-    return 0;
-}
```

`Main.cpp`
```diff
     return engine.Run();
 }
 
-/// <summary>
-/// ゲームエンジンを実行する
-/// </summary>
-/// <returns>0 : 正常に実行が完了,
-/// 0以外 : エラーが発生</returns>
-int Engine::Run()
-{
-    const int result = Initialize();
-    if (result)
-    {
-        return result;
-    }
-    // ウィンドウの終了要求が来ていなかった(0)時,
-    // メインループ処理を続ける
-    // 引数 : GLFWwindowへのポインタ
-    while (!glfwWindowShouldClose(window))
-    {
-        Update();
-        Render();
-    }
-    // GLFWライブラリの終了
-    glfwTerminate();
-    return 0;
-}
-
-/// <summary>
-/// ゲームエンジンの初期化
-/// </summary>
-/// <returns>0 : 正常に初期化,
-/// 0以外 : エラーが発生した</returns>
-int Engine::Initialize()
-{
-#pragma region GLFWライブラリの初期化
-    // 初期化に成功しなかった(!GLFW_TRUE)時,
-    // 1を返して終了
-    if (glfwInit() != GLFW_TRUE)
-    {
-        return 1; // 初期化失敗
-    }
-#pragma endregion
-
-#pragma region 描画ウィンドウの作成
-    // GLFWライブラリを使ってコンテキストの種類の設定
-    // GLFW_TRUE : デバッグコンテキストの作成
-    // GLFW_FALSE : 通常のコンテキストの作成
-    glfwWindowHint
-    (
-        GLFW_OPENGL_DEBUG_CONTEXT,  // ヒントの種類
-        GLFW_TRUE                   // ヒントに設定する値
-    );
-    window =
-        //グラフィックス描画用ウィンドウの作成
-        glfwCreateWindow
-        (
-            1280,           // ウィンドウの幅
-            720,            // ウィンドウの高さ
-            title.c_str(),  // タイトルバーに表示する文字列
-            nullptr,        // 表示するモニタの選択
-            nullptr         // リソースを共有するウィンドウ
-        );
-    // ウィンドウの作成に成功しなかった(nullptr)時,
-    // 1を返して終了
-    if (!window)
-    {
-        // GLFWライブラリの終了
-        glfwTerminate();
-        return 1;   // ウィンドウ作成失敗
-    }
-#pragma endregion
-
-#pragma region OpenGL関数の導入
-    // OpenGLコンテキストの作成
-    // 引数 : GLFWウィンドウオブジェクトのアドレス
-    glfwMakeContextCurrent(window);
-
-    // gladLoadGLLoader : 必要な関数のアドレスを全て取得
-    // glfwGetProcAddress : OpenGL関数名に対応する関数のアドレスを返す
-    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
-    {
-        // GLFWライブラリの終了
-        glfwTerminate();
-        return 1; // アドレス取得失敗
-    }
-#pragma endregion
-
-#pragma region メッセージコールバックの設定
-    glDebugMessageCallback
-    (
-        DebugCallback,  // 呼び出す関数のアドレス
-        nullptr         // 関数のuserParam引数に渡される値
-    );
-#pragma endregion
-
-#pragma region シェーダのコンパイルとリンク
-    // シェーダを読み込んでコンパイル
-    vs =
-        CompileShader
-        (
-            GL_VERTEX_SHADER,
-            "Res/standard.vert"
-        );
-    fs =
-        CompileShader
-        (
-            GL_FRAGMENT_SHADER,
-            "Res/standard.frag"
-        );
-
-    // プログラムオブジェクトの作成
-    prog3D = glCreateProgram();
-    // 頂点シェーダの指定
-    glAttachShader
-    (
-        prog3D, // プログラムオブジェクトの管理番号
-        vs      // 頂点シェーダの管理番号
-    );
-    // フラグメントシェーダの指定
-    glAttachShader
-    (
-        prog3D, // プログラムオブジェクトの管理番号
-        fs      // フラグメントシェーダの管理番号
-    );
-    // 指定した2つのシェーダのリンク
-    // 引数 : プログラムオブジェクトの管理番号
-    glLinkProgram(prog3D);
-#pragma endregion
-
-#pragma region 頂点データをGPUメモリにコピー
-    // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
-    struct Vertex
-    {
-        vec3 position; // 頂点座標
-        vec2 texcoord; // テクスチャ座標
-    };
-    const Vertex vertexData[] =
-    {
-        // +Z(手前の面)
-        { {-1,-1, 1 }, { 0, 0 } },
-        { { 1,-1, 1 }, { 1, 0 } },
-        { { 1, 1, 1 }, { 1, 1 } },
-        { {-1, 1, 1 }, { 0, 1 } },
-
-        // -Z(奥の面)
-        { { 1,-1,-1 }, { 0, 0 } },
-        { {-1,-1,-1 }, { 1, 0 } },
-        { {-1, 1,-1 }, { 1, 1 } },
-        { { 1, 1,-1 }, { 0, 1 } },
-
-        // +Y(上の面)
-        { { 1, 1, 1 }, { 0, 0 } },
-        { { 1, 1,-1 }, { 1, 0 } },
-        { {-1, 1,-1 }, { 1, 1 } },
-        { {-1, 1, 1 }, { 0, 1 } },
-
-        // -Y(下の面)
-        { { 1,-1,-1 }, { 0, 0 } },
-        { { 1,-1, 1 }, { 1, 0 } },
-        { {-1,-1, 1 }, { 1, 1 } },
-        { {-1,-1,-1 }, { 0, 1 } },
-
-        // +X(右の面)
-        { { 1, 1, 1 }, { 0, 0 } },
-        { { 1, 1,-1 }, { 1, 0 } },
-        { { 1,-1,-1 }, { 1, 1 } },
-        { { 1,-1, 1 }, { 0, 1 } },
-
-        // -X(左の面)
-        { {-1, 1, 1 }, { 0, 0 } },
-        { {-1, 1,-1 }, { 1, 0 } },
-        { {-1,-1,-1 }, { 1, 1 } },
-        { {-1,-1, 1 }, { 0, 1 } },
-    };
-    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
-    glCreateBuffers
-    (
-        1,      // 作成するオブジェクト数
-        &vbo    // 頂点バッファの管理番号を格納する配列
-    );
-    // GPUメモリを確保のち,データをコピー
-    // データのアドレスがnullptrなら,
-    // GPUメモリの確保のみ
-    glNamedBufferStorage
-    (
-        vbo,                // 頂点バッファの管理番号
-        sizeof(vertexData), // コピーするバイト数
-        vertexData,         // コピーするデータのアドレス
-        0                   // 各種フラグ
-    );
-#pragma endregion
-
-#pragma region インデックスデータをGPUメモリにコピー
-    // インデックスデータ(図形を構成する頂点番号)
-    const GLushort indexData[] =
-    {
-        0, 1, 2, 2, 3, 0,
-        4, 5, 6, 6, 7, 4,
-        8, 9,10,10,11, 8,
-        12,13,14,14,15,12,
-        16, 17,18,18,19, 16,
-        20,21,22,22,23,20,
-    };
-    indexCount = static_cast<GLsizei>(std::size(indexData));  // インデックス数
-    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
-    glCreateBuffers
-    (
-        1,      // 作成するオブジェクト数
-        &ibo    // インデックスバッファの管理番号
-    );
-    // GPUメモリを確保のち,データをコピー
-    // データのアドレスがnullptrなら,
-    // GPUメモリの確保のみ
-    glNamedBufferStorage
-    (
-        ibo,               // インデックスバッファの管理番号
-        sizeof(indexData), // コピーするバイト数
-        indexData,         // コピーするデータのアドレス
-        0                  // 各種フラグ
-    );
-#pragma endregion
-
-#pragma region 頂点データ形式の設定
-    // 頂点属性オブジェクトの作成
-    glCreateVertexArrays
-    (
-        1,    // 作成するオブジェクト数
-        &vao  // 頂点属性配列の管理番号を格納する配列
-    );
-    // VAOをOpenGLコンテキストにバインド(割り当てる)
-    // 引数 : 割り当てる頂点属性配列の管理番号
-    glBindVertexArray(vao);
-
-    // IBOをOpenGLコンテキストとVAOの両方にバインド
-    // (VAOがOpenGLコンテキストにバインドされているため)
-    // GL_ELEMENT_ARRAY_BUFFER
-    // ->OpenGLコンテキストにVAOが割り当てられている場合,
-    //   バッファオブジェクトをVAOにも割り当てる
-    glBindBuffer
-    (
-        GL_ELEMENT_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
-        ibo                       // 割り当てるインデックスバッファの管理番号
-    );
-
-    // VBOをOpenGLコンテキストにバインド(IBOと異なり,VAOにはバインドされない)
-    glBindBuffer
-    (
-        GL_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
-        vbo               // 割り当てる頂点バッファ管理番号
-    );
-
-    // 0番目の頂点属性の有効化
-    // 引数 : 有効にする頂点属性配列のインデックス
-    glEnableVertexAttribArray(0);
-
-    // 0番目の頂点属性の設定
-    // このとき,OpenGLコンテキストにバインドされているVBOが,
-    // 頂点属性にバインドされる
-    glVertexAttribPointer
-    (
-        0,              // 頂点属性配列のインデックス
-        3,              // データの要素数
-        GL_FLOAT,       // データの型
-        GL_FALSE,       // 正規化の有無
-        sizeof(Vertex), // 次のデータまでのバイト数
-        0               // 最初のデータの位置
-    );
-
-    // 1番目の頂点属性の有効化
-    // 引数 : 有効にする頂点属性配列のインデックス
-    glEnableVertexAttribArray(1);
-
-    // 1番目の頂点属性の設定
-    glVertexAttribPointer
-    (
-        1,                              // 頂点属性配列のインデックス
-        2,                              // データの要素数
-        GL_FLOAT,                       // データの型
-        GL_FALSE,                       // 正規化の有無
-        sizeof(Vertex),                 // 次のデータまでのバイト数
-        reinterpret_cast<const void*>   // 最初のデータの位置
-        (
-            // 構造体の先頭から特定のメンバまでの
-            // バイト数の計算
-            offsetof
-            (
-                Vertex,     // 構造体名
-                texcoord    // メンバ名
-            )
-            )
-    );
-#pragma endregion
-
-#pragma region boxのパラメータ
-    box0.scale = { 0.2f,0.2f,0.2f };
-    box0.position = { 0.6f,0.6f,-1 };
-
-    box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
-    box1.scale = { 0.2f, 0.2f, 0.2f };
-    box1.position = { 0, 0, -1 };
-#pragma endregion
-
-#pragma region テクスチャの作成
-    tex = LoadTexture("Res/box.tga");
-#pragma endregion
-
-    return 0; // 正常に初期化された
-}
-
-/// <summary>
-/// ゲームエンジンの状態の更新
-/// </summary>
-void Engine::Update()
-{
-    // box0の回転
-    box0.rotation.y += 0.0001f;
-
-    // glfwGetKey(GLFWウィンドウオブジェクトのアドレス,キー番号);
-    // GLFW_RELEASE : キー入力なし
-    // GLFW_PRESS   : キー入力あり
-    // カメラの移動
-    const float cameraSpeed = 0.0005f;
-    const float cameraCos = cos(camera.rotation.y);
-    const float cameraSin = sin(camera.rotation.y);
-    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
-    {
-        camera.position.x -= cameraSpeed * cameraCos;
-        camera.position.z -= cameraSpeed * -cameraSin;
-    }
-    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
-    {
-        camera.position.x += cameraSpeed * cameraCos;
-        camera.position.z += cameraSpeed * -cameraSin;
-    }
-    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
-    {
-        camera.position.x -= cameraSpeed * cameraSin;
-        camera.position.z -= cameraSpeed * cameraCos;
-    }
-    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
-    {
-        camera.position.x += cameraSpeed * cameraSin;
-        camera.position.z += cameraSpeed * cameraCos;
-    }
-
-    // カメラの回転
-    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
-    {
-        camera.rotation.y -= 0.0005f;
-    }
-    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
-    {
-        camera.rotation.y += 0.0005f;
-    }
-    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
-    //{
-    //  camera.rotation.x += 0.0005f;
-    //}
-    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
-    //{
-    //  camera.rotation.x -= 0.0005f;
-    //}
-}
-
-/// <summary>
-/// ゲームエンジンの状態を描画
-/// </summary>
-void Engine::Render()
-{
-        // バックバッファを消去するときに使用する色の指定
-        glClearColor
-        (
-            0.3f,   // R(赤)
-            0.6f,   // G(緑)
-            0.9f,   // B(青)
-            1.0f    // A(透明度)
-        );
-        // バックバッファのクリア
-        // GL_COLOR_BUFFER_BIT : カラーバッファ(色)
-        // GL_DEPTH_BUFFER_BIT : 深度バッファ(奥行き)
-        // GL_STENCIL_BUFFER_BIT : ステンシルバッファ(切り抜き)
-        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
-
-        // VAOをOpenGLコンテキストに割り当てる    
-        // 引数 : 割り当てる頂点属性配列の管理番号
-        glBindVertexArray(vao);
-
-        // 描画に使うシェーダの指定
-        glUseProgram(prog3D);
-
-        // フレームバッファの大きさを取得
-        int fbWidth, fbHeight;
-        glfwGetFramebufferSize
-        (
-            window,     // GLFWウィンドウオブジェクトのアドレス
-            &fbWidth,   // 描画ウィンドウの幅を格納する変数のアドレス
-            &fbHeight   // 描画ウィンドウの高さを格納する変数のアドレス
-        );
-
-        // ビューポートの設定
-        glViewport
-        (
-            0,          // 描画範囲の左下Xの座標
-            0,          // 描画範囲の左下のY座標
-            fbWidth,    // 描画範囲の幅
-            fbHeight    // 描画範囲の高さ
-        );
-
-        // アスペクト比と視野角の設定
-        const float aspectRatio =
-            static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
-        const float degFovY = 60; // 垂直視野角
-        const float radFovY = degFovY * 3.1415926535f / 180;
-        const float scaleFov = tan(radFovY / 2); // 視野角による拡大率
-        glProgramUniform2f
-        (
-            prog3D,                         // プログラムオブジェクトの管理番号
-            3,                              // 送り先ロケーション番号
-            1 / (aspectRatio * scaleFov),   // データ数
-            1 / scaleFov                    // データのアドレス
-        );
-
-        // カメラパラメータの設定
-        glProgramUniform3fv
-        (
-            prog3D,             // プログラムオブジェクトの管理番号
-            4,                  // 送り先ロケーション番号
-            1,                  // データ数
-            &camera.position.x  // データのアドレス
-        );
-        glProgramUniform2f
-        (
-            prog3D,                     // プログラムオブジェクトの管理番号
-            5,                          // 送り先ロケーション番号
-            sin(-camera.rotation.y),    // データ数
-            cos(-camera.rotation.y)     // データのアドレス
-        );
-
-        // 深度テストの有効化
-        // glEnable : 指定されたOpenGLコンテキストの有効化
-        // 引数 : 有効にする機能を表すマクロ定数
-        // GL_DEPTH_TEST : 深度テスト
-        glEnable(GL_DEPTH_TEST);
-
-        // 変数ユニフォームにデータワット
-        glProgramUniform4fv
-        (
-            prog3D,     // プログラムオブジェクトの管理番号
-            100,        // 送り先ロケーション番号
-            1,          // データ数
-            box0.color  // データのアドレス
-        );
-        glProgramUniform3fv
-        (
-            prog3D,         // プログラムオブジェクトの管理番号
-            0,              // 送り先ロケーション番号
-            1,              // データ数
-            &box0.scale.x   // データのアドレス
-        );
-        glProgramUniform3fv
-        (
-            prog3D,             // プログラムオブジェクトの管理番号
-            1,                  // 送り先ロケーション番号
-            1,                  // データ数
-            &box0.position.x    // データのアドレス
-        );
-        glProgramUniform2f
-        (
-            prog3D,
-            2,
-            sin(box0.rotation.y),
-            cos(box0.rotation.y)
-        );
-
-        // 描画に使うテクスチャを
-        // (テクスチャ・イメージ・ユニットに)割り当て
-        glBindTextures
-        (
-            0,      // 割り当て開始インデックス
-            1,      // 割り当てる個数
-            &tex    // テクスチャ管理番号配列のアドレス
-        );
-
-        // 図形の描画
-        glDrawElementsInstanced
-        (
-            GL_TRIANGLES,       // 基本図形の種類
-            indexCount,         // インデックスデータ数
-            GL_UNSIGNED_SHORT,  // インデックスデータの型
-            0,                  // インデックスデータの開始位置
-            1                   // 描画する図形の数
-        );
-
-        // ふたつめの図形の描画
-        glProgramUniform4fv
-        (
-            prog3D,     // プログラムオブジェクトの管理番号
-            100,        // 送り先ロケーション番号
-            1,          // データ数
-            box1.color  // データのアドレス
-        );
-        glProgramUniform3fv
-        (
-            prog3D,         // プログラムオブジェクトの管理番号
-            0,              // 送り先ロケーション番号
-            1,              // データ数
-            &box1.scale.x   // データのアドレス
-        );
-        glProgramUniform3fv
-        (
-            prog3D,             // プログラムオブジェクトの管理番号
-            1,                  // 送り先ロケーション番号
-            1,                  // データ数
-            &box1.position.x    // データのアドレス
-        );
-        glProgramUniform2f
-        (
-            prog3D,
-            2,
-            sin(box1.rotation.y),
-            cos(box1.rotation.y)
-        );
-        glDrawElementsInstanced
-        (
-            GL_TRIANGLES,       // 基本図形の種類
-            indexCount,         // インデックスデータ数
-            GL_UNSIGNED_SHORT,  // インデックスデータの型
-            0,                  // インデックスデータの開始位置
-            1                   // 描画する図形の数
-        );
-
-        // VAOの割り当てを解除
-        // 引数 : 割り当てる頂点属性配列の管理番号
-        glBindVertexArray(0);
-
-        // バックバッファの描画終了時,
-        // フロントバッファと役割の交換
-        // 引数 : GLFWwindowへのポインタ
-        glfwSwapBuffers(window);
-
-        // 「OSからの要求」の処理
-        // (キーボードやマウスなどの状態を取得するなど)
-        glfwPollEvents();
-}
```

`Engine.cpp`
```diff
 /**
 * @file Engine.cpp
 */
 #include "Engine.h"
 
+/// <summary>
+/// ゲームエンジンを実行する
+/// </summary>
+/// <returns>0 : 正常に実行が完了,
+/// 0以外 : エラーが発生</returns>
+int Engine::Run()
+{
+    const int result = Initialize();
+    if (result)
+    {
+        return result;
+    }
+    // ウィンドウの終了要求が来ていなかった(0)時,
+    // メインループ処理を続ける
+    // 引数 : GLFWwindowへのポインタ
+    while (!glfwWindowShouldClose(window))
+    {
+        Update();
+        Render();
+    }
+    // GLFWライブラリの終了
+    glfwTerminate();
+    return 0;
+}
+
+/// <summary>
+/// ゲームエンジンの初期化
+/// </summary>
+/// <returns>0 : 正常に初期化,
+/// 0以外 : エラーが発生した</returns>
+int Engine::Initialize()
+{
+#pragma region GLFWライブラリの初期化
+    // 初期化に成功しなかった(!GLFW_TRUE)時,
+    // 1を返して終了
+    if (glfwInit() != GLFW_TRUE)
+    {
+        return 1; // 初期化失敗
+    }
+#pragma endregion
+
+#pragma region 描画ウィンドウの作成
+    // GLFWライブラリを使ってコンテキストの種類の設定
+    // GLFW_TRUE : デバッグコンテキストの作成
+    // GLFW_FALSE : 通常のコンテキストの作成
+    glfwWindowHint
+    (
+        GLFW_OPENGL_DEBUG_CONTEXT,  // ヒントの種類
+        GLFW_TRUE                   // ヒントに設定する値
+    );
+    window =
+        //グラフィックス描画用ウィンドウの作成
+        glfwCreateWindow
+        (
+            1280,           // ウィンドウの幅
+            720,            // ウィンドウの高さ
+            title.c_str(),  // タイトルバーに表示する文字列
+            nullptr,        // 表示するモニタの選択
+            nullptr         // リソースを共有するウィンドウ
+        );
+    // ウィンドウの作成に成功しなかった(nullptr)時,
+    // 1を返して終了
+    if (!window)
+    {
+        // GLFWライブラリの終了
+        glfwTerminate();
+        return 1;   // ウィンドウ作成失敗
+    }
+#pragma endregion
+
+#pragma region OpenGL関数の導入
+    // OpenGLコンテキストの作成
+    // 引数 : GLFWウィンドウオブジェクトのアドレス
+    glfwMakeContextCurrent(window);
+
+    // gladLoadGLLoader : 必要な関数のアドレスを全て取得
+    // glfwGetProcAddress : OpenGL関数名に対応する関数のアドレスを返す
+    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
+    {
+        // GLFWライブラリの終了
+        glfwTerminate();
+        return 1; // アドレス取得失敗
+    }
+#pragma endregion
+
+#pragma region メッセージコールバックの設定
+    glDebugMessageCallback
+    (
+        DebugCallback,  // 呼び出す関数のアドレス
+        nullptr         // 関数のuserParam引数に渡される値
+    );
+#pragma endregion
+
+#pragma region シェーダのコンパイルとリンク
+    // シェーダを読み込んでコンパイル
+    vs =
+        CompileShader
+        (
+            GL_VERTEX_SHADER,
+            "Res/standard.vert"
+        );
+    fs =
+        CompileShader
+        (
+            GL_FRAGMENT_SHADER,
+            "Res/standard.frag"
+        );
+
+    // プログラムオブジェクトの作成
+    prog3D = glCreateProgram();
+    // 頂点シェーダの指定
+    glAttachShader
+    (
+        prog3D, // プログラムオブジェクトの管理番号
+        vs      // 頂点シェーダの管理番号
+    );
+    // フラグメントシェーダの指定
+    glAttachShader
+    (
+        prog3D, // プログラムオブジェクトの管理番号
+        fs      // フラグメントシェーダの管理番号
+    );
+    // 指定した2つのシェーダのリンク
+    // 引数 : プログラムオブジェクトの管理番号
+    glLinkProgram(prog3D);
+#pragma endregion
+
+#pragma region 頂点データをGPUメモリにコピー
+    // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
+    struct Vertex
+    {
+        vec3 position; // 頂点座標
+        vec2 texcoord; // テクスチャ座標
+    };
+    const Vertex vertexData[] =
+    {
+        // +Z(手前の面)
+        { {-1,-1, 1 }, { 0, 0 } },
+        { { 1,-1, 1 }, { 1, 0 } },
+        { { 1, 1, 1 }, { 1, 1 } },
+        { {-1, 1, 1 }, { 0, 1 } },
+
+        // -Z(奥の面)
+        { { 1,-1,-1 }, { 0, 0 } },
+        { {-1,-1,-1 }, { 1, 0 } },
+        { {-1, 1,-1 }, { 1, 1 } },
+        { { 1, 1,-1 }, { 0, 1 } },
+
+        // +Y(上の面)
+        { { 1, 1, 1 }, { 0, 0 } },
+        { { 1, 1,-1 }, { 1, 0 } },
+        { {-1, 1,-1 }, { 1, 1 } },
+        { {-1, 1, 1 }, { 0, 1 } },
+
+        // -Y(下の面)
+        { { 1,-1,-1 }, { 0, 0 } },
+        { { 1,-1, 1 }, { 1, 0 } },
+        { {-1,-1, 1 }, { 1, 1 } },
+        { {-1,-1,-1 }, { 0, 1 } },
+
+        // +X(右の面)
+        { { 1, 1, 1 }, { 0, 0 } },
+        { { 1, 1,-1 }, { 1, 0 } },
+        { { 1,-1,-1 }, { 1, 1 } },
+        { { 1,-1, 1 }, { 0, 1 } },
+
+        // -X(左の面)
+        { {-1, 1, 1 }, { 0, 0 } },
+        { {-1, 1,-1 }, { 1, 0 } },
+        { {-1,-1,-1 }, { 1, 1 } },
+        { {-1,-1, 1 }, { 0, 1 } },
+    };
+    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
+    glCreateBuffers
+    (
+        1,      // 作成するオブジェクト数
+        &vbo    // 頂点バッファの管理番号を格納する配列
+    );
+    // GPUメモリを確保のち,データをコピー
+    // データのアドレスがnullptrなら,
+    // GPUメモリの確保のみ
+    glNamedBufferStorage
+    (
+        vbo,                // 頂点バッファの管理番号
+        sizeof(vertexData), // コピーするバイト数
+        vertexData,         // コピーするデータのアドレス
+        0                   // 各種フラグ
+    );
+#pragma endregion
+
+#pragma region インデックスデータをGPUメモリにコピー
+    // インデックスデータ(図形を構成する頂点番号)
+    const GLushort indexData[] =
+    {
+        0, 1, 2, 2, 3, 0,
+        4, 5, 6, 6, 7, 4,
+        8, 9,10,10,11, 8,
+        12,13,14,14,15,12,
+        16, 17,18,18,19, 16,
+        20,21,22,22,23,20,
+    };
+    indexCount = static_cast<GLsizei>(std::size(indexData));  // インデックス数
+    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
+    glCreateBuffers
+    (
+        1,      // 作成するオブジェクト数
+        &ibo    // インデックスバッファの管理番号
+    );
+    // GPUメモリを確保のち,データをコピー
+    // データのアドレスがnullptrなら,
+    // GPUメモリの確保のみ
+    glNamedBufferStorage
+    (
+        ibo,               // インデックスバッファの管理番号
+        sizeof(indexData), // コピーするバイト数
+        indexData,         // コピーするデータのアドレス
+        0                  // 各種フラグ
+    );
+#pragma endregion
+
+#pragma region 頂点データ形式の設定
+    // 頂点属性オブジェクトの作成
+    glCreateVertexArrays
+    (
+        1,    // 作成するオブジェクト数
+        &vao  // 頂点属性配列の管理番号を格納する配列
+    );
+    // VAOをOpenGLコンテキストにバインド(割り当てる)
+    // 引数 : 割り当てる頂点属性配列の管理番号
+    glBindVertexArray(vao);
+
+    // IBOをOpenGLコンテキストとVAOの両方にバインド
+    // (VAOがOpenGLコンテキストにバインドされているため)
+    // GL_ELEMENT_ARRAY_BUFFER
+    // ->OpenGLコンテキストにVAOが割り当てられている場合,
+    //   バッファオブジェクトをVAOにも割り当てる
+    glBindBuffer
+    (
+        GL_ELEMENT_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
+        ibo                       // 割り当てるインデックスバッファの管理番号
+    );
+
+    // VBOをOpenGLコンテキストにバインド(IBOと異なり,VAOにはバインドされない)
+    glBindBuffer
+    (
+        GL_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
+        vbo               // 割り当てる頂点バッファ管理番号
+    );
+
+    // 0番目の頂点属性の有効化
+    // 引数 : 有効にする頂点属性配列のインデックス
+    glEnableVertexAttribArray(0);
+
+    // 0番目の頂点属性の設定
+    // このとき,OpenGLコンテキストにバインドされているVBOが,
+    // 頂点属性にバインドされる
+    glVertexAttribPointer
+    (
+        0,              // 頂点属性配列のインデックス
+        3,              // データの要素数
+        GL_FLOAT,       // データの型
+        GL_FALSE,       // 正規化の有無
+        sizeof(Vertex), // 次のデータまでのバイト数
+        0               // 最初のデータの位置
+    );
+
+    // 1番目の頂点属性の有効化
+    // 引数 : 有効にする頂点属性配列のインデックス
+    glEnableVertexAttribArray(1);
+
+    // 1番目の頂点属性の設定
+    glVertexAttribPointer
+    (
+        1,                              // 頂点属性配列のインデックス
+        2,                              // データの要素数
+        GL_FLOAT,                       // データの型
+        GL_FALSE,                       // 正規化の有無
+        sizeof(Vertex),                 // 次のデータまでのバイト数
+        reinterpret_cast<const void*>   // 最初のデータの位置
+        (
+            // 構造体の先頭から特定のメンバまでの
+            // バイト数の計算
+            offsetof
+            (
+                Vertex,     // 構造体名
+                texcoord    // メンバ名
+            )
+            )
+    );
+#pragma endregion
+
+#pragma region boxのパラメータ
+    box0.scale = { 0.2f,0.2f,0.2f };
+    box0.position = { 0.6f,0.6f,-1 };
+
+    box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
+    box1.scale = { 0.2f, 0.2f, 0.2f };
+    box1.position = { 0, 0, -1 };
+#pragma endregion
+
+#pragma region テクスチャの作成
+    tex = LoadTexture("Res/box.tga");
+#pragma endregion
+
+    return 0; // 正常に初期化された
+}
+
+/// <summary>
+/// ゲームエンジンの状態の更新
+/// </summary>
+void Engine::Update()
+{
+    // box0の回転
+    box0.rotation.y += 0.0001f;
+
+    // glfwGetKey(GLFWウィンドウオブジェクトのアドレス,キー番号);
+    // GLFW_RELEASE : キー入力なし
+    // GLFW_PRESS   : キー入力あり
+    // カメラの移動
+    const float cameraSpeed = 0.0005f;
+    const float cameraCos = cos(camera.rotation.y);
+    const float cameraSin = sin(camera.rotation.y);
+    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
+    {
+        camera.position.x -= cameraSpeed * cameraCos;
+        camera.position.z -= cameraSpeed * -cameraSin;
+    }
+    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
+    {
+        camera.position.x += cameraSpeed * cameraCos;
+        camera.position.z += cameraSpeed * -cameraSin;
+    }
+    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
+    {
+        camera.position.x -= cameraSpeed * cameraSin;
+        camera.position.z -= cameraSpeed * cameraCos;
+    }
+    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
+    {
+        camera.position.x += cameraSpeed * cameraSin;
+        camera.position.z += cameraSpeed * cameraCos;
+    }
+
+    //// マウスの入力の取得
+    //double mouseX, mouseY, mouseBeforeX, mouseBeforeY;
+    //mouseBeforeX = mouseX;
+    //mouseBeforeY = mouseY;
+    //glfwGetCursorPos
+    //(
+    //  window,
+    //  &mouseX,
+    //  &mouseY
+    // );
+    //if (mouseBeforeX != mouseX)
+    //{
+    //    camera.rotation.y += 0.0005f * mouseX - mouseBeforeX;
+    //}
+    //if (mouseBeforeY != mouseY)
+    //{
+    //    camera.rotation.x += 0.0005f * mouseY - mouseBeforeY;
+    //}
+
+    // カメラの回転
+    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
+    {
+        camera.rotation.y -= 0.0005f;
+    }
+    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
+    {
+        camera.rotation.y += 0.0005f;
+    }
+    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
+    //{
+    //  camera.rotation.x += 0.0005f;
+    //}
+    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
+    //{
+    //  camera.rotation.x -= 0.0005f;
+    //}
+}
+
+/// <summary>
+/// ゲームエンジンの状態を描画
+/// </summary>
+void Engine::Render()
+{
+    // バックバッファを消去するときに使用する色の指定
+    glClearColor
+    (
+        0.3f,   // R(赤)
+        0.6f,   // G(緑)
+        0.9f,   // B(青)
+        1.0f    // A(透明度)
+    );
+    // バックバッファのクリア
+    // GL_COLOR_BUFFER_BIT : カラーバッファ(色)
+    // GL_DEPTH_BUFFER_BIT : 深度バッファ(奥行き)
+    // GL_STENCIL_BUFFER_BIT : ステンシルバッファ(切り抜き)
+    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
+
+    // VAOをOpenGLコンテキストに割り当てる    
+    // 引数 : 割り当てる頂点属性配列の管理番号
+    glBindVertexArray(vao);
+
+    // 描画に使うシェーダの指定
+    glUseProgram(prog3D);
+
+    // フレームバッファの大きさを取得
+    int fbWidth, fbHeight;
+    glfwGetFramebufferSize
+    (
+        window,     // GLFWウィンドウオブジェクトのアドレス
+        &fbWidth,   // 描画ウィンドウの幅を格納する変数のアドレス
+        &fbHeight   // 描画ウィンドウの高さを格納する変数のアドレス
+    );
+
+    // ビューポートの設定
+    glViewport
+    (
+        0,          // 描画範囲の左下Xの座標
+        0,          // 描画範囲の左下のY座標
+        fbWidth,    // 描画範囲の幅
+        fbHeight    // 描画範囲の高さ
+    );
+
+    // アスペクト比と視野角の設定
+    const float aspectRatio =
+        static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
+    const float degFovY = 60; // 垂直視野角
+    const float radFovY = degFovY * 3.1415926535f / 180;
+    const float scaleFov = tan(radFovY / 2); // 視野角による拡大率
+    glProgramUniform2f
+    (
+        prog3D,                         // プログラムオブジェクトの管理番号
+        3,                              // 送り先ロケーション番号
+        1 / (aspectRatio * scaleFov),   // データ数
+        1 / scaleFov                    // データのアドレス
+    );
+
+    // カメラパラメータの設定
+    glProgramUniform3fv
+    (
+        prog3D,             // プログラムオブジェクトの管理番号
+        4,                  // 送り先ロケーション番号
+        1,                  // データ数
+        &camera.position.x  // データのアドレス
+    );
+    glProgramUniform2f
+    (
+        prog3D,                     // プログラムオブジェクトの管理番号
+        5,                          // 送り先ロケーション番号
+        sin(-camera.rotation.y),    // データ数
+        cos(-camera.rotation.y)     // データのアドレス
+    );
+
+    // 深度テストの有効化
+    // glEnable : 指定されたOpenGLコンテキストの有効化
+    // 引数 : 有効にする機能を表すマクロ定数
+    // GL_DEPTH_TEST : 深度テスト
+    glEnable(GL_DEPTH_TEST);
+
+    // 変数ユニフォームにデータワット
+    glProgramUniform4fv
+    (
+        prog3D,     // プログラムオブジェクトの管理番号
+        100,        // 送り先ロケーション番号
+        1,          // データ数
+        box0.color  // データのアドレス
+    );
+    glProgramUniform3fv
+    (
+        prog3D,         // プログラムオブジェクトの管理番号
+        0,              // 送り先ロケーション番号
+        1,              // データ数
+        &box0.scale.x   // データのアドレス
+    );
+    glProgramUniform3fv
+    (
+        prog3D,             // プログラムオブジェクトの管理番号
+        1,                  // 送り先ロケーション番号
+        1,                  // データ数
+        &box0.position.x    // データのアドレス
+    );
+    glProgramUniform2f
+    (
+        prog3D,
+        2,
+        sin(box0.rotation.y),
+        cos(box0.rotation.y)
+    );
+
+    // 描画に使うテクスチャを
+    // (テクスチャ・イメージ・ユニットに)割り当て
+    glBindTextures
+    (
+        0,      // 割り当て開始インデックス
+        1,      // 割り当てる個数
+        &tex    // テクスチャ管理番号配列のアドレス
+    );
+
+    // 図形の描画
+    glDrawElementsInstanced
+    (
+        GL_TRIANGLES,       // 基本図形の種類
+        indexCount,         // インデックスデータ数
+        GL_UNSIGNED_SHORT,  // インデックスデータの型
+        0,                  // インデックスデータの開始位置
+        1                   // 描画する図形の数
+    );
+
+    // ふたつめの図形の描画
+    glProgramUniform4fv
+    (
+        prog3D,     // プログラムオブジェクトの管理番号
+        100,        // 送り先ロケーション番号
+        1,          // データ数
+        box1.color  // データのアドレス
+    );
+    glProgramUniform3fv
+    (
+        prog3D,         // プログラムオブジェクトの管理番号
+        0,              // 送り先ロケーション番号
+        1,              // データ数
+        &box1.scale.x   // データのアドレス
+    );
+    glProgramUniform3fv
+    (
+        prog3D,             // プログラムオブジェクトの管理番号
+        1,                  // 送り先ロケーション番号
+        1,                  // データ数
+        &box1.position.x    // データのアドレス
+    );
+    glProgramUniform2f
+    (
+        prog3D,
+        2,
+        sin(box1.rotation.y),
+        cos(box1.rotation.y)
+    );
+    glDrawElementsInstanced
+    (
+        GL_TRIANGLES,       // 基本図形の種類
+        indexCount,         // インデックスデータ数
+        GL_UNSIGNED_SHORT,  // インデックスデータの型
+        0,                  // インデックスデータの開始位置
+        1                   // 描画する図形の数
+    );
+
+    // VAOの割り当てを解除
+    // 引数 : 割り当てる頂点属性配列の管理番号
+    glBindVertexArray(0);
+
+    // バックバッファの描画終了時,
+    // フロントバッファと役割の交換
+    // 引数 : GLFWwindowへのポインタ
+    glfwSwapBuffers(window);
+
+    // 「OSからの要求」の処理
+    // (キーボードやマウスなどの状態を取得するなど)
+    glfwPollEvents();
+}
```

## 課題03
内容

DebugCallback,CompileShader,

LoadTexture関数を,Main.cppから

Engine.cppの先頭(インクルード文の直後)に

移動しなさい.

## 課題04
内容

Engine.cppに必要なインクルード文を追加して,

エラーなくビルドできるようにしなさい.

`Engine.cpp`
```diff
 /**
 * @file Engine.cpp
 */
+#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
 #include "Engine.h"
+#include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル
+
+#pragma warning(push)
+#pragma warning(disable:4005)
+#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
+#pragma warning(pop)
+
+#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
+#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル
+
+/// <summary>
+/// OpenGLからのメッセージを処理するコールバック関数
+/// </summary>
+/// <param name="source">メッセージの発信者(OpenGL,Windows,シェーダなど)</param>
+/// <param name="type">メッセージの種類(エラー,警告など)</param>
+/// <param name="id">メッセージを一位に識別する値</param>
+/// <param name="severity">メッセージの重要度(高,中,低,最低)</param>
+/// <param name="length">メッセージの文字数. 負数ならメッセージは0終端されている</param>
+/// <param name="message">メッセージ本体</param>
+/// <param name="userParam">コールバック設定時に指定したポインタ</param>
+void APIENTRY DebugCallback
+(
+    GLenum source,
+    GLenum type,
+    GLuint id,
+    GLenum severity,
+    GLsizei length,
+    const GLchar* message,
+    const void* userParam
+)
+{
+  std::string s;
+  // メッセージの文字数が
+  // 負数の時,0終端("\0")
+  if (length < 0)
+  {
+    s = message;
+  }
+  // メッセージの読み込み
+  else
+  {
+    s.assign(message, message + length);
+  }
+  s += '\n'; // メッセージには改行がないので追加する
+  // 出力ウィンドウへの表示
+  // 引数 : 出力する文字列(エラーメッセージ)
+  OutputDebugStringA(s.c_str());
+}
+
+/// <summary>
+/// シェーダファイルを読み込んでコンパイルする
+/// </summary>
+/// <param name="type">シェーダの種類</param>
+/// <param name="filename">シェーダファイル名</param>
+/// <returns>シェーダの管理番号</returns>
+GLuint CompileShader
+(
+    GLenum type,
+    const char* filename
+)
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
+        return 0;
+    }
+
+    // ファイルサイズの取得
+    // 引数 : ファイルパス
+    const size_t filesize = std::filesystem::file_size(filename);
+    // ファイルサイズ分の領域を用意
+    std::vector<char> buffer(filesize);
+    // ファイルの読み込み
+    file.read
+    (
+        buffer.data(),  // バッファアドレス
+        filesize        // 読み込むバイト数
+    );
+    // ファイルを閉じる
+    file.close();
+
+    const char* source[] = { buffer.data() };       // bufferの先頭アドレス
+    const GLint length[] = { int(buffer.size()) };  // bufferのサイズ
+    // 「シェーダを管理するオブジェクト」の作成
+    // 引数 : 作成するシェーダの種類
+    const GLuint object = glCreateShader(type);
+    // シェーダ文字列の設定
+    glShaderSource
+    (
+        object, // シェーダの管理番号
+        1,      // 文字列の数
+        source, // 文字列の配列
+        length  // 文字列長の配列
+    );
+    // シェーダ文字列のコンパイル
+    // 引数 : シェーダの管理番号
+    glCompileShader(object);
+    return object;
+}
+
+/// <summary>
+/// テクスチャの読み込み
+/// </summary>
+/// <param name="filename">テクスチャファイル名</param>
+/// <returns>テクスチャの管理番号</returns>
+GLuint LoadTexture(const char* filename)
+{
+  // ファイルを開く
+  // std::ios::binary : バイナリモードの指定
+  std::ifstream file
+  (
+      filename,
+      std::ios::binary
+  );
+  // ファイルが正常に開けない(false)時,
+  // エラーメッセージを出力して,
+  // 0(オブジェクトが存在しない)を返して終了
+  if ( ! file)
+  {
+    char s[256];
+    snprintf
+    (
+        s,
+        256,
+        "[ERROR] %s: %sを開けません\n",
+        __func__,
+        filename
+    );
+    OutputDebugStringA(s);
+    return 0;
+  }
+
+  // ファイルサイズの取得
+// 引数 : ファイルパス
+  const size_t filesize = std::filesystem::file_size(filename);
+  // ファイルサイズ分の領域を用意
+  std::vector<uint8_t> buffer(filesize);
+  // ファイルの読み込み
+  file.read
+  (
+      reinterpret_cast<char*>(buffer.data()),   // バッファアドレス
+      filesize                                  // 読み込むバイト数
+  );
+  // ファイルを閉じる
+  file.close();
+
+  // ヘッダから情報を取得
+  const size_t tgaHeaderSize = 18; // ヘッダ情報のバイト数
+  const int width = buffer[12] + buffer[13] * 256;
+  const int height = buffer[14] + buffer[15] * 256;
+
+  GLuint object = 0; // テクスチャの管理番号
+  // テクスチャの作成
+  glCreateTextures
+  (
+      GL_TEXTURE_2D,    // テクスチャの種類
+      1,                // 作成する個数
+      &object           // 番号を格納する配列のアドレス
+  );
+  // 画像データ分のGPUメモリ領域の確保
+  glTextureStorage2D
+  (
+      object,   // テクスチャの管理番号
+      1,        // 作成するレベル数(ミップマップ)
+      GL_RGBA8, // ピクセル形式
+      width,    // 幅
+      height    // 高さ
+  );
+  // 画像データをGPUメモリにコピー
+  glTextureSubImage2D
+  (
+      object,                       // テクスチャの管理番号
+      0,                            // コピー先のレイヤー番号
+      0,                            // コピー先のX座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
+      0,                            // コピー先のY座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
+      width,                        // コピーする画像の幅
+      height,                       // コピーする画像の高さ
+      GL_BGRA,                      // ピクセルに含まれる要素と順序
+      GL_UNSIGNED_BYTE,             // 要素の型
+      buffer.data() + tgaHeaderSize // 画像データのアドレス
+  );
+  return object;
+}
 
 /// <summary>
 /// ゲームエンジンを実行する
 /// </summary>
 /// <returns>0 : 正常に実行が完了,
 /// 0以外 : エラーが発生</returns>
 int Engine::Run()
 {
```

## 課題05
内容

Main.cppのインクルード文について,

以下の手順に従って不要なインクルード文を削除しなさい.

1. 不要そうなインクルード文を削除する.
2. ビルドする.
3. ビルドに失敗したら,<br>
1で削除したインクルード文を元に戻し,2に戻る.
4. ビルドに成功したら1に戻る.
5. 削除できるインクルード文がなくなったら終了.

`Main.cpp`
```diff
 /**
 * @file Main.cpp
 */
-#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
 #include "Engine/Engine.h"
-#include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル
-
 #pragma warning(push)
 #pragma warning(disable:4005)
 #include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
 #pragma warning(pop)
-
-#include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
-#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
-#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
-#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル
-#include <stdio.h>      // 標準入出力ライブラリの関数が定義されているヘッダファイル
-
-/// <summary>
-/// OpenGLからのメッセージを処理するコールバック関数
-/// </summary>
-/// <param name="source">メッセージの発信者(OpenGL,Windows,シェーダなど)</param>
-/// <param name="type">メッセージの種類(エラー,警告など)</param>
-/// <param name="id">メッセージを一位に識別する値</param>
-/// <param name="severity">メッセージの重要度(高,中,低,最低)</param>
-/// <param name="length">メッセージの文字数. 負数ならメッセージは0終端されている</param>
-/// <param name="message">メッセージ本体</param>
-/// <param name="userParam">コールバック設定時に指定したポインタ</param>
-void APIENTRY DebugCallback
-(
-    GLenum source,
-    GLenum type,
-    GLuint id,
-    GLenum severity,
-    GLsizei length,
-    const GLchar* message,
-    const void* userParam
-)
-{
-  std::string s;
-  // メッセージの文字数が
-  // 負数の時,0終端("\0")
-  if (length < 0)
-  {
-    s = message;
-  }
-  // メッセージの読み込み
-  else
-  {
-    s.assign(message, message + length);
-  }
-  s += '\n'; // メッセージには改行がないので追加する
-  // 出力ウィンドウへの表示
-  // 引数 : 出力する文字列(エラーメッセージ)
-  OutputDebugStringA(s.c_str());
-}
-
-/// <summary>
-/// シェーダファイルを読み込んでコンパイルする
-/// </summary>
-/// <param name="type">シェーダの種類</param>
-/// <param name="filename">シェーダファイル名</param>
-/// <returns>シェーダの管理番号</returns>
-GLuint CompileShader
-(
-    GLenum type,
-    const char* filename
-)
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
-    // 引数 : ファイルパス
-    const size_t filesize = std::filesystem::file_size(filename);
-    // ファイルサイズ分の領域を用意
-    std::vector<char> buffer(filesize);
-    // ファイルの読み込み
-    file.read
-    (
-        buffer.data(),  // バッファアドレス
-        filesize        // 読み込むバイト数
-    );
-    // ファイルを閉じる
-    file.close();
-
-    const char* source[] = { buffer.data() };       // bufferの先頭アドレス
-    const GLint length[] = { int(buffer.size()) };  // bufferのサイズ
-    // 「シェーダを管理するオブジェクト」の作成
-    // 引数 : 作成するシェーダの種類
-    const GLuint object = glCreateShader(type);
-    // シェーダ文字列の設定
-    glShaderSource
-    (
-        object, // シェーダの管理番号
-        1,      // 文字列の数
-        source, // 文字列の配列
-        length  // 文字列長の配列
-    );
-    // シェーダ文字列のコンパイル
-    // 引数 : シェーダの管理番号
-    glCompileShader(object);
-    return object;
-}
-
-/// <summary>
-/// テクスチャの読み込み
-/// </summary>
-/// <param name="filename">テクスチャファイル名</param>
-/// <returns>テクスチャの管理番号</returns>
-GLuint LoadTexture(const char* filename)
-{
-  // ファイルを開く
-  // std::ios::binary : バイナリモードの指定
-  std::ifstream file
-  (
-      filename,
-      std::ios::binary
-  );
-  // ファイルが正常に開けない(false)時,
-  // エラーメッセージを出力して,
-  // 0(オブジェクトが存在しない)を返して終了
-  if ( ! file)
-  {
-    char s[256];
-    snprintf
-    (
-        s,
-        256,
-        "[ERROR] %s: %sを開けません\n",
-        __func__,
-        filename
-    );
-    OutputDebugStringA(s);
-    return 0;
-  }
-
-  // ファイルサイズの取得
-// 引数 : ファイルパス
-  const size_t filesize = std::filesystem::file_size(filename);
-  // ファイルサイズ分の領域を用意
-  std::vector<uint8_t> buffer(filesize);
-  // ファイルの読み込み
-  file.read
-  (
-      reinterpret_cast<char*>(buffer.data()),   // バッファアドレス
-      filesize                                  // 読み込むバイト数
-  );
-  // ファイルを閉じる
-  file.close();
-
-  // ヘッダから情報を取得
-  const size_t tgaHeaderSize = 18; // ヘッダ情報のバイト数
-  const int width = buffer[12] + buffer[13] * 256;
-  const int height = buffer[14] + buffer[15] * 256;
-
-  GLuint object = 0; // テクスチャの管理番号
-  // テクスチャの作成
-  glCreateTextures
-  (
-      GL_TEXTURE_2D,    // テクスチャの種類
-      1,                // 作成する個数
-      &object           // 番号を格納する配列のアドレス
-  );
-  // 画像データ分のGPUメモリ領域の確保
-  glTextureStorage2D
-  (
-      object,   // テクスチャの管理番号
-      1,        // 作成するレベル数(ミップマップ)
-      GL_RGBA8, // ピクセル形式
-      width,    // 幅
-      height    // 高さ
-  );
-  // 画像データをGPUメモリにコピー
-  glTextureSubImage2D
-  (
-      object,                       // テクスチャの管理番号
-      0,                            // コピー先のレイヤー番号
-      0,                            // コピー先のX座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
-      0,                            // コピー先のY座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
-      width,                        // コピーする画像の幅
-      height,                       // コピーする画像の高さ
-      GL_BGRA,                      // ピクセルに含まれる要素と順序
-      GL_UNSIGNED_BYTE,             // 要素の型
-      buffer.data() + tgaHeaderSize // 画像データのアドレス
-  );
-  return object;
-}
 
 /// <summary>
 /// エントリーポイント
 /// </summary>
 int WINAPI WinMain
 (
```

## 2.ゲームオブジェクトの管理
### 2-1.コンポーネントクラスの追加
プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\Component.hを入力

->追加をクリック

`Component.h`
```diff
/**
* @file Component.h
*/
#ifndef COMOPNENT_H_INCLUDED
#define COMOPNENT_H_INCLUDED
#include <memory>

// 先行宣言
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;
class Component;
using ComponentPtr = std::shared_ptr<Component>; // コンポーネントのポインタ

/// <summary>
/// コンポーネントの基底クラス
/// </summary>
class Component
{
    friend GameObject;
public:
    Component() = default;
    virtual ~Component() = default;

    // コンポーネントの所有者を取得
    GameObject* GetOwner() const
    {
        return owner;
    }

    // コンポーネントをゲームオブジェクトから削除する
    void Destroy()
    {
        isDestroyed = true;
    }

    // コンポーネントが破壊されていたらtrueを返す
    bool IsDestroyed() const
    {
        return isDestroyed;
    }

    // ゲームオブジェクトに追加された時に呼び出される
    virtual void Awake()
    {}

    // 最初のUpdateの直前で呼び出される
    virtual void Start()
    {}

    // 毎フレーム1回呼び出される
    virtual void Update(float deltaTime)
    {}

    // 衝突が起きたときに呼び出される
    virtual void OnCollision(
        const ComponentPtr& self, const ComponentPtr& other)
    {}

    // ゲームオブジェクトがエンジンから削除されるときに呼び出される
    virtual void OnDestroy()
    {}

private:
    GameObject* owner = nullptr; // このコンポーネントの所有者
    bool isStarted = false;      // Startが実行されたらtrueになる
    bool isDestroyed = false;    // Destroyが実行されたらtrueになる
};

#endif // !COMOPNENT_H_INCLUDED
```

`shared_ptr(C++11~)`

スマートポインタの一種.

ポインタのコピー数を記録して,

全てのコピーが削除されたときに

ポインタのdeleteを実行するクラス

->バグの可能性 減

`privateメンバ`

他のクラスから変数を参照したり,

操作できなくする

特定の操作を許可するために,

public関数やfriend指定が必要

->バグの可能性 減

### 2-2.ゲームオブジェクトクラスの拡張
プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\GameObject.hを入力

->追加をクリック

`GameObject.h`
```diff
 /**
 * @file GameObject.h
 */
+#ifndef GAMEOBJECT_H_INCLUDED
+#define GAMEOBJECT_H_INCLUDED
+#include "Component.h"
 #include "VecMath.h"
+#include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
+#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
+#include <memory>

+// 先行宣言
+class Engine;
+class GameObject;
+using GameObjectPtr = std::shared_ptr<GameObject>; // ゲームオブジェクトのポインタ
+using GameObjectList = std::vector<GameObjectPtr>; // ゲームオブジェクトポインタの配列

 /// <summary>
+/// ゲームに登場する様々なオブジェクトを表す基本クラス
 /// </summary>
+class GameObject
+{
+    friend Engine;
+public:
+    GameObject() = default;
+    virtual ~GameObject() = default;
+
+    // ゲームエンジンを取得
+    Engine* GetEngine() const
+    {
+        return engine;
+    }
+
+    // ゲームオブジェクトをエンジンから削除する
+    void Destroy()
+    {
+        isDestroyed = true;
+    }
+
+    // ゲームオブジェクトが破壊されていたらtrueを返す
+    bool IsDestroyed() const
+    {
+        return isDestroyed;
+    }
+
+    // ゲームオブジェクトにコンポーネントを追加する
+    template<typename T>
+    std::shared_ptr<T> AddComponent()
+    {
+        auto p = std::make_shared<T>();
+        p->owner = this;
+        components.push_back(p);
+        p->Awake();
+        return p;
+    }
+
+    // ゲームオブジェクトからコンポーネントを削除する
+    void RemoveDestroyedComponent();
+
+    // イベント制御
+    virtual void Start();
+    virtual void Update(float deltaTime);
+    virtual void OnCollision(
+        const ComponentPtr& self, const ComponentPtr& other);
+    virtual void OnDestroy();
+
+    std::string name;                   // オブジェクト名
+    vec3 position = { 0, 0, 0 };        // 物体の位置
+    vec3 rotation = { 0, 0, 0 };        // 物体の回転角度
+    vec3 scale = { 1,1,1 };             // 物体の拡大率
+    float color[4] = { 1, 1, 1, 1 };    // 物体の色
+
+private:
+    Engine* engine = nullptr;             // エンジンのアドレス
+    bool isDestroyed = false;             // 死亡フラグ
+    std::vector<ComponentPtr> components; // コンポーネント配列
+};

 #endif // !GAMEOBJECT_H_INCLUDED
```

プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\GameObject.cppを入力

->追加をクリック

`GameObject.cpp`
```diff
+/**
+* @file GameObject.cpp
+*/
+#include "GameObject.h"
+#include <algorithm>
+
+/// <summary>
+/// ゲームオブジェクトから削除予定のコンポーネントを削除する
+/// </summary>
+void GameObject::RemoveDestroyedComponent()
+{
+    if (components.empty())
+    {
+        return; // コンポーネントを持っていなければ何もしない
+    }
+
+    // 破棄予定の有無でコンポーネントを分ける
+    const auto iter =
+        std::stable_partition
+        (
+            components.begin(), // 範囲の先頭
+            components.end(),   // 範囲の終端
+            // 振り分け条件
+            [](const auto& p)
+            {
+                return !p->IsDestroyed();
+            });
+
+    // 破棄予定のコンポーネントを別の配列に移動
+    // 引数 : もとになるイテレータ
+    std::vector<ComponentPtr> destroyList(
+        std::move_iterator(iter),
+        std::move_iterator(components.end()));
+
+    // 配列から移動済みコンポーネントを削除
+    components.erase(iter, components.end());
+
+    // 破棄予定のコンポーネントのOnDestoryを実行
+    for (auto& e : destroyList)
+    {
+        e->OnDestroy();
+    }
+
+    // ここで実際にコンポーネントが削除される(destoyListの寿命が終わるため)
+}
+
+/// <summary>
+/// スタートイベント
+/// </summary>
+void GameObject::Start()
+{
+    for (auto& e : components)
+    {
+        if (!e->isStarted)
+        {
+            e->Start();
+            e->isStarted = true;
+        }
+    }
+}
+
+/// <summary>
+/// 更新イベント
+/// </summary>
+/// <param name="deltaTime">前回の更新からの経過時間(秒)</param>
+void GameObject::Update(float deltaTime)
+{
+    for (auto& e : components)
+    {
+        e->Update(deltaTime);
+    }
+    RemoveDestroyedComponent();
+}
+
+/// <summary>
+/// 衝突イベント
+/// </summary>
+/// <param name="self">衝突したコンポーネント(自分)</param>
+/// <param name="other">衝突したコンポーネント(相手)</param>
+void GameObject::OnCollision(
+    const ComponentPtr& self, const ComponentPtr& other)
+{
+    for (auto& e : components)
+    {
+        e->OnCollision(self, other);
+    }
+}
+
+/// <summary>
+/// 削除イベント
+/// </summary>
+void GameObject::OnDestroy()
+{
+    for (auto& e : components)
+    {
+        e->OnDestroy();
+    }
+}
```

### 2-3.エンジンクラスでゲームオブジェクトの管理
`Engine.h`
```diff
 	int Run();
 
+	/// <summary>
+	/// ゲームオブジェクトを作成する
+	/// </summary>
+	/// <param name="name">オブジェクトの名前</param>
+	/// <param name="position">オブジェクトを配置する座標</param>
+	/// <param name="rotation">オブジェクトの回転角度</param>
+	template<typename T>
+	std::shared_ptr<T> Create
+	(
+		const std::string& name,
+		const vec3& position = { 0, 0, 0 },
+		const vec3& rotation = { 0, 0, 0 }
+	)
+	{
+		std::shared_ptr<T> p = std::make_shared<T>();
+		p->engine = this;
+		p->name = name;
+		p->position = position;
+		p->rotation = rotation;
+		gameObjects.push_back(p); // エンジンに登録
+		return p;
+	}
+
+	// すべてのゲームオブジェクトを削除する
+	void ClearGameObjectAll();
 
 private:
 	int Initialize();
 	void Update();
 	void Render();
+	void UpdateGameObject(float deltaTime);
+	void RemoveDestroyedGameObject();
 
 	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
 	const std::string title = "OpenGLGame"; // ウィンドウタイトル
 	GLuint vs = 0;							// 頂点シェーダの管理番号
 	GLuint fs = 0;							// フラグメントシェーダの管理番号
 	GLuint prog3D = 0;						// シェーダプログラムの管理番号
 	GLuint vbo = 0;							// 頂点バッファの管理番号
 	GLuint ibo = 0;							// インデックスバッファの管理番号
 	GLuint vao = 0;							// 頂点属性配列の管理番号
+	GameObjectList gameObjects;				// ゲームオブジェクト配列
+	double previousTime = 0;				// 前回更新時の時刻
+	float deltaTime = 0;					// 前回更新からの経過時間
 	GLuint tex = 0;							// テクスチャ
```

`Engine.cpp`
```diff
     // ウィンドウの終了要求が来ていなかった(0)時,
     // メインループ処理を続ける
     // 引数 : GLFWwindowへのポインタ
     while (!glfwWindowShouldClose(window))
     {
         Update();
         Render();
+        RemoveDestroyedGameObject();
     }
     // GLFWライブラリの終了
     glfwTerminate();
     return 0;
 }
 
+/// <summary>
+/// ゲームエンジンから全てのゲームオブジェクトの破棄
+/// </summary>
+void Engine::ClearGameObjectAll()
+{
+    for (auto& e : gameObjects)
+    {
+        e->OnDestroy();
+    }
+    gameObjects.clear();
+}
 
 /// <summary>
 /// ゲームエンジンの初期化
 /// </summary>
 /// <returns>0 : 正常に初期化,
 /// 0以外 : エラーが発生した</returns>
 int Engine::Initialize()
 {
```
```diff
 #pragma endregion
 
 #pragma region boxのパラメータ
+    auto& box0 = *Create<GameObject>("box0");
     box0.scale = { 0.2f,0.2f,0.2f };
     box0.position = { 0.6f,0.6f,-1 };
 
+    auto& box1 = *Create<GameObject>("box1");
     box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
     box1.scale = { 0.2f, 0.2f, 0.2f };
     box1.position = { 0, 0, -1 };
 #pragma endregion
 
+#pragma region ゲームオブジェクト配列の容量を予約
+    gameObjects.reserve(1000);
+#pragma endregion
 
 #pragma region テクスチャの作成
```
```diff
     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
     {
         camera.rotation.y += 0.0005f;
     }
 
+    // デルタタイム(前回の更新からの経過時間)の計算
+    // glfwGetTime : プログラムが起動してからの経過時間の取得
+    const double currentTime = glfwGetTime(); // 現在時刻
+    deltaTime = static_cast<float>(currentTime - previousTime);
+    previousTime = currentTime;
+
+    // 経過時間が長すぎる場合は適当に短くする(主にデバッグ対策)
+    if (deltaTime >= 0.5f)
+    {
+        deltaTime = 1.0f / 60.0f;
+    }
+
+    UpdateGameObject(deltaTime);
 }
 
 /// <summary>
 /// ゲームエンジンの状態を描画
 /// </summary>
 void Engine::Render()
 {
```
```diff
 // 深度テストの有効化
 // glEnable : 指定されたOpenGLコンテキストの有効化
 // 引数 : 有効にする機能を表すマクロ定数
 // GL_DEPTH_TEST : 深度テスト
 glEnable(GL_DEPTH_TEST);
 
+// ゲームオブジェクトを描画
+for (const auto& e : gameObjects)
+{
     // ユニフォーム変数にデータワット
     glProgramUniform4fv
     (
         prog3D,     // プログラムオブジェクトの管理番号
         100,        // 送り先ロケーション番号
         1,          // データ数
+        e->color    // データのアドレス
     );
     glProgramUniform3fv
     (
         prog3D,         // プログラムオブジェクトの管理番号
         0,              // 送り先ロケーション番号
         1,              // データ数
+        &e->scale.x     // データのアドレス
     );
     glProgramUniform3fv
     (
         prog3D,             // プログラムオブジェクトの管理番号
         1,                  // 送り先ロケーション番号
         1,                  // データ数
+        &e->position.x      // データのアドレス
     );
     glProgramUniform2f
     (
         prog3D,
         2,
+        sin(e->rotation.y),
+        cos(e->rotation.y)
     );
 
     // 描画に使うテクスチャを
     // (テクスチャ・イメージ・ユニットに)割り当て
     glBindTextures
     (
            0,      // 割り当て開始インデックス
            1,      // 割り当てる個数
            &tex    // テクスチャ管理番号配列のアドレス
     );

        // 図形の描画
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // 基本図形の種類
            indexCount,         // インデックスデータ数
            GL_UNSIGNED_SHORT,  // インデックスデータの型
            0,                  // インデックスデータの開始位置
            1                   // 描画する図形の数
        );
+   }
```
```diff
     glfwPollEvents();
 }
 
+/// <summary>
+/// ゲームオブジェクトの状態を更新
+/// </summary>
+/// <param name="deltaTime">前回の更新からの経過時間(秒)</param>
+void Engine::UpdateGameObject(float deltaTime)
+{
+    // 要素の追加に対応するため添字を選択
+    for (int i = 0; i < gameObjects.size(); ++i)
+    {
+        GameObject* e = gameObjects[i].get();
+        if (!e->IsDestroyed())
+        {
+            e->Start();
+            e->Update(deltaTime);
+        }
+    }
+} // UpdateGameObject
+
+/// <summary>
+/// 破棄予定のゲームオブジェクトの削除
+/// </summary>
+void Engine::RemoveDestroyedGameObject()
+{
+    if (gameObjects.empty())
+    {
+        return; // ゲームオブジェクトを持っていなければ何もしない
+    }
+
+    // 破棄予定の有無でゲームオブジェクトを分ける
+    const auto iter = std::partition(
+        gameObjects.begin(), gameObjects.end(),
+        [](const GameObjectPtr& e)
+        {
+            return !e->IsDestroyed();
+        });
+
+    // 破棄予定のオブジェクトを別の配列に移動
+    GameObjectList destroyList(
+        std::make_move_iterator(iter),
+        std::make_move_iterator(gameObjects.end()));
+
+    // 配列から移動済みオブジェクトを削除
+    gameObjects.erase(iter, gameObjects.end());
+
+    // 破棄予定のオブジェクトのOnDestroyを実行
+    for (auto& e : destroyList)
+    {
+        e->OnDestroy();
+    }
+
+    // ここで実際にゲームオブジェクトが削除される(destoyListの寿命が終わるため)
+}
```

## 課題06
内容

box0を回転させるコードを削除して,

エラーを解消しなさい.

```diff
 /// <summary>
 /// ゲームエンジンの状態の更新
 /// </summary>
 void Engine::Update()
 {
-    // box0の回転
-    box0.rotation.y += 0.0001f;
-
     // glfwGetKey(GLFWウィンドウオブジェクトのアドレス,キー番号);
     // GLFW_RELEASE : キー入力なし
     // GLFW_PRESS   : キー入力あり
     // カメラの移動
     const float cameraSpeed = 0.0005f;
```

### 2-4.カメラ操作用のコンポーネントの追加
プロジェクトの選択

->ctrl + shift + A

->プロジェクト名\Src\Engine\PlayerComponent.hを入力

->追加をクリック

`PlayerComponent.h`
```diff
+/**
+* @file PlayerComponent.h
+*/
+#ifndef PLAYERCOMPONENT_H_INCLUDED
+#define PLAYERCOMPONENT_H_INCLUDED
+#include "Engine/Component.h"
+#include "Engine/Engine.h"
+
+/// <summary>
+/// プレイヤー
+/// </summary>
+class PlayerComponent
+	: public Component
+{
+public:
+	PlayerComponent() = default;
+	virtual ~PlayerComponent() = default;
+
+    virtual void Update(float deltaTime) override
+    {    
+        Engine* engine = GetOwner()->GetEngine();
+        GameObject & camera = engine->GetMainCamera();
+
+        // glfwGetKey(GLFWウィンドウオブジェクトのアドレス,キー番号);
+        // GLFW_RELEASE : キー入力なし
+        // GLFW_PRESS   : キー入力あり
+        // カメラの移動
+        const float cameraSpeed = 0.0005f;
+        const float cameraCos = cos(camera.rotation.y);
+        const float cameraSin = sin(camera.rotation.y);
+        if (engine->GetKey(GLFW_KEY_A) == GLFW_PRESS)
+        {
+            camera.position.x -= cameraSpeed * cameraCos;
+            camera.position.z -= cameraSpeed * -cameraSin;
+        }
+        if (engine->GetKey(GLFW_KEY_D) == GLFW_PRESS)
+        {
+            camera.position.x += cameraSpeed * cameraCos;
+            camera.position.z += cameraSpeed * -cameraSin;
+        }
+        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
+        {
+            camera.position.x -= cameraSpeed * cameraSin;
+            camera.position.z -= cameraSpeed * cameraCos;
+        }
+        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
+        {
+            camera.position.x += cameraSpeed * cameraSin;
+            camera.position.z += cameraSpeed * cameraCos;
+        }
+
+        // カメラの回転
+        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
+        {
+            camera.rotation.y -= 0.0005f;
+        }
+        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
+        {
+            camera.rotation.y += 0.0005f;
+        }
+    }
+};
+
+#endif // !PLAYERCOMPONENT_H_INCLUDED
```

`Engine.h`
```diff
 	// すべてのゲームオブジェクトを削除する
 	void ClearGameObjectAll();
 
+	// カメラを取得する
+	GameObject& GetMainCamera()
+	{
+		return camera;
+	}
+	const GameObject& GetMainCamera() const
+	{
+		return camera;
+	}
+
+	// キーが押されていたらtrue, 押されてなければfalseを返す
+	bool GetKey(int key) const
+	{
+		return glfwGetKey(window, key) == GLFW_PRESS;
+	}
 
 private:
 	int Initialize();
```

`Engine.cpp`
```diff
 /**
 * @file Engine.cpp
 */
 #include "Engine.h"
+#include "../PlayerComponent.h"
 #pragma warning(push)
```
```diff
 #pragma endregion
 
+#pragma region カメラ操作用コンポーネントを追加
+    auto player = Create<GameObject>("player", { 0, 10, 0 });
+    player->AddComponent<PlayerComponent>();
+#pragma endregion
 
 #pragma region ゲームオブジェクト配列の容量を予約
```

## 課題07
内容

PlayerComponent::Updateのエラーを

全て解決しなさい.
```diff
         if (engine->GetKey(GLFW_KEY_D) == GLFW_PRESS)
         {
             camera.position.x += cameraSpeed * cameraCos;
             camera.position.z += cameraSpeed * -cameraSin;
         }
+        if (engine->GetKey(GLFW_KEY_W) == GLFW_PRESS)
         {
             camera.position.x -= cameraSpeed * cameraSin;
             camera.position.z -= cameraSpeed * cameraCos;
         }
+        if (engine->GetKey(GLFW_KEY_S) == GLFW_PRESS)
         {
             camera.position.x += cameraSpeed * cameraSin;
             camera.position.z += cameraSpeed * cameraCos;
         }
 
         // カメラの回転
+        if (engine->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
         {
             camera.rotation.y -= 0.0005f;
         }
+        if (engine->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
         {
             camera.rotation.y += 0.0005f;
         }
     }
 };
 
 #endif // !PLAYERCOMPONENT_H_INCLUDED
```