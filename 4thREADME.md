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