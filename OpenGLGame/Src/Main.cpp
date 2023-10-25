/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
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

    // gladLoadGLLoader : 必要な関数のアドレスを全て取得
    // glfwGetProcAddress : OpenGL関数名に対応する関数のアドレスを返す
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        // GLFWライブラリの終了
        glfwTerminate();
        return 1; // アドレス取得失敗
    }
#pragma endregion

#pragma region メインループの定義
    // ウィンドウの終了要求が来ていなかった(0)時,
    // メインループ処理を続ける
    while (!glfwWindowShouldClose(window))
    {
        // バックバッファを消去するときに使用する色を指定
        glClearColor
        (
            0.3f,   // R(赤)
            0.6f,   // G(緑)
            0.9f,   // B(青)
            1.0f    // A(透明度)
        );
        // バックバッファをクリア
        // GL_COLOR_BUFFER_BIT : カラーバッファ(色)
        // GL_DEPTH_BUFFER_BIT : 深度バッファ(奥行き)
        // GL_STENCIL_BUFFER_BIT : ステンシルバッファ(切り抜き)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // バックバッファの描画終了時,
        // フロントバッファと役割の交換
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