/**
* @file Main.cpp
*/
#include "glad/glad.h"  // GLAD���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
#include <GLFW/glfw3.h> // GLFW���C�u�����̊֐�����`����Ă���w�b�_�t�@�C��
#include <Windows.h>    // �E�B���h�E�Y�p�̊֐�����`����Ă���w�b�_�t�@�C��
#include <string>       // ������^�╶���񑀍�֐��Ȃǂ���`����Ă���w�b�_�t�@�C��

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

#pragma region ���C�����[�v�̒�`
    // �E�B���h�E�̏I���v�������Ă��Ȃ�����(0)��,
    // ���C�����[�v�����𑱂���
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

        // �o�b�N�o�b�t�@�̕`��I����,
        // �t�����g�o�b�t�@�Ɩ����̌���
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