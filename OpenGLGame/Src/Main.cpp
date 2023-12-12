/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLAD���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
#include "Engine/Engine.h"
#include <GLFW/glfw3.h> // GLFW���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // �E�B���h�E�Y�p�̊֐�����`����Ă���w�b�_�t�@�C��
#pragma warning(pop)

#include <string>       // ������^�╶���񑀍�֐��Ȃǂ���`����Ă���w�b�_�t�@�C��
#include <vector>       // ���I�z��^"vector�N���X"����`����Ă���w�b�_�t�@�C��
#include <fstream>      // ifstream�N���X����`����Ă���w�b�_�t�@�C��
#include <filesystem>   // filesystem���O��Ԃ���`����Ă���w�b�_�t�@�C��
#include <stdio.h>      // �W�����o�̓��C�u�����̊֐�����`����Ă���w�b�_�t�@�C��

/// <summary>
/// OpenGL����̃��b�Z�[�W����������R�[���o�b�N�֐�
/// </summary>
/// <param name="source">���b�Z�[�W�̔��M��(OpenGL,Windows,�V�F�[�_�Ȃ�)</param>
/// <param name="type">���b�Z�[�W�̎��(�G���[,�x���Ȃ�)</param>
/// <param name="id">���b�Z�[�W����ʂɎ��ʂ���l</param>
/// <param name="severity">���b�Z�[�W�̏d�v�x(��,��,��,�Œ�)</param>
/// <param name="length">���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă���</param>
/// <param name="message">���b�Z�[�W�{��</param>
/// <param name="userParam">�R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^</param>
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
  // ���b�Z�[�W�̕�������
  // �����̎�,0�I�[("\0")
  if (length < 0)
  {
    s = message;
  }
  // ���b�Z�[�W�̓ǂݍ���
  else
  {
    s.assign(message, message + length);
  }
  s += '\n'; // ���b�Z�[�W�ɂ͉��s���Ȃ��̂Œǉ�����
  // �o�̓E�B���h�E�ւ̕\��
  // ���� : �o�͂��镶����(�G���[���b�Z�[�W)
  OutputDebugStringA(s.c_str());
}

/// <summary>
/// �V�F�[�_�t�@�C����ǂݍ���ŃR���p�C������
/// </summary>
/// <param name="type">�V�F�[�_�̎��</param>
/// <param name="filename">�V�F�[�_�t�@�C����</param>
/// <returns>�V�F�[�_�̊Ǘ��ԍ�</returns>
GLuint CompileShader
(
    GLenum type,
    const char* filename
)
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
        return 0;
    }

    // �t�@�C���T�C�Y�̎擾
    // ���� : �t�@�C���p�X
    const size_t filesize = std::filesystem::file_size(filename);
    // �t�@�C���T�C�Y���̗̈��p��
    std::vector<char> buffer(filesize);
    // �t�@�C���̓ǂݍ���
    file.read
    (
        buffer.data(),  // �o�b�t�@�A�h���X
        filesize        // �ǂݍ��ރo�C�g��
    );
    // �t�@�C�������
    file.close();

    const char* source[] = { buffer.data() };       // buffer�̐擪�A�h���X
    const GLint length[] = { int(buffer.size()) };  // buffer�̃T�C�Y
    // �u�V�F�[�_���Ǘ�����I�u�W�F�N�g�v�̍쐬
    // ���� : �쐬����V�F�[�_�̎��
    const GLuint object = glCreateShader(type);
    // �V�F�[�_������̐ݒ�
    glShaderSource
    (
        object, // �V�F�[�_�̊Ǘ��ԍ�
        1,      // ������̐�
        source, // ������̔z��
        length  // �����񒷂̔z��
    );
    // �V�F�[�_������̃R���p�C��
    // ���� : �V�F�[�_�̊Ǘ��ԍ�
    glCompileShader(object);
    return object;
}

/// <summary>
/// �e�N�X�`���̓ǂݍ���
/// </summary>
/// <param name="filename">�e�N�X�`���t�@�C����</param>
/// <returns>�e�N�X�`���̊Ǘ��ԍ�</returns>
GLuint LoadTexture(const char* filename)
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
  if ( ! file)
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
    return 0;
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
  const int width = buffer[12] + buffer[13] * 256;
  const int height = buffer[14] + buffer[15] * 256;

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
  return object;
}

/// <summary>
/// �G���g���[�|�C���g
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