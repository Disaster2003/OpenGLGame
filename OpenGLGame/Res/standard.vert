/**
* @file standard.vert
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition; // ���_���W

void main()
{
  gl_Position = vec4(inPosition, 1);
}