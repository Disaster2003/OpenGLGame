/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include "glad/glad.h"
#include <string>
#include <memory>

// 先行宣言
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class Texture
{
public:
	explicit Texture(const char* filename);
	~Texture();

	// コピーと代入を禁止
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// 管理番号を取得
	operator GLuint() const
	{
		return id;
	}

	// 幅と高さを取得
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}

	// 名前を取得
	const std::string& GetName() const
	{
		return name;
	}

private:
	std::string name; // テクスチャ名(主にデバッグ用)
	GLuint id = 0;    // オブジェクト管理番号
	int width = 0;    // テクスチャの幅
	int height = 0;   // テクスチャの高さ
};

#endif // TEXTURE_H_INCLUDED