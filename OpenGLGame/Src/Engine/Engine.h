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

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���쐬����
	/// </summary>
	/// <param name="name">�I�u�W�F�N�g�̖��O</param>
	/// <param name="position">�I�u�W�F�N�g��z�u������W</param>
	/// <param name="rotation">�I�u�W�F�N�g�̉�]�p�x</param>
	template<typename T>
	std::shared_ptr<T> Create
	(
		const std::string& name,
		const vec3& position = { 0, 0, 0 },
		const vec3& rotation = { 0, 0, 0 }
	)
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		gameObjects.push_back(p); // �G���W���ɓo�^
		return p;
	}

	// ���ׂẴQ�[���I�u�W�F�N�g���폜����
	void ClearGameObjectAll();

private:
	int Initialize();
	void Update();
	void Render();
	void UpdateGameObject(float deltaTime);
	void RemoveDestroyedGameObject();

	GLFWwindow* window = nullptr;           // �E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame"; // �E�B���h�E�^�C�g��
	GLuint vs = 0;							// ���_�V�F�[�_�̊Ǘ��ԍ�
	GLuint fs = 0;							// �t���O�����g�V�F�[�_�̊Ǘ��ԍ�
	GLuint prog3D = 0;						// �V�F�[�_�v���O�����̊Ǘ��ԍ�
	GLuint vbo = 0;							// ���_�o�b�t�@�̊Ǘ��ԍ�
	GLuint ibo = 0;							// �C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
	GLuint vao = 0;							// ���_�����z��̊Ǘ��ԍ�
	GameObjectList gameObjects;				// �Q�[���I�u�W�F�N�g�z��
	double previousTime = 0;				// �O��X�V���̎���
	float deltaTime = 0;					// �O��X�V����̌o�ߎ���
	GLuint tex = 0;							// �e�N�X�`��

	GameObject camera;						// �J�����I�u�W�F�N�g
	GLsizei indexCount = 0;
};

#endif						// !ENGINE_H_INCLUDED(�C���N���[�h�K�[�h)