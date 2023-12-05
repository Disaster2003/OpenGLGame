/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// �V�F�[�_�ւ̓���
layout(location=1) in vec2 inTexcoord;	// �e�N�X�`�����W

// �e�N�X�`���T���v��
// sampler : �u�e�N�X�`���E�C���[�W�E���j�b�g�v��
//            ���蓖�Ă�ꂽ�e�N�X�`�����g��
// binding : �T���v���ϐ����g�p���郆�j�b�g��I������
layout(binding=0) uniform sampler2D texColor;

// �v���O��������̓���
layout(location=100) uniform vec4 color; // ���̂̐F

// �o�͂���F�f�[�^
// out�C���q : �V�F�[�_����̏o�͂̊i�[
out vec4 outColor;

void main()
{
	vec4 c = 
		// �e�N�X�`���̓ǂݍ���
		texture
		(
			texColor,
			inTexcoord
		);
	outColor = c * color;
}