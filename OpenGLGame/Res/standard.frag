/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// �V�F�[�_�ւ̓���
layout(location=0) in vec4 inColor; // ���_�F

// �e�N�X�`���T���v��
layout(binding=0) uniform sampler2D texColor;

// �o�͂���F�f�[�^
// out�C���q : �V�F�[�_����̏o�͂̊i�[
out vec4 outColor;

void main()
{
	vec4 c = 
		texture
		(
			texColor,				// �T���v���ϐ�
			gl_FragCoord.xy * 0.01	// �e�N�X�`�����W
		);
	outColor = c * inColor;
}