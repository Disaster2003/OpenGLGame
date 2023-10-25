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
+    while (!glfwWindowShouldClose(window))
+    {
+        // バックバッファの描画終了時,
+        // フロントバッファと役割の交換
+        glfwSwapBuffers(window);
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