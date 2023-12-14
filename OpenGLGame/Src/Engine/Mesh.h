/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "glad/glad.h"
#include "VecMath.h"

/// <summary>
/// ���_�f�[�^�`��
/// </summary>
struct Vertex
{
	vec3 position; // ���_���W
	vec2 texcoord; // �e�N�X�`�����W
};

/// <summary>
/// �`��p�����[�^
/// </summary>
struct DrawParams
{
	GLenum mode = GL_TRIANGLES; // �v���~�e�B�u�̎��
	GLsizei count = 0;          // �`�悷��C���f�b�N�X��
	const void* indices = 0;    // �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
	GLint baseVertex = 0;       // �C���f�b�N�X0�ƂȂ钸�_�z����̈ʒu
};

#endif // MESH_H_INCLUDED