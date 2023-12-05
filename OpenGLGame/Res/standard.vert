/**
* @file standard.vert
*/
#version 450 // GLSL ver. * 100

// �V�F�[�_�ւ̓���
// layout�C���q : �^��X�e�[�W�ɌŗL�̑����̐ݒ�
// location : ���̃v���O�������ϐ��ɃA�N�Z�X���邽�߂̔ԍ��̕t�^
// in�C���q : �o�[�e�b�N�X�v���[�X�e�[�W���瑗��ꂽ�l�̐ݒ�
//            (glVertexArrayAttribPointer�֐��ɂ���Ďw�肵�����_�f�[�^)
layout(location=0) in vec3 inPosition; // ���_���W
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

// �V�F�[�_����̏o��
layout(location=1) out vec2 outTexcoord;    // �e�N�X�`�����W

// �v���O��������̓���
// uniform�ϐ�
// ->�V�F�[�_�v���O������
// C++�v���O��������l��n�����߂̕ϐ�

void main()
{
  outTexcoord = inTexcoord;
  gl_Position = vec4(inPosition, 1.0);
}