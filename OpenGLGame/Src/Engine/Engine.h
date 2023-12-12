/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED	// �C���N���[�h�K�[�h
#define ENGINE_H_INCLUDED	// �C���N���[�h�K�[�h
#include "glad/glad.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>
#include <string>

/// <summary>
/// �Q�[���G���W��
/// </summary>
class Engine
{
public:
	Engine() = default;
	~Engine() = default;
	int Run();

private:
	GLFWwindow* window = nullptr;           // �E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame"; // �E�B���h�E�^�C�g��
	GLuint vs = 0;							// ���_�V�F�[�_�̊Ǘ��ԍ�
	GLuint fs = 0;							// �t���O�����g�V�F�[�_�̊Ǘ��ԍ�
	GLuint prog3D = 0;						// �V�F�[�_�v���O�����̊Ǘ��ԍ�
	GLuint vbo = 0;							// ���_�o�b�t�@�̊Ǘ��ԍ�
	GLuint ibo = 0;							// �C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
	GLuint vao = 0;							// ���_�����z��̊Ǘ��ԍ�
	GLuint tex = 0;							// �e�N�X�`��

	GameObject camera;						// �J�����I�u�W�F�N�g
	GameObject box0;
	GameObject box1;
};

#endif						// !ENGINE_H_INCLUDED(�C���N���[�h�K�[�h)