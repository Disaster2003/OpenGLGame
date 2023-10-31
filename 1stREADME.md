# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.プロジェクトの作成
VisualStudioの起動

->`新しいプロジェクトの作成`をクリック

->`空のプロジェクト`を選択

->`次へ`をクリック

->プロジェクト名と保存場所の設定

->`作成`をクリック

## 2.機能設定
プロジェクトを選択

->alt + enter

->構成を`すべての構成`に

->プラットフォームを`すべてのプラットフォーム`に

->構成プロパティ\全般のC++言語標準を

　`ISO C++20標準(/std:c++20)`に

->構成プロパティ\詳細の文字セットを

　`マルチバイト文字セットを使用する`に

->構成プロパティ\リンカー\システムのサブシステムを

　`Windows(/SUBSYSTEM:WINDOWS)`に

->`適用`をクリック

->`OK`をクリック

## 3.OpenGLの導入
プロジェクトを選択

->`NuGetパッケージの管理`をクリック

->`参照`をクリック

->検索boxに`glfw`を入力

->`glfw`を選択

->`インストール`をクリック

->`OK`をクリック

## 4.ウィンドウの作成
プロジェクトの選択

->ctrl + shift + A

->`プロジェクト名\Src\Main.cpp`を入力

->`追加`をクリック

```diff
+/**
+* @file Main.cpp
+*/
+#include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル
+#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
+#include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
+
+/// <summary>
+/// エントリーポイント
+/// </summary>
+int WINAPI WinMain
+(
+    _In_ HINSTANCE hInstnce,
+    _In_opt_ HINSTANCE hPrevInstance,
+    _In_ LPSTR lpCmdLine,
+    _In_ int nShowCmd
+)
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
+    GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
+    const std::string title = "OpenGLGame"; // ウィンドウタイトル
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
+#pragma endregion
+
+#pragma region メインループの定義
+    // ウィンドウの終了要求が来ていなかった(0)時,
+    // メインループ処理を続ける
+    // 引数 : GLFWwindowへのポインタ
+    while (!glfwWindowShouldClose(window))
+    {
+        // バックバッファの描画終了時,
+        // フロントバッファと役割の交換
+        // 引数 : GLFWwindowへのポインタ
+        glfwSwapBuffers(window);
+
+        // 「OSからの要求」の処理
+        // (キーボードやマウスなどの状態を取得するなど)
+        glfwPollEvents();
+    }
+#pragma endregion
+
+#pragma region GLFWライブラリの終了
+    glfwTerminate();
+#pragma endregion
+
+    return 0;
+}
```

## 5.OpenGL関数の試用

[GLADのダウンロードページ](https://glad.dav1d.de/)

ダウンロードページの起動

->Specificationを`OpenGL`に

->APIのglを`Version4.5`に

->Profileを`Compatibility`に

->Optionsの`Generate a loader`をチェック

->`GENERATE`をクリック

->`glad.zip`をダウンロード

->`プロジェクト名/Src/glad`に`glad.zip`を展開

->(VisualStudio)ソースファイルを選択

->右クリック,追加,`新しいフィルター`を選択

->名前を`glad`に

->プロジェクトの選択

->alt + enter

->構成プロパティ\C/C++\全般の

　追加のインクルードディレクトリのvの`編集`をクリック

->テキストボックスに`$(ProjectDir)Src\glad\include`を入力

->`OK`をクリック

```diff
 /**
 * @file Main.cpp
 */
+#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
 #include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル
 #include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
 #include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
 
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
 #pragma region GLFWライブラリの初期化
     // 初期化に成功しなかった(!GLFW_TRUE)時,
     // 1を返して終了
     if (glfwInit() != GLFW_TRUE)
     {
         return 1; // 初期化失敗
     }
 #pragma endregion
 
 #pragma region 描画ウィンドウの作成
     GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
     const std::string title = "OpenGLGame"; // ウィンドウタイトル
     window =
         //グラフィックス描画用ウィンドウの作成
         glfwCreateWindow
         (
             1280,           // ウィンドウの幅
             720,            // ウィンドウの高さ
             title.c_str(),  // タイトルバーに表示する文字列
             nullptr,        // 表示するモニタの選択
             nullptr         // リソースを共有するウィンドウ
         );
     // ウィンドウの作成に成功しなかった(nullptr)時,
     // 1を返して終了
     if (!window)
     {
         // GLFWライブラリの終了
         glfwTerminate();
         return 1;   // ウィンドウ作成失敗
     }
 #pragma endregion
 
 #pragma region OpenGL関数の導入
     // OpenGLコンテキストの作成
     // 引数 : GLFWウィンドウオブジェクトのアドレス
     glfwMakeContextCurrent(window);
 
+    // gladLoadGLLoader : 必要な関数のアドレスを全て取得
+    // glfwGetProcAddress : OpenGL関数名に対応する関数のアドレスを返す
+    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
+    {
+        // GLFWライブラリの終了
+        glfwTerminate();
+        return 1; // アドレス取得失敗
+    }
 #pragma endregion
 
 #pragma region メインループの定義
     // ウィンドウの終了要求が来ていなかった(0)時,
     // メインループ処理を続ける
     // 引数 : GLFWwindowへのポインタ
     while (!glfwWindowShouldClose(window))
     {
+        // バックバッファを消去するときに使用する色を指定
+        glClearColor
+        (
+            0.3f,   // R(赤)
+            0.6f,   // G(緑)
+            0.9f,   // B(青)
+            1.0f    // A(透明度)
+        );
+        // バックバッファをクリア
+        // GL_COLOR_BUFFER_BIT : カラーバッファ(色)
+        // GL_DEPTH_BUFFER_BIT : 深度バッファ(奥行き)
+        // GL_STENCIL_BUFFER_BIT : ステンシルバッファ(切り抜き)
+        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
         // バックバッファの描画終了時,
         // フロントバッファと役割の交換
         // 引数 : GLFWwindowへのポインタ
         glfwSwapBuffers(window);
         // 「OSからの要求」の処理
         // (キーボードやマウスなどの状態を取得するなど)
         glfwPollEvents();
     }
 #pragma endregion
 
 #pragma region GLFWライブラリの終了
     glfwTerminate();
 #pragma endregion
 
     return 0;
 }
```

## 課題01
内容

glClearColorの引数を変更して,

描画ウィンドウを青色(または好きな色)で塗りつぶしなさい。

```C++
glClearColor
(
    0.3f,   // R(赤)
    0.6f,   // G(緑)
    0.9f,   // B(青)
    1.0f    // A(透明度)
);
```
を変える

## 6. 図形の描画
OpenGLで図形を描画するには...
>*頂点データ: 頂点の座標や法線など。
>*インデックスデータ: 図形を構成する頂点の番号。
>*頂点データ形式: 頂点データの解釈方法。

```diff
```

>*頂点属性 : 頂点データ形式のこと。
>*GPUは16要素の頂点属性配列を扱える。

`※常に16個全てはデータ処理 長`

`　必要な部分だけを有効に`

```diff
```