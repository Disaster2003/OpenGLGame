/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
#pragma warning(pop)

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="filename">テクスチャファイル名</param>
/// <returns>テクスチャの管理番号</returns>
Texture::Texture(const char* filename)
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
        return;
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
    width = buffer[12] + buffer[13] * 256;
    height = buffer[14] + buffer[15] * 256;

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

    id = object;
    name = filename;
}

/// <summary>
/// デストラクタ
/// </summary>
Texture::~Texture()
{
    // テクスチャオブジェクトの削除
    glDeleteTextures
    (
        1,
        &id
    );
}