/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// �V�F�[�_�ւ̓���
layout(location=0) in vec4 inColor; // ���_�F

// �o�͂���F�f�[�^
// out�C���q : �V�F�[�_����̏o�͂̊i�[
out vec4 outColor;

void main()
{
  outColor = inColor;
}