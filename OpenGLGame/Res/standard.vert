/**
* @file standard.vert
*/
#version 450 // GLSLver * 100

// �V�F�[�_�ւ̓���
// layout�C���q : �^��X�e�[�W�ɌŗL�̑����̐ݒ�
// location : ���̃v���O�������ϐ��ɃA�N�Z�X���邽�߂̔ԍ��̕t�^
// in�C���q : �o�[�e�b�N�X�v���[�X�e�[�W���瑗��ꂽ�l�̐ݒ�
//            (glVertexArrayAttribPointer�֐��ɂ���Ďw�肵�����_�f�[�^)
layout(location=0) in vec3 inPosition; // ���_���W

void main()
{
  gl_Position = vec4(inPosition, 1);
}