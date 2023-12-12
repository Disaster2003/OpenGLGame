# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.プログラムの構造化
`構造化の思考基盤`
>1. プログラムのすべての構成要素を洗い出し,<br>
正しさ(意図したとおりに動くこと)を<br>
容易に証明できる程度に小さな単位へと<br>
分解する(分割、モジュール化)。
>2. 分解した構成要素を階層化して,<br>
プログラム全体の正しさを段階的に<br>
証明できるようにする(可読性、テスト容易性).

`ゲームプログラムの二大構成`
>* どのゲームでも必要な要素(Engine)<br>
(入出力制御、画面描画、ゲームオブジェクト管理など)
>* ゲーム固有の要素<br>
(プレイヤーの操作方法、敵の行動、ゲームルールの管理など)

### 1-1.WinMain() -> Engine.Run()への移行
`Engienの要素分解`
>* ゲームループの制御
>* OpenGLの制御
>* シーン(レベル)の管理
>* ゲームオブジェクトの管理
>* アセット(モデル, テクスチャ, シェーダなど)の管理

`目標 : 「WinMain関数の全てのプログラムを<br>
エンジンクラスのメンバにする」こと`

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

->プロジェクト名\Src\Enigne\Engine.hを入力

->追加をクリック

`Engine.h`
```diff
+/**
+* @file Engine.h
+*/
+#ifndef ENGINE_H_INCLUDED	// インクルードガード
+#define ENGINE_H_INCLUDED	// インクルードガード
+#include "glad/glad.h"
+#include <GLFW/glfw3.h>
+#include <string>
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
 その後,宣言だけを切り取る。
>2. 切り取った変数宣言を,<br>
 移動先のクラス定義に貼り付ける.
>3. プログラムをビルドして,<br>
 正しく動作することを確認する.

`現時点で「要素をまたいで利用している」,<br>
あるいは「今後,要素をまたいで利用する可能性がある」変数`
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
```C++
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
```C++
     20,21,22,22,23,20,
 };
-GLuint ibo = 0; // インデックスバッファの管理番号
 // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
 glCreateBuffers
 (
```
```C++
 #pragma region 頂点データ形式の設定
-    GLuint vao = 0; // 頂点属性配列の管理番号
     // 頂点属性オブジェクトの作成
     glCreateVertexArrays
     (
```
```C++
 #pragma endregion
 
 #pragma region テクスチャの作成
+    tex = LoadTexture("Res/box.tga");
 #pragma endregion
 
 #pragma region メインループの定義
```

`Engine.h`
```C++
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
+	GLuint tex = 0;							// テクスチャ
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
 #include "glad/glad.h"
+#include "GameObject.h"
 #include <GLFW/glfw3.h>
 #include <string>
```

## 課題02
内容

Main.cppの

camera,box0,box1を

メンバ変数にしなさい

`Engine.h`
```C++
	GLuint tex = 0;							// テクスチャ

	GameObject camera;						// カメラオブジェクト
	GameObject box0;
	GameObject box1;
};

#endif						// !ENGINE_H_INCLUDED(インクルードガード)
```

### 1-4.Run関数を3つのパートに分割