/**
* @file Engine.cpp
*/
#include "Engine.h"
#include "../PlayerComponent.h"
#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
#pragma warning(pop)

#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル

// 図形データ
#include "../../Res/MeshData/crystal_mesh.h"
#include "../../Res/MeshData/wall_mesh.h"

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
/// シェーダファイルを読み込んでコンパイル
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
/// ゲームエンジンの実行
/// </summary>
/// <returns>0 : 正常に実行が完了,
/// 0以外 : エラーが発生</returns>
int Engine::Run()
{
    const int result = Initialize();
    if (result)
    {
        return result;
    }
    // ウィンドウの終了要求が来ていなかった(0)時,
    // メインループ処理を続ける
    // 引数 : GLFWwindowへのポインタ
    while (!glfwWindowShouldClose(window))
    {
        Update();
        Render();
        RemoveDestroyedGameObject();
    }
    // GLFWライブラリの終了
    glfwTerminate();
    return 0;
}

/// <summary>
/// ゲームエンジンから全てのゲームオブジェクトの破棄
/// </summary>
void Engine::ClearGameObjectAll()
{
    for (auto& e : gameObjects)
    {
        e->OnDestroy();
    }
    gameObjects.clear();
}

/// <summary>
/// ゲームエンジンの初期化
/// </summary>
/// <returns>0 : 正常に初期化,
/// 0以外 : エラーが発生した</returns>
int Engine::Initialize()
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
    vs =
        CompileShader
        (
            GL_VERTEX_SHADER,
            "Res/standard.vert"
        );
    fs =
        CompileShader
        (
            GL_FRAGMENT_SHADER,
            "Res/standard.frag"
        );

    // プログラムオブジェクトの作成
    prog3D = glCreateProgram();
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
        vbo,                    // 頂点バッファの管理番号
        sizeof(Vertex)*10'000,  // コピーするバイト数(1万頂点までOK)
        nullptr,                // コピーするデータのアドレス
        0                       // 各種フラグ
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
    indexCount = static_cast<GLsizei>(std::size(indexData));  // インデックス数
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
        ibo,                        // インデックスバッファの管理番号
        sizeof(uint16_t)*40'000,    // コピーするバイト数(4万インデックスまでOK)
        nullptr,                    // コピーするデータのアドレス
        0                           // 各種フラグ
    );
#pragma endregion

#pragma region 図形データの情報
    struct MeshData
    {
        size_t vertexSize;      // 頂点データのバイト数
        size_t indexSize;       // インデックスデータのバイト数
        const void* vertexData; // 頂点データのアドレス
        const void* indexData;  // インデックスデータのアドレス
    };
    const MeshData meshes[] =
    {
      { sizeof(vertexData), sizeof(indexData), vertexData, indexData },
      { sizeof(crystal_vertices), sizeof(crystal_indices),crystal_vertices, crystal_indices },
      { sizeof(wall_vertices), sizeof(wall_indices),wall_vertices, wall_indices },
    };
    // 図形データから描画パラメータを作成し、データをGPUメモリにコピーする
    drawParamsList.reserve(std::size(meshes));
    for (const auto& e : meshes)
    {
        // 図形データをGPUメモリにコピー
        GLuint tmp[2];  // 一時的なバッファ
        // コピーするデータを入れる一時的なGPU側バッファを作成
        glCreateBuffers
        (
            2,     // 作成するオブジェクト数
            tmp    // 頂点バッファの管理番号
        );
        // 作成した一時的なGPU側バッファに,CPU側にあるデータをコピー
        glNamedBufferStorage
        (
            tmp[0],        // 一時的なバッファの管理番号
            e.vertexSize,  // コピーするバイト数
            e.vertexData,  // コピーするデータのアドレス
            0              // 各種フラグ
        );
        // 作成した一時的なGPU側バッファに,CPU側にあるデータをコピー
        glNamedBufferStorage
        (
            tmp[1],       // 一時的なバッファの管理番号
            e.indexSize,  // コピーするバイト数
            e.indexData,  // コピーするデータのアドレス
            0             // 各種フラグ
        );
        // 一時的なバッファから,既存のバッファにコピー
        glCopyNamedBufferSubData
        (
            tmp[0],         // コピー元バッファの管理番号
            vbo,            // コピー先バッファの管理番号
            0,              // コピー元の読み取り開始位置
            vboSize,        // コピー先の書き込み開始位置
            e.vertexSize    // コピーするバイト数
        );
        // 一時的なバッファから,既存のバッファにコピー
        glCopyNamedBufferSubData
        (
            tmp[1],         // コピー元バッファの
            ibo,            // コピー先バッファの
            0,              // コピー元の読み取り
            iboSize,        // コピー先の書き込み
            e.indexSize     // コピーするバイト数
        );
        // 一時的なバッファの削除
        glDeleteBuffers
        (
            2,     // 削除するオブジェクト数
            tmp    // 一時的なバッファの管理番号
        );

        // 描画パラメータを作成
        DrawParams params;
        params.mode = GL_TRIANGLES;
        params.count = static_cast<GLsizei>(e.indexSize / sizeof(uint16_t));
        params.indices = reinterpret_cast<void*>(iboSize);
        params.baseVertex = static_cast<GLint>(vboSize / sizeof(Vertex));
        drawParamsList.push_back(params); // 描画パラメータを配列に追加

        // バッファの現在のサイズを更新
        vboSize += e.vertexSize;
        iboSize += e.indexSize;
    }
#pragma endregion

#pragma region 頂点データ形式の設定
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

#pragma region boxのパラメータ
    auto& box0 = *Create<GameObject>("box0");
    box0.scale = { 0.2f,0.2f,0.2f };
    box0.position = { 0.6f,0.6f,-1 };

    auto& box1 = *Create<GameObject>("box1");
    box1.color[1] = 0.5f; // 緑成分の明るさを半分にしてみる
    box1.scale = { 0.2f, 0.2f, 0.2f };
    box1.position = { 0, 0, -1 };
#pragma endregion

#pragma region カメラ操作用コンポーネントを追加
    auto player = Create<GameObject>("player", { 0, 10, 0 });
    player->AddComponent<PlayerComponent>();
#pragma endregion

#pragma region ゲームオブジェクト配列の容量を予約
    gameObjects.reserve(1000);
#pragma endregion

#pragma region カメラの初期設定
    camera.position = { 3, 1, 3 };
    camera.rotation.y = 3.14159265f;
#pragma endregion

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

    // シーンの切り替え
    if (nextScene)
    {
        if (scene)
        {
            scene->Finalize(*this);
        }
        nextScene->Initialize(*this);
        scene = std::move(nextScene);
    }

    // シーンを更新
    if (scene)
    {
        scene->Update(*this, deltaTime);
    }

    UpdateGameObject(deltaTime);
}

/// <summary>
/// ゲームエンジンの状態を描画
/// </summary>
void Engine::Render()
{
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

    // ゲームオブジェクトを描画
    for (const auto& e : gameObjects)
    {
        // 図形番号がリストにない場合は描画しない
        if (e->meshId < 0 || e->meshId >= drawParamsList.size())
        {
            continue;
        }

        // ユニフォーム変数にデータワット
        glProgramUniform4fv
        (
            prog3D,     // プログラムオブジェクトの管理番号
            100,        // 送り先ロケーション番号
            1,          // データ数
            e->color    // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,         // プログラムオブジェクトの管理番号
            0,              // 送り先ロケーション番号
            1,              // データ数
            &e->scale.x     // データのアドレス
        );
        glProgramUniform3fv
        (
            prog3D,             // プログラムオブジェクトの管理番号
            1,                  // 送り先ロケーション番号
            1,                  // データ数
            &e->position.x      // データのアドレス
        );
        glProgramUniform2f
        (
            prog3D,
            2,
            sin(e->rotation.y),
            cos(e->rotation.y)
        );

        // 描画に使うテクスチャを
        // (テクスチャ・イメージ・ユニットに)割り当て
        if (e->texColor)
        {
            const GLuint tex = *e->texColor;
            glBindTextures
            (
                0,      // 割り当て開始インデックス
                1,      // 割り当てる個数
                &tex    // テクスチャ管理番号配列のアドレス
            );
        }

        // 図形の描画
        const DrawParams& params = drawParamsList[e->meshId];
        glDrawElementsInstancedBaseVertex
        (
            params.mode,        // 図形の種類
            params.count,       // インデックスデータ数
            GL_UNSIGNED_SHORT,  // インデックスデータの型
            params.indices,     // インデックスデータの開始位置
            1,                  // 描画する図形の数
            params.baseVertex   // インデックス0とみなす頂点データの位置
        );
    }

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

/// <summary>
/// ゲームオブジェクトの状態を更新
/// </summary>
/// <param name="deltaTime">前回の更新からの経過時間(秒)</param>
void Engine::UpdateGameObject(float deltaTime)
{
    // 要素の追加に対応するため添字を選択
    for (int i = 0; i < gameObjects.size(); ++i)
    {
        GameObject* e = gameObjects[i].get();
        if (!e->IsDestroyed())
        {
            e->Start();
            e->Update(deltaTime);
        }
    }
} // UpdateGameObject

/// <summary>
/// 破棄予定のゲームオブジェクトの削除
/// </summary>
void Engine::RemoveDestroyedGameObject()
{
    if (gameObjects.empty())
    {
        return; // ゲームオブジェクトを持っていなければ何もしない
    }

    // 破棄予定の有無でゲームオブジェクトを分ける
    const auto iter = std::partition(
        gameObjects.begin(), gameObjects.end(),
        [](const GameObjectPtr& e)
        {
            return !e->IsDestroyed();
        });

    // 破棄予定のオブジェクトを別の配列に移動
    GameObjectList destroyList(
        std::make_move_iterator(iter),
        std::make_move_iterator(gameObjects.end()));

    // 配列から移動済みオブジェクトを削除
    gameObjects.erase(iter, gameObjects.end());

    // 破棄予定のオブジェクトのOnDestroyを実行
    for (auto& e : destroyList)
    {
        e->OnDestroy();
    }

    // ここで実際にゲームオブジェクトが削除される(destoyListの寿命が終わるため)
}