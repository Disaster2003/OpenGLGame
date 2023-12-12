/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLADライブラリの関数が定義されているヘッダファイル
#include "Engine/Engine.h"
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
    Engine engine;
    return engine.Run();
}