/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <vector>       // ���I�z��^"vector�N���X"����`����Ă���w�b�_�t�@�C��
#include <fstream>      // ifstream�N���X����`����Ă���w�b�_�t�@�C��
#include <filesystem>   // filesystem���O��Ԃ���`����Ă���w�b�_�t�@�C��

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // �E�B���h�E�Y�p�̊֐�����`����Ă���w�b�_�t�@�C��
#pragma warning(pop)

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="filename">�e�N�X�`���t�@�C����</param>
/// <returns>�e�N�X�`���̊Ǘ��ԍ�</returns>
Texture::Texture(const char* filename)
{
    // �t�@�C�����J��
    // std::ios::binary : �o�C�i�����[�h�̎w��
    std::ifstream file
    (
        filename,
        std::ios::binary
    );
    // �t�@�C��������ɊJ���Ȃ�(false)��,
    // �G���[���b�Z�[�W���o�͂���,
    // 0(�I�u�W�F�N�g�����݂��Ȃ�)��Ԃ��ďI��
    if (!file)
    {
        char s[256];
        snprintf
        (
            s,
            256,
            "[ERROR] %s: %s���J���܂���\n",
            __func__,
            filename
        );
        OutputDebugStringA(s);
        return;
    }

    // �t�@�C���T�C�Y�̎擾
    // ���� : �t�@�C���p�X
    const size_t filesize = std::filesystem::file_size(filename);
    // �t�@�C���T�C�Y���̗̈��p��
    std::vector<uint8_t> buffer(filesize);
    // �t�@�C���̓ǂݍ���
    file.read
    (
        reinterpret_cast<char*>(buffer.data()),   // �o�b�t�@�A�h���X
        filesize                                  // �ǂݍ��ރo�C�g��
    );
    // �t�@�C�������
    file.close();

    // �w�b�_��������擾
    const size_t tgaHeaderSize = 18; // �w�b�_���̃o�C�g��
    width = buffer[12] + buffer[13] * 256;
    height = buffer[14] + buffer[15] * 256;

    GLuint object = 0; // �e�N�X�`���̊Ǘ��ԍ�
    // �e�N�X�`���̍쐬
    glCreateTextures
    (
        GL_TEXTURE_2D,    // �e�N�X�`���̎��
        1,                // �쐬�����
        &object           // �ԍ����i�[����z��̃A�h���X
    );
    // �摜�f�[�^����GPU�������̈�̊m��
    glTextureStorage2D
    (
        object,   // �e�N�X�`���̊Ǘ��ԍ�
        1,        // �쐬���郌�x����(�~�b�v�}�b�v)
        GL_RGBA8, // �s�N�Z���`��
        width,    // ��
        height    // ����
    );
    // �摜�f�[�^��GPU�������ɃR�s�[
    glTextureSubImage2D
    (
        object,                       // �e�N�X�`���̊Ǘ��ԍ�
        0,                            // �R�s�[��̃��C���[�ԍ�
        0,                            // �R�s�[���X���W(�����̉摜���ЂƂɂ܂Ƃ߂�p ex)�X�v���C�g�V�[�g,�e�N�X�`���A�g���X)
        0,                            // �R�s�[���Y���W(�����̉摜���ЂƂɂ܂Ƃ߂�p ex)�X�v���C�g�V�[�g,�e�N�X�`���A�g���X)
        width,                        // �R�s�[����摜�̕�
        height,                       // �R�s�[����摜�̍���
        GL_BGRA,                      // �s�N�Z���Ɋ܂܂��v�f�Ə���
        GL_UNSIGNED_BYTE,             // �v�f�̌^
        buffer.data() + tgaHeaderSize // �摜�f�[�^�̃A�h���X
    );

    id = object;
    name = filename;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Texture::~Texture()
{
    // �e�N�X�`���I�u�W�F�N�g�̍폜
    glDeleteTextures
    (
        1,
        &id
    );
}