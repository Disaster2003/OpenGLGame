/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
#include <GLFW/glfw3.h> // GLFWライブラリの関数が定義されているヘッダファイル

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
#pragma warning(pop)

#include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル
#include <stdio.h>      // 標準入出力ライブラリの関数が定義されているヘッダファイル

/// <summary>
/// OpenGLからのメッセージを処理するコールバック関数
/// </summary>
/// <param name="source">メッセージの発信者(OpenGL,Windows,シェーダなど)</param>
/// <param name="type">メッセージの種類(エラー,警告など)</param>
/// <param name="id">メッセージを一位に識別する値</param>
/// <param name="severity">メッセージの重要度(高,中,低,最低)</param>
/// <param name="length">メッセージの文字数. 負数ならメッセージは0終端されている</param>
/// <param name="message">メッセージ本体</param>
/// <param name="userParam">コールバック設定時に指定したポインタ</param>
void APIENTRY DebugCallback
(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
{
  std::string s;
  // メッセージの文字数が
  // 負数の時,0終端("\0")
  if (length < 0)
  {
    s = message;
  }
  // メッセージの読み込み
  else
  {
    s.assign(message, message + length);
  }
  s += '\n'; // メッセージには改行がないので追加する
  // 出力ウィンドウへの表示
  // 引数 : 出力する文字列(エラーメッセージ)
  OutputDebugStringA(s.c_str());
}

/// <summary>
/// シェーダファイルを読み込んでコンパイルする
/// </summary>
/// <param name="type">シェーダの種類</param>
/// <param name="filename">シェーダファイル名</param>
/// <returns>シェーダの管理番号</returns>
GLuint CompileShader
(
    GLenum type,
    const char* filename
)
{
    // ファイルを開く
    // std::ios::binary : バイナリモードの指定
    std::ifstream file
    (
        filename,
        std::ios::binary
    );
    // ファイルが正常に開けない(false)時,
    // エラーメッセージを出力して,
    // 0(オブジェクトが存在しない)を返して終了
    if (!file)
    {
        char s[256];
        snprintf
        (
            s,
            256,
            "[ERROR] %s: %sを開けません\n",
            __func__,
            filename
        );
        OutputDebugStringA(s);
        return 0;
    }

    // ファイルサイズの取得
    // 引数 : ファイルパス
    const size_t filesize = std::filesystem::file_size(filename);
    // ファイルサイズ分の領域を用意
    std::vector<char> buffer(filesize);
    // ファイルの読み込み
    file.read
    (
        buffer.data(),  // バッファアドレス
        filesize        // 読み込むバイト数
    );
    // ファイルを閉じる
    file.close();

    const char* source[] = { buffer.data() };       // bufferの先頭アドレス
    const GLint length[] = { int(buffer.size()) };  // bufferのサイズ
    // 「シェーダを管理するオブジェクト」の作成
    // 引数 : 作成するシェーダの種類
    const GLuint object = glCreateShader(type);
    // シェーダ文字列の設定
    glShaderSource
    (
        object, // シェーダの管理番号
        1,      // 文字列の数
        source, // 文字列の配列
        length  // 文字列長の配列
    );
    // シェーダ文字列のコンパイル
    // 引数 : シェーダの管理番号
    glCompileShader(object);
    return object;
}

/// <summary>
/// テクスチャの読み込み
/// </summary>
/// <param name="filename">テクスチャファイル名</param>
/// <returns>テクスチャの管理番号</returns>
GLuint LoadTexture(const char* filename)
{
  // ファイルを開く
  // std::ios::binary : バイナリモードの指定
  std::ifstream file
  (
      filename,
      std::ios::binary
  );
  // ファイルが正常に開けない(false)時,
  // エラーメッセージを出力して,
  // 0(オブジェクトが存在しない)を返して終了
  if ( ! file)
  {
    char s[256];
    snprintf
    (
        s,
        256,
        "[ERROR] %s: %sを開けません\n",
        __func__,
        filename
    );
    OutputDebugStringA(s);
    return 0;
  }

  // ファイルサイズの取得
// 引数 : ファイルパス
  const size_t filesize = std::filesystem::file_size(filename);
  // ファイルサイズ分の領域を用意
  std::vector<uint8_t> buffer(filesize);
  // ファイルの読み込み
  file.read
  (
      reinterpret_cast<char*>(buffer.data()),   // バッファアドレス
      filesize                                  // 読み込むバイト数
  );
  // ファイルを閉じる
  file.close();

  // ヘッダから情報を取得
  const size_t tgaHeaderSize = 18; // ヘッダ情報のバイト数
  const int width = buffer[12] + buffer[13] * 256;
  const int height = buffer[14] + buffer[15] * 256;

  GLuint object = 0; // テクスチャの管理番号
  // テクスチャの作成
  glCreateTextures
  (
      GL_TEXTURE_2D,    // テクスチャの種類
      1,                // 作成する個数
      &object           // 番号を格納する配列のアドレス
  );
  // 画像データ分のGPUメモリ領域の確保
  glTextureStorage2D
  (
      object,   // テクスチャの管理番号
      1,        // 作成するレベル数(ミップマップ)
      GL_RGBA8, // ピクセル形式
      width,    // 幅
      height    // 高さ
  );
  // 画像データをGPUメモリにコピー
  glTextureSubImage2D
  (
      object,                       // テクスチャの管理番号
      0,                            // コピー先のレイヤー番号
      0,                            // コピー先のX座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
      0,                            // コピー先のY座標(複数の画像をひとつにまとめる用 ex)スプライトシート,テクスチャアトラス)
      width,                        // コピーする画像の幅
      height,                       // コピーする画像の高さ
      GL_BGRA,                      // ピクセルに含まれる要素と順序
      GL_UNSIGNED_BYTE,             // 要素の型
      buffer.data() + tgaHeaderSize // 画像データのアドレス
  );
  return object;
}

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
    // GLFWライブラリを使ってコンテキストの種類の設定
    // GLFW_TRUE : デバッグコンテキストの作成
    // GLFW_FALSE : 通常のコンテキストの作成
    glfwWindowHint
    (
        GLFW_OPENGL_DEBUG_CONTEXT,  // ヒントの種類
        GLFW_TRUE                   // ヒントに設定する値
    );
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

#pragma region メッセージコールバックの設定
    glDebugMessageCallback
    (
        DebugCallback,  // 呼び出す関数のアドレス
        nullptr         // 関数のuserParam引数に渡される値
    );
#pragma endregion

#pragma region シェーダのコンパイルとリンク
    // シェーダを読み込んでコンパイル
    const GLuint vs = 
        CompileShader
        (
            GL_VERTEX_SHADER,
            "Res/standard.vert"
        );
    const GLuint fs = 
        CompileShader
        (
            GL_FRAGMENT_SHADER,
            "Res/standard.frag"
        );

    // プログラムオブジェクトの作成
    GLuint prog3D = glCreateProgram();
    // 頂点シェーダの指定
    glAttachShader
    (
        prog3D, // プログラムオブジェクトの管理番号
        vs      // 頂点シェーダの管理番号
    );
    // フラグメントシェーダの指定
    glAttachShader
    (
        prog3D, // プログラムオブジェクトの管理番号
        fs      // フラグメントシェーダの管理番号
    );
    // 指定した2つのシェーダのリンク
    // 引数 : プログラムオブジェクトの管理番号
    glLinkProgram(prog3D);
#pragma endregion

#pragma region 頂点データをGPUメモリにコピー
    // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
    struct vec2 { float x, y; };
    struct vec3 { float x, y, z; };
    struct Vertex
    {
        vec3 position; // 頂点座標
        vec2 texcoord; // テクスチャ座標
    };
    const Vertex vertexData[] =
    {
        // +Z(手前の面)
        { {-1,-1, 1 }, { 0, 0 } },
        { { 1,-1, 1 }, { 1, 0 } },
        { { 1, 1, 1 }, { 1, 1 } },
        { {-1, 1, 1 }, { 0, 1 } },

        // -Z(奥の面)
        { { 1,-1,-1 }, { 0, 0 } },
        { {-1,-1,-1 }, { 1, 0 } },
        { {-1, 1,-1 }, { 1, 1 } },
        { { 1, 1,-1 }, { 0, 1 } },

        // +Y(上の面)
        { { 1, 1, 1 }, { 0, 0 } },
        { { 1, 1,-1 }, { 1, 0 } },
        { {-1, 1,-1 }, { 1, 1 } },
        { {-1, 1, 1 }, { 0, 1 } },
        
        // -Y(下の面)
        { { 1,-1,-1 }, { 0, 0 } },
        { { 1,-1, 1 }, { 1, 0 } },
        { {-1,-1, 1 }, { 1, 1 } },
        { {-1,-1,-1 }, { 0, 1 } },
        
        // +X(右の面)
        { { 1, 1, 1 }, { 0, 0 } },
        { { 1, 1,-1 }, { 1, 0 } },
        { { 1,-1,-1 }, { 1, 1 } },
        { { 1,-1, 1 }, { 0, 1 } },
        
        // -X(左の面)
        { {-1, 1, 1 }, { 0, 0 } },
        { {-1, 1,-1 }, { 1, 0 } },
        { {-1,-1,-1 }, { 1, 1 } },
        { {-1,-1, 1 }, { 0, 1 } },
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
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16, 17,18,18,19, 16,
        20,21,22,22,23,20,
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
    GLuint vao = 0; // 頂点属性配列の管理番号
    // 頂点属性オブジェクトの作成
    glCreateVertexArrays
    (
        1,    // 作成するオブジェクト数
        &vao  // 頂点属性配列の管理番号を格納する配列
    );
    // VAOをOpenGLコンテキストにバインド(割り当てる)
    // 引数 : 割り当てる頂点属性配列の管理番号
    glBindVertexArray(vao);

    // IBOをOpenGLコンテキストとVAOの両方にバインド
    // (VAOがOpenGLコンテキストにバインドされているため)
    // GL_ELEMENT_ARRAY_BUFFER
    // ->OpenGLコンテキストにVAOが割り当てられている場合,
    //   バッファオブジェクトをVAOにも割り当てる
    glBindBuffer
    (
        GL_ELEMENT_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
        ibo                       // 割り当てるインデックスバッファの管理番号
    );

    // VBOをOpenGLコンテキストにバインド(IBOと異なり,VAOにはバインドされない)
    glBindBuffer
    (
        GL_ARRAY_BUFFER,  // 割り当てるオブジェクトの種類
        vbo               // 割り当てる頂点バッファ管理番号
    );

    // 0番目の頂点属性の有効化
    // 引数 : 有効にする頂点属性配列のインデックス
    glEnableVertexAttribArray(0);

    // 0番目の頂点属性の設定
    // このとき,OpenGLコンテキストにバインドされているVBOが,
    // 頂点属性にバインドされる
    glVertexAttribPointer
    (
        0,              // 頂点属性配列のインデックス
        3,              // データの要素数
        GL_FLOAT,       // データの型
        GL_FALSE,       // 正規化の有無
        sizeof(Vertex), // 次のデータまでのバイト数
        0               // 最初のデータの位置
    );

    // 1番目の頂点属性の有効化
    // 引数 : 有効にする頂点属性配列のインデックス
    glEnableVertexAttribArray(1);

    // 1番目の頂点属性の設定
    glVertexAttribPointer
    (
        1,                              // 頂点属性配列のインデックス
        2,                              // データの要素数
        GL_FLOAT,                       // データの型
        GL_FALSE,                       // 正規化の有無
        sizeof(Vertex),                 // 次のデータまでのバイト数
        reinterpret_cast<const void*>   // 最初のデータの位置
        (
            // 構造体の先頭から特定のメンバまでの
            // バイト数の計算
            offsetof
            (
                Vertex,     // 構造体名
                texcoord    // メンバ名
            )
        )
    );
#pragma endregion

#pragma region 物体のパラメータ
    class GameObject
    {
    public:
        vec3 position = { 0, 0, 0 };    // 物体の位置
        vec3 rotation = { 0, 0, 0 };    // 物体の回転角度
        vec3 scale = { 1,1,1 };         // 物体の拡大率
        float color[4] = { 1, 1, 1, 1 };// 物体の色
    };

    // カメラオブジェクト
    GameObject camera;

    GameObject box0;
    box0.scale = { 0.2f,0.2f,0.2f };
    box0.position = { 0.6f,0.6f,-1 };

    GameObject box1;
    box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
    box1.scale = { 0.2f, 0.2f, 0.2f };
    box1.position = { 0, 0, -1 };
#pragma endregion

#pragma region テクスチャの作成
    GLuint tex = LoadTexture("Res/box.tga");
#pragma endregion

#pragma region メインループの定義
    // ウィンドウの終了要求が来ていなかった(0)時,
    // メインループ処理を続ける
    // 引数 : GLFWwindowへのポインタ
    while (!glfwWindowShouldClose(window))
    {
        // box0の回転
        box0.rotation.y += 0.0001f;

        // glfwGetKey(GLFWウィンドウオブジェクトのアドレス,キー番号);
        // GLFW_RELEASE : キー入力なし
        // GLFW_PRESS   : キー入力あり
        // カメラの移動
        const float cameraSpeed = 0.0005f;
        const float cameraCos = cos(camera.rotation.y);
        const float cameraSin = sin(camera.rotation.y);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.position.x -= cameraSpeed * cameraCos;
            camera.position.z -= cameraSpeed * -cameraSin;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.position.x += cameraSpeed * cameraCos;
            camera.position.z += cameraSpeed * -cameraSin;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.position.x -= cameraSpeed * cameraSin;
            camera.position.z -= cameraSpeed * cameraCos;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.position.x += cameraSpeed * cameraSin;
            camera.position.z += cameraSpeed * cameraCos;
        }

        //// マウスの入力の取得
        //double mouseX, mouseY;
        //glfwGetCursorPos(window, &mouseX, &mouseY);
        //if ()
        //{
        //  camera.rotation.y -= 0.0005f;
        //}
        //if ()
        //{
        //  camera.rotation.y += 0.0005f;
        //}
        //if ()
        //{
        //  camera.rotation.x += 0.0005f;
        //}
        //if ()
        //{
        //  camera.rotation.x -= 0.0005f;
        //}

        // カメラの回転
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
          camera.rotation.y -= 0.0005f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
          camera.rotation.y += 0.0005f;
        }
        //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        //{
        //  camera.rotation.x += 0.0005f;
        //}
        //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        //{
        //  camera.rotation.x -= 0.0005f;
        //}

        // バックバッファを消去するときに使用する色の指定
        glClearColor
        (
            0.3f,   // R(赤)
            0.6f,   // G(緑)
            0.9f,   // B(青)
            1.0f    // A(透明度)
        );
        // バックバッファのクリア
        // GL_COLOR_BUFFER_BIT : カラーバッファ(色)
        // GL_DEPTH_BUFFER_BIT : 深度バッファ(奥行き)
        // GL_STENCIL_BUFFER_BIT : ステンシルバッファ(切り抜き)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // VAOをOpenGLコンテキストに割り当てる    
        // 引数 : 割り当てる頂点属性配列の管理番号
        glBindVertexArray(vao);

        // 描画に使うシェーダの指定
        glUseProgram(prog3D);

        // フレームバッファの大きさを取得
        int fbWidth, fbHeight;
        glfwGetFramebufferSize
        (
            window,     // GLFWウィンドウオブジェクトのアドレス
            &fbWidth,   // 描画ウィンドウの幅を格納する変数のアドレス
            &fbHeight   // 描画ウィンドウの高さを格納する変数のアドレス
        );

        // ビューポートの設定
        glViewport
        (
            0,          // 描画範囲の左下Xの座標
            0,          // 描画範囲の左下のY座標
            fbWidth,    // 描画範囲の幅
            fbHeight    // 描画範囲の高さ
        );
    
        // アスペクト比と視野角の設定
        const float aspectRatio =
          static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
        const float degFovY = 60; // 垂直視野角
        const float radFovY = degFovY * 3.1415926535f / 180;
        const float scaleFov = tan(radFovY / 2); // 視野角による拡大率
        glProgramUniform2f
        (
            prog3D,                         // プログラムオブジェクトの管理番号
            3,                              // 送り先ロケーション番号
            1 / (aspectRatio * scaleFov),   // データ数
            1 / scaleFov                    // データのアドレス
        );

        // カメラパラメータの設定
        glProgramUniform3fv
        (
            prog3D,             // プログラムオブジェクトの管理番号
            4,                  // 送り先ロケーション番号
            1,                  // データ数
            &camera.position.x  // データのアドレス
        );
        glProgramUniform2f
        (
            prog3D,                     // プログラムオブジェクトの管理番号
            5,                          // 送り先ロケーション番号
            sin(-camera.rotation.y),    // データ数
            cos(-camera.rotation.y)     // データのアドレス
        );

        // 深度テストの有効化
        // glEnable : 指定されたOpenGLコンテキストの有効化
        // 引数 : 有効にする機能を表すマクロ定数
        // GL_DEPTH_TEST : 深度テスト
        glEnable(GL_DEPTH_TEST);

        // 変数ユニフォームにデータワット
        glProgramUniform4fv
        (
            prog3D,     // プログラムオブジェクトの管理番号
            100,        // 送り先ロケーション番号
            1,          // データ数
            box0.color  // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,         // プログラムオブジェクトの管理番号
            0,              // 送り先ロケーション番号
            1,              // データ数
            &box0.scale.x   // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,             // プログラムオブジェクトの管理番号
            1,                  // 送り先ロケーション番号
            1,                  // データ数
            &box0.position.x    // データのアドレス
        );
        glProgramUniform2f
        (
            prog3D,
            2,
            sin(box0.rotation.y),
            cos(box0.rotation.y)
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
        const GLsizei indexCount = static_cast<GLsizei>(std::size(indexData));    // インデックス数
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // 基本図形の種類
            indexCount,         // インデックスデータ数
            GL_UNSIGNED_SHORT,  // インデックスデータの型
            0,                  // インデックスデータの開始位置
            1                   // 描画する図形の数
        );

        // ふたつめの図形の描画
        glProgramUniform4fv
        (
            prog3D,     // プログラムオブジェクトの管理番号
            100,        // 送り先ロケーション番号
            1,          // データ数
            box1.color  // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,         // プログラムオブジェクトの管理番号
            0,              // 送り先ロケーション番号
            1,              // データ数
            &box1.scale.x   // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,             // プログラムオブジェクトの管理番号
            1,                  // 送り先ロケーション番号
            1,                  // データ数
            &box1.position.x    // データのアドレス
        );
        glProgramUniform2f
        (
            prog3D,
            2,
            sin(box1.rotation.y),
            cos(box1.rotation.y)
        );
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // 基本図形の種類
            indexCount,         // インデックスデータ数
            GL_UNSIGNED_SHORT,  // インデックスデータの型
            0,                  // インデックスデータの開始位置
            1                   // 描画する図形の数
        );

        // VAOの割り当てを解除
        // 引数 : 割り当てる頂点属性配列の管理番号
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