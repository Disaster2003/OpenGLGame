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

#pragma region 頂点データをGPUメモリにコピー
    // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
    const float vertexData[][3] =
    {
        {-0.2f,-0.2f, 0}, { 0.2f,-0.2f, 0}, { 0.0f, 0.2f, 0},
        {-0.8f,-0.2f, 0}, {-0.4f,-0.2f, 0}, {-0.4f, 0.2f, 0}, {-0.8f, 0.2f, 0},
    };
    GLuint vbo = 0; // 頂点バッファの管理番号
    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
    glCreateBuffers
    (
        1,      // 作成するオブジェクト数
        &vbo    // 頂点バッファの管理番号を格納する配列
    );
    // GPUメモリを確保のち,データをコピー
    // データのアドレスがnullptrなら,
    // GPUメモリの確保のみ
    glNamedBufferStorage
    (
        vbo,                // 頂点バッファの管理番号
        sizeof(vertexData), // コピーするバイト数
        vertexData,         // コピーするデータのアドレス
        0                   // 各種フラグ
    );
#pragma endregion

#pragma region インデックスデータをGPUメモリにコピー
    // インデックスデータ(図形を構成する頂点番号)
    const GLushort indexData[] =
    {
        0, 1, 2,
        3, 4, 5, 5, 6, 3,
    };
    GLuint ibo = 0; // インデックスバッファの管理番号
    // バッファオブジェクト(GPUメモリを管理するためのオブジェクト)の作成
    glCreateBuffers
    (
        1,      // 作成するオブジェクト数
        &ibo    // インデックスバッファの管理番号
    );
    // GPUメモリを確保のち,データをコピー
    // データのアドレスがnullptrなら,
    // GPUメモリの確保のみ
    glNamedBufferStorage
    (
        ibo,               // インデックスバッファの管理番号
        sizeof(indexData), // コピーするバイト数
        indexData,         // コピーするデータのアドレス
        0                  // 各種フラグ
    );
#pragma endregion

#pragma region 頂点データ形式の設定
    // 頂点属性配列の設定
    GLuint vao = 0; // 頂点属性の管理番号
    // VAOの作成
    glCreateVertexArrays
    (
        1,    // 作成するオブジェクト数
        &vao  // 頂点属性の管理番号を格納する配列
    );
    // VAOをOpenGLコンテキストにバインド(割り当てる)
    // 引数 : 割り当てるVAOの管理番号
    glBindVertexArray(vao);

    // IBOをOpenGLコンテキストとVAOの両方にバインド(VAOがOpenGLコンテキストにバインドされているため)
    // GL_ELEMENT_ARRAY_BUFFER
    // ->OpenGLコンテキストにVAOが割り当てられている場合、
    //   バッファオブジェクトをVAOにも割り当てる
    glBindBuffer
    (
        GL_ELEMENT_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
        ibo                       // 割り当てるバッファオブジェクトの管理番号
    );

    // VBOをOpenGLコンテキストにバインド(IBOと異なり、VAOにはバインドされない)
    glBindBuffer
    (
        GL_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
        vbo               // 割り当てるバッファオブジェクトの管理番号
    );

    // 0番目の頂点属性を有効化
    // 引数 : 有効にする頂点属性配列のインデックス
    glEnableVertexAttribArray(0);

    // 0番目の頂点属性を設定
    // このとき、OpenGLコンテキストにバインドされているVBOが、頂点属性にバインドされる
    glVertexAttribPointer
    (
        0,        // 頂点属性配列のインデックス
        3,        // データの要素数
        GL_FLOAT, // データの型
        GL_FALSE, // 正規化の有無
        0,        // 次のデータまでのバイト数
        0         // 最初のデータの位置
    );
#pragma endregion

#pragma region メインループの定義
    // ウィンドウの終了要求が来ていなかった(0)時,
    // メインループ処理を続ける
    // 引数 : GLFWwindowへのポインタ
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

        // VAOをOpenGLコンテキストに割り当てる    
        // 引数 : 割り当てるVAOの管理番号
        glBindVertexArray(vao);
        // 図形を描画
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // 基本図形の種類
            9,                  // インデックスデータ数
            GL_UNSIGNED_SHORT,  // インデックスデータの型
            0,                  // インデックスデータの開始位置
            1                   // 描画する図形の数
        );
        // VAOの割り当てを解除
        // 引数 : 割り当てるVAOの管理番号
        glBindVertexArray(0);

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