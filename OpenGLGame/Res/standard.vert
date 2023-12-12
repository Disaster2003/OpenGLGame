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
layout(location=0) uniform vec3 scale;			// �g�嗦
layout(location=1) uniform vec3 position;		// �ʒu
layout(location=2) uniform vec2 sinCosY;		// Y����]
layout(location=3) uniform float aspectRatio;	// �A�X�y�N�g��
layout(location=4) uniform vec3 cameraPosition; // �J�����̈ʒu
layout(location=5) uniform vec2 cameraSinCosY;  // �J������Y����]

void main()
{
	outTexcoord = inTexcoord;

	// ���[�J�����W�n���烏�[���h���W�n�ɕϊ�
	// �X�P�[��
	vec3 pos = inPosition * scale;
	
	// Y����]
	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;
	
	// ���s�ړ�
	gl_Position.xyz += position;

	// ���[���h���W�n����r���[���W�n�ɕϊ�
    pos = gl_Position.xyz - cameraPosition;
    float cameraSinY = cameraSinCosY.x;
    float cameraCosY = cameraSinCosY.y;
    gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
    gl_Position.y = pos.y;
    gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

    // �r���[���W�n����N���b�v���W�n�ɕϊ�
	gl_Position.x /= aspectRatio;

	// �[�x�l�̌v�Z���ʂ�-1�`+1�ɂȂ�悤�ȃp�����[�^A, B���v�Z
	const float near = 0.5;
	const float far = 1000;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	// ���ߖ@��L���ɂ���
	gl_Position.w = -gl_Position.z;
	gl_Position.z = -gl_Position.z * B + A; // �[�x�l��␳
}