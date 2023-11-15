/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLAD���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
#include <GLFW/glfw3.h> // GLFW���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
#include <Windows.h>    // �E�B���h�E�Y�p�̊֐�����`����Ă���w�b�_�t�@�C��
#include <string>       // ������^�╶���񑀍�֐��Ȃǂ���`����Ă���w�b�_�t�@�C��
#include <vector>       // ���I�z��^"vector�N���X"����`����Ă���w�b�_�t�@�C��
#include <fstream>      // ifstream�N���X����`����Ă���w�b�_�t�@�C��
#include <filesystem>   // filesystem���O��Ԃ���`����Ă���w�b�_�t�@�C��
#include <stdio.h>      // �W�����o�̓��C�u�����̊֐�����`����Ă���w�b�_�t�@�C��

/// <summary>
/// OpenGL����̃��b�Z�[�W����������R�[���o�b�N�֐�
/// </summary>
/// <param name="source">���b�Z�[�W�̔��M��(OpenGL�AWindows�A�V�F�[�_�Ȃ�)</param>
/// <param name="type">���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)</param>
/// <param name="id">���b�Z�[�W����ʂɎ��ʂ���l</param>
/// <param name="severity">���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)</param>
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
    const float vertexData[][3] =
    {
        {-0.2f,-0.2f, 0}, { 0.2f,-0.2f, 0}, { 0.0f, 0.2f, 0},
        {-0.8f,-0.2f, 0}, {-0.4f,-0.2f, 0}, {-0.4f, 0.2f, 0}, {-0.8f, 0.2f, 0},
        {0.8f,-0.2f, 0}, {0.4f,-0.2f, 0}, {0.4f, 0.2f, 0}, {0.8f, 0.2f, 0},
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
        0, 1, 2,
        3, 4, 5, 5, 6, 3,
        7, 8, 9, 9, 10, 7,
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
    // ->OpenGL�R���e�L�X�g��VAO�����蓖�Ă��Ă���ꍇ�A
    //   �o�b�t�@�I�u�W�F�N�g��VAO�ɂ����蓖�Ă�
    glBindBuffer
    (
        GL_ELEMENT_ARRAY_BUFFER,  // ���蓖�Ă�I�u�W�F�N�g�̎��
        ibo                       // ���蓖�Ă�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
    );

    // VBO��OpenGL�R���e�L�X�g�Ƀo�C���h(IBO�ƈقȂ�AVAO�ɂ̓o�C���h����Ȃ�)
    glBindBuffer
    (
        GL_ARRAY_BUFFER,  // ���蓖�Ă�I�u�W�F�N�g�̎��
        vbo               // ���蓖�Ă钸�_�o�b�t�@�Ǘ��ԍ�
    );

    // 0�Ԗڂ̒��_������L����
    // ���� : �L���ɂ��钸�_�����z��̃C���f�b�N�X
    glEnableVertexAttribArray(0);

    // 0�Ԗڂ̒��_������ݒ�
    // ���̂Ƃ��AOpenGL�R���e�L�X�g�Ƀo�C���h����Ă���VBO���A���_�����Ƀo�C���h�����
    glVertexAttribPointer
    (
        0,        // ���_�����z��̃C���f�b�N�X
        3,        // �f�[�^�̗v�f��
        GL_FLOAT, // �f�[�^�̌^
        GL_FALSE, // ���K���̗L��
        0,        // ���̃f�[�^�܂ł̃o�C�g��
        0         // �ŏ��̃f�[�^�̈ʒu
    );
#pragma endregion

#pragma region ���C�����[�v�̒�`
    // �E�B���h�E�̏I���v�������Ă��Ȃ�����(0)��,
    // ���C�����[�v�����𑱂���
    // ���� : GLFWwindow�ւ̃|�C���^
    while (!glfwWindowShouldClose(window))
    {
        // �o�b�N�o�b�t�@����������Ƃ��Ɏg�p����F���w��
        glClearColor
        (
            0.3f,   // R(��)
            0.6f,   // G(��)
            0.9f,   // B(��)
            1.0f    // A(�����x)
        );
        // �o�b�N�o�b�t�@���N���A
        // GL_COLOR_BUFFER_BIT : �J���[�o�b�t�@(�F)
        // GL_DEPTH_BUFFER_BIT : �[�x�o�b�t�@(���s��)
        // GL_STENCIL_BUFFER_BIT : �X�e���V���o�b�t�@(�؂蔲��)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // VAO��OpenGL�R���e�L�X�g�Ɋ��蓖�Ă�    
        // ���� : ���蓖�Ă钸�_�����z��̊Ǘ��ԍ�
        glBindVertexArray(vao);

        // �`��Ɏg���V�F�[�_���w��
        glUseProgram(prog3D);

        // ���j�t�H�[���ϐ��Ƀf�[�^���R�s�[
        const float timer = static_cast<float>(glfwGetTime());
        glProgramUniform1f(prog3D, 0, timer * 0.5f);

        // �}�`��`��
        glDrawElementsInstanced
        (
            GL_TRIANGLES,       // ��{�}�`�̎��
            15,                 // �C���f�b�N�X�f�[�^��
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