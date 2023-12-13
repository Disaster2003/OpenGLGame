/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include "glad/glad.h"
#include <string>
#include <memory>

// ��s�錾
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/// <summary>
/// �e�N�X�`���Ǘ��N���X
/// </summary>
class Texture
{
public:
	explicit Texture(const char* filename);
	~Texture();

	// �R�s�[�Ƒ�����֎~
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// �Ǘ��ԍ����擾
	operator GLuint() const
	{
		return id;
	}

	// ���ƍ������擾
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}

	// ���O���擾
	const std::string& GetName() const
	{
		return name;
	}

private:
	std::string name; // �e�N�X�`����(��Ƀf�o�b�O�p)
	GLuint id = 0;    // �I�u�W�F�N�g�Ǘ��ԍ�
	int width = 0;    // �e�N�X�`���̕�
	int height = 0;   // �e�N�X�`���̍���
};

#endif // TEXTURE_H_INCLUDED