/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLAD���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
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
#pragma region GLFW���C�u�����̏�����
    // �������ɐ������Ȃ�����(!GLFW_TRUE)��,
    // 1��Ԃ��ďI��
    if (glfwInit() != GLFW_TRUE)
    {
        return 1; // ���������s
    }
#pragma endregion

#pragma region �`��E�B���h�E�̍쐬
    GLFWwindow* window = nullptr;           // �E�B���h�E�I�u�W�F�N�g
    const std::string title = "OpenGLGame"; // �E�B���h�E�^�C�g��
    // GLFW���C�u�������g���ăR���e�L�X�g�̎�ނ̐ݒ�
    // GLFW_TRUE : �f�o�b�O�R���e�L�X�g�̍쐬
    // GLFW_FALSE : �ʏ�̃R���e�L�X�g�̍쐬
    glfwWindowHint
    (
        GLFW_OPENGL_DEBUG_CONTEXT,  // �q���g�̎��
        GLFW_TRUE                   // �q���g�ɐݒ肷��l
    );
    window =
        //�O���t�B�b�N�X�`��p�E�B���h�E�̍쐬
        glfwCreateWindow
        (
            1280,           // �E�B���h�E�̕�
            720,            // �E�B���h�E�̍���
            title.c_str(),  // �^�C�g���o�[�ɕ\�����镶����
            nullptr,        // �\�����郂�j�^�̑I��
            nullptr         // ���\�[�X�����L����E�B���h�E
        );
    // �E�B���h�E�̍쐬�ɐ������Ȃ�����(nullptr)��,
    // 1��Ԃ��ďI��
    if (!window)
    {
        // GLFW���C�u�����̏I��
        glfwTerminate();
        return 1;   // �E�B���h�E�쐬���s
    }
#pragma endregion

#pragma region OpenGL�֐��̓���
    // OpenGL�R���e�L�X�g�̍쐬
    // ���� : GLFW�E�B���h�E�I�u�W�F�N�g�̃A�h���X
    glfwMakeContextCurrent(window);

    // gladLoadGLLoader : �K�v�Ȋ֐��̃A�h���X��S�Ď擾
    // glfwGetProcAddress : OpenGL�֐����ɑΉ�����֐��̃A�h���X��Ԃ�
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        // GLFW���C�u�����̏I��
        glfwTerminate();
        return 1; // �A�h���X�擾���s
    }
#pragma endregion

#pragma region ���b�Z�[�W�R�[���o�b�N�̐ݒ�
    glDebugMessageCallback
    (
        DebugCallback,  // �Ăяo���֐��̃A�h���X
        nullptr         // �֐���userParam�����ɓn�����l
    );
#pragma endregion

#pragma region �V�F�[�_�̃R���p�C���ƃ����N
    // �V�F�[�_��ǂݍ���ŃR���p�C��
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

    // �v���O�����I�u�W�F�N�g�̍쐬
    GLuint prog3D = glCreateProgram();
    // ���_�V�F�[�_�̎w��
    glAttachShader
    (
        prog3D, // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
        vs      // ���_�V�F�[�_�̊Ǘ��ԍ�
    );
    // �t���O�����g�V�F�[�_�̎w��
    glAttachShader
    (
        prog3D, // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
        fs      // �t���O�����g�V�F�[�_�̊Ǘ��ԍ�
    );
    // �w�肵��2�̃V�F�[�_�̃����N
    // ���� : �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
    glLinkProgram(prog3D);
#pragma endregion

#pragma region ���_�f�[�^��GPU�������ɃR�s�[
    // ���_�f�[�^(x,y,z���W��-1~+1�̍��W�n�ɂ�������W)
    struct vec2 { float x, y; };
    struct vec3 { float x, y, z; };
    struct Vertex
    {
        vec3 position; // ���_���W
        vec2 texcoord; // �e�N�X�`�����W
    };
    const Vertex vertexData[] =
    {
        // +Z(��O�̖�)
        { {-1,-1, 1 }, { 0, 0 } },
        { { 1,-1, 1 }, { 1, 0 } },
        { { 1, 1, 1 }, { 1, 1 } },
        { {-1, 1, 1 }, { 0, 1 } },

        // -Z(���̖�)
        { { 1,-1,-1 }, { 0, 0 } },
        { {-1,-1,-1 }, { 1, 0 } },
        { {-1, 1,-1 }, { 1, 1 } },
        { { 1, 1,-1 }, { 0, 1 } },

        // +Y(��̖�)
        { { 1, 1, 1 }, { 0, 0 } },
        { { 1, 1,-1 }, { 1, 0 } },
        { {-1, 1,-1 }, { 1, 1 } },
        { {-1, 1, 1 }, { 0, 1 } },
        
        // -Y(���̖�)
        { { 1,-1,-1 }, { 0, 0 } },
        { { 1,-1, 1 }, { 1, 0 } },
        { {-1,-1, 1 }, { 1, 1 } },
        { {-1,-1,-1 }, { 0, 1 } },
        
        // +X(�E�̖�)
        { { 1, 1, 1 }, { 0, 0 } },
        { { 1, 1,-1 }, { 1, 0 } },
        { { 1,-1,-1 }, { 1, 1 } },
        { { 1,-1, 1 }, { 0, 1 } },
        
        // -X(���̖�)
        { {-1, 1, 1 }, { 0, 0 } },
        { {-1, 1,-1 }, { 1, 0 } },
        { {-1,-1,-1 }, { 1, 1 } },
        { {-1,-1, 1 }, { 0, 1 } },
    };
    GLuint vbo = 0; // ���_�o�b�t�@�̊Ǘ��ԍ�
    // �o�b�t�@�I�u�W�F�N�g(GPU���������Ǘ����邽�߂̃I�u�W�F�N�g)�̍쐬
    glCreateBuffers
    (
        1,      // �쐬����I�u�W�F�N�g��
        &vbo    // ���_�o�b�t�@�̊Ǘ��ԍ����i�[����z��
    );
    // GPU���������m�ۂ̂�,�f�[�^���R�s�[
    // �f�[�^�̃A�h���X��nullptr�Ȃ�,
    // GPU�������̊m�ۂ̂�
    glNamedBufferStorage
    (
        vbo,                // ���_�o�b�t�@�̊Ǘ��ԍ�
        sizeof(vertexData), // �R�s�[����o�C�g��
        vertexData,         // �R�s�[����f�[�^�̃A�h���X
        0                   // �e��t���O
    );
#pragma endregion

#pragma region �C���f�b�N�X�f�[�^��GPU�������ɃR�s�[
    // �C���f�b�N�X�f�[�^(�}�`���\�����钸�_�ԍ�)
    const GLushort indexData[] =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16, 17,18,18,19, 16,
        20,21,22,22,23,20,
    };
    GLuint ibo = 0; // �C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
    // �o�b�t�@�I�u�W�F�N�g(GPU���������Ǘ����邽�߂̃I�u�W�F�N�g)�̍쐬
    glCreateBuffers
    (
        1,      // �쐬����I�u�W�F�N�g��
        &ibo    // �C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
    );
    // GPU���������m�ۂ̂�,�f�[�^���R�s�[
    // �f�[�^�̃A�h���X��nullptr�Ȃ�,
    // GPU�������̊m�ۂ̂�
    glNamedBufferStorage
    (
        ibo,               // �C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
        sizeof(indexData), // �R�s�[����o�C�g��
        indexData,         // �R�s�[����f�[�^�̃A�h���X
        0                  // �e��t���O
    );
#pragma endregion

#pragma region ���_�f�[�^�`���̐ݒ�
    GLuint vao = 0; // ���_�����z��̊Ǘ��ԍ�
    // ���_�����I�u�W�F�N�g�̍쐬
    glCreateVertexArrays
    (
        1,    // �쐬����I�u�W�F�N�g��
        &vao  // ���_�����z��̊Ǘ��ԍ����i�[����z��
    );
    // VAO��OpenGL�R���e�L�X�g�Ƀo�C���h(���蓖�Ă�)
    // ���� : ���蓖�Ă钸�_�����z��̊Ǘ��ԍ�
    glBindVertexArray(vao);

    // IBO��OpenGL�R���e�L�X�g��VAO�̗����Ƀo�C���h
    // (VAO��OpenGL�R���e�L�X�g�Ƀo�C���h����Ă��邽��)
    // GL_ELEMENT_ARRAY_BUFFER
    // ->OpenGL�R���e�L�X�g��VAO�����蓖�Ă��Ă���ꍇ,
    //   �o�b�t�@�I�u�W�F�N�g��VAO�ɂ����蓖�Ă�
    glBindBuffer
    (
        GL_ELEMENT_ARRAY_BUFFER,  // ���蓖�Ă�I�u�W�F�N�g�̎��
        ibo                       // ���蓖�Ă�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
    );

    // VBO��OpenGL�R���e�L�X�g�Ƀo�C���h(IBO�ƈقȂ�,VAO�ɂ̓o�C���h����Ȃ�)
    glBindBuffer
    (
        GL_ARRAY_BUFFER,  // ���蓖�Ă�I�u�W�F�N�g�̎��
        vbo               // ���蓖�Ă钸�_�o�b�t�@�Ǘ��ԍ�
    );

    // 0�Ԗڂ̒��_�����̗L����
    // ���� : �L���ɂ��钸�_�����z��̃C���f�b�N�X
    glEnableVertexAttribArray(0);

    // 0�Ԗڂ̒��_�����̐ݒ�
    // ���̂Ƃ�,OpenGL�R���e�L�X�g�Ƀo�C���h����Ă���VBO��,
    // ���_�����Ƀo�C���h�����
    glVertexAttribPointer
    (
        0,              // ���_�����z��̃C���f�b�N�X
        3,              // �f�[�^�̗v�f��
        GL_FLOAT,       // �f�[�^�̌^
        GL_FALSE,       // ���K���̗L��
        sizeof(Vertex), // ���̃f�[�^�܂ł̃o�C�g��
        0               // �ŏ��̃f�[�^�̈ʒu
    );

    // 1�Ԗڂ̒��_�����̗L����
    // ���� : �L���ɂ��钸�_�����z��̃C���f�b�N�X
    glEnableVertexAttribArray(1);

    // 1�Ԗڂ̒��_�����̐ݒ�
    glVertexAttribPointer
    (
        1,                              // ���_�����z��̃C���f�b�N�X
        2,                              // �f�[�^�̗v�f��
        GL_FLOAT,                       // �f�[�^�̌^
        GL_FALSE,                       // ���K���̗L��
        sizeof(Vertex),                 // ���̃f�[�^�܂ł̃o�C�g��
        reinterpret_cast<const void*>   // �ŏ��̃f�[�^�̈ʒu
        (
            // �\���̂̐擪�������̃����o�܂ł�
            // �o�C�g���̌v�Z
            offsetof
            (
                Vertex,     // �\���̖�
                texcoord    // �����o��
            )
        )
    );
#pragma endregion

#pragma region ���̂̃p�����[�^
    class GameObject
    {
    public:
        vec3 position = { 0, 0, 0 };    // ���̂̈ʒu
        vec3 rotation = { 0, 0, 0 };    // ���̂̉�]�p�x
        vec3 scale = { 1,1,1 };         // ���̂̊g�嗦
        float color[4] = { 1, 1, 1, 1 };// ���̂̐F
    };

    // �J�����I�u�W�F�N�g
    GameObject camera;

    GameObject box0;
    box0.scale = { 0.2f,0.2f,0.2f };
    box0.position = { 0.6f,0.6f,-1 };

    GameObject box1;
    box1.color[1] = 0.5f; // �ΐ����̖��邳�𔼕��ɂ��Ă݂�
    box1.scale = { 0.2f, 0.2f, 0.2f };
    box1.position = { 0, 0, -1 };
#pragma endregion

#pragma region �e�N�X�`���̍쐬
    GLuint tex = LoadTexture("Res/box.tga");
#pragma endregion

#pragma region ���C�����[�v�̒�`
    // �E�B���h�E�̏I���v�������Ă��Ȃ�����(0)��,
    // ���C�����[�v�����𑱂���
    // ���� : GLFWwindow�ւ̃|�C���^
    while (!glfwWindowShouldClose(window))
    {
        // box0�̉�]
        box0.rotation.y += 0.0001f;

        // glfwGetKey(GLFW�E�B���h�E�I�u�W�F�N�g�̃A�h���X,�L�[�ԍ�);
        // GLFW_RELEASE : �L�[���͂Ȃ�
        // GLFW_PRESS   : �L�[���͂���
        // �J�����̈ړ�
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

        //// �}�E�X�̓��͂̎擾
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

        // �J�����̉�]
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

        // �o�b�N�o�b�t�@����������Ƃ��Ɏg�p����F�̎w��
        glClearColor
        (
            0.3f,   // R(��)
            0.6f,   // G(��)
            0.9f,   // B(��)
            1.0f    // A(�����x)
        );
        // �o�b�N�o�b�t�@�̃N���A
        // GL_COLOR_BUFFER_BIT : �J���[�o�b�t�@(�F)
        // GL_DEPTH_BUFFER_BIT : �[�x�o�b�t�@(���s��)
        // GL_STENCIL_BUFFER_BIT : �X�e���V���o�b�t�@(�؂蔲��)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // VAO��OpenGL�R���e�L�X�g�Ɋ��蓖�Ă�    
        // ���� : ���蓖�Ă钸�_�����z��̊Ǘ��ԍ�
        glBindVertexArray(vao);

        // �`��Ɏg���V�F�[�_�̎w��
        glUseProgram(prog3D);

        // �t���[���o�b�t�@�̑傫�����擾
        int fbWidth, fbHeight;
        glfwGetFramebufferSize
        (
            window,     // GLFW�E�B���h�E�I�u�W�F�N�g�̃A�h���X
            &fbWidth,   // �`��E�B���h�E�̕����i�[����ϐ��̃A�h���X
            &fbHeight   // �`��E�B���h�E�̍������i�[����ϐ��̃A�h���X
        );

        // �r���[�|�[�g�̐ݒ�
        glViewport
        (
            0,          // �`��͈͂̍���X�̍��W
            0,          // �`��͈͂̍�����Y���W
            fbWidth,    // �`��͈͂̕�
            fbHeight    // �`��͈͂̍���
        );
    
        // �A�X�y�N�g��Ǝ���p�̐ݒ�
        const float aspectRatio =
          static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
        const float degFovY = 60; // ��������p
        const float radFovY = degFovY * 3.1415926535f / 180;
        const float scaleFov = tan(radFovY / 2); // ����p�ɂ��g�嗦
        glProgramUniform2f
        (
            prog3D,                         // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            3,                              // ����惍�P�[�V�����ԍ�
            1 / (aspectRatio * scaleFov),   // �f�[�^��
            1 / scaleFov                    // �f�[�^�̃A�h���X
        );

        // �J�����p�����[�^�̐ݒ�
        glProgramUniform3fv
        (
            prog3D,             // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            4,                  // ����惍�P�[�V�����ԍ�
            1,                  // �f�[�^��
            &camera.position.x  // �f�[�^�̃A�h���X
        );
        glProgramUniform2f
        (
            prog3D,                     // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            5,                          // ����惍�P�[�V�����ԍ�
            sin(-camera.rotation.y),    // �f�[�^��
            cos(-camera.rotation.y)     // �f�[�^�̃A�h���X
        );

        // �[�x�e�X�g�̗L����
        // glEnable : �w�肳�ꂽOpenGL�R���e�L�X�g�̗L����
        // ���� : �L���ɂ���@�\��\���}�N���萔
        // GL_DEPTH_TEST : �[�x�e�X�g
        glEnable(GL_DEPTH_TEST);

        // �ϐ����j�t�H�[���Ƀf�[�^���b�g
        glProgramUniform4fv
        (
            prog3D,     // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            100,        // ����惍�P�[�V�����ԍ�
            1,          // �f�[�^��
            box0.color  // �f�[�^�̃A�h���X
        );
        glProgramUniform3fv
        (
            prog3D,         // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            0,              // ����惍�P�[�V�����ԍ�
            1,              // �f�[�^��
            &box0.scale.x   // �f�[�^�̃A�h���X
        );
        glProgramUniform3fv
        (
            prog3D,             // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            1,                  // ����惍�P�[�V�����ԍ�
            1,                  // �f�[�^��
            &box0.position.x    // �f�[�^�̃A�h���X
        );
        glProgramUniform2f
        (
            prog3D,
            2,
            sin(box0.rotation.y),
            cos(box0.rotation.y)
        );

        // �`��Ɏg���e�N�X�`����
        // (�e�N�X�`���E�C���[�W�E���j�b�g��)���蓖��
        glBindTextures
        (
            0,      // ���蓖�ĊJ�n�C���f�b�N�X
            1,      // ���蓖�Ă��
            &tex    // �e�N�X�`���Ǘ��ԍ��z��̃A�h���X
        );

        // �}�`�̕`��
        const GLsizei indexCount = static_cast<GLsizei>(std::size(indexData));    // �C���f�b�N�X��
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // ��{�}�`�̎��
            indexCount,         // �C���f�b�N�X�f�[�^��
            GL_UNSIGNED_SHORT,  // �C���f�b�N�X�f�[�^�̌^
            0,                  // �C���f�b�N�X�f�[�^�̊J�n�ʒu
            1                   // �`�悷��}�`�̐�
        );

        // �ӂ��߂̐}�`�̕`��
        glProgramUniform4fv
        (
            prog3D,     // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            100,        // ����惍�P�[�V�����ԍ�
            1,          // �f�[�^��
            box1.color  // �f�[�^�̃A�h���X
        );
        glProgramUniform3fv
        (
            prog3D,         // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            0,              // ����惍�P�[�V�����ԍ�
            1,              // �f�[�^��
            &box1.scale.x   // �f�[�^�̃A�h���X
        );
        glProgramUniform3fv
        (
            prog3D,             // �v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�
            1,                  // ����惍�P�[�V�����ԍ�
            1,                  // �f�[�^��
            &box1.position.x    // �f�[�^�̃A�h���X
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
            GL_TRIANGLES,       // ��{�}�`�̎��
            indexCount,         // �C���f�b�N�X�f�[�^��
            GL_UNSIGNED_SHORT,  // �C���f�b�N�X�f�[�^�̌^
            0,                  // �C���f�b�N�X�f�[�^�̊J�n�ʒu
            1                   // �`�悷��}�`�̐�
        );

        // VAO�̊��蓖�Ă�����
        // ���� : ���蓖�Ă钸�_�����z��̊Ǘ��ԍ�
        glBindVertexArray(0);

        // �o�b�N�o�b�t�@�̕`��I����,
        // �t�����g�o�b�t�@�Ɩ����̌���
        // ���� : GLFWwindow�ւ̃|�C���^
        glfwSwapBuffers(window);

        // �uOS����̗v���v�̏���
        // (�L�[�{�[�h��}�E�X�Ȃǂ̏�Ԃ��擾����Ȃ�)
        glfwPollEvents();
    }
#pragma endregion

#pragma region GLFW���C�u�����̏I��
    glfwTerminate();
#pragma endregion

    return 0;
}