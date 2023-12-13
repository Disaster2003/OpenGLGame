/**
* @file PlayerComponent.h
*/
#ifndef PLAYERCOMPONENT_H_INCLUDED
#define PLAYERCOMPONENT_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"

/// <summary>
/// �v���C���[
/// </summary>
class PlayerComponent
	: public Component
{
public:
	PlayerComponent() = default;
	virtual ~PlayerComponent() = default;

    virtual void Update(float deltaTime) override
    {    
        Engine* engine = GetOwner()->GetEngine();
        GameObject & camera = engine->GetMainCamera();

        // glfwGetKey(GLFW�E�B���h�E�I�u�W�F�N�g�̃A�h���X,�L�[�ԍ�);
        // GLFW_RELEASE : �L�[���͂Ȃ�
        // GLFW_PRESS   : �L�[���͂���
        // �J�����̈ړ�
        const float cameraSpeed = 0.003f;
        const float cameraCos = cos(camera.rotation.y);
        const float cameraSin = sin(camera.rotation.y);
        if (engine->GetKey(GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.position.x -= cameraSpeed * cameraCos;
            camera.position.z -= cameraSpeed * -cameraSin;
        }
        if (engine->GetKey(GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.position.x += cameraSpeed * cameraCos;
            camera.position.z += cameraSpeed * -cameraSin;
        }
        if (engine->GetKey(GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.position.x -= cameraSpeed * cameraSin;
            camera.position.z -= cameraSpeed * cameraCos;
        }
        if (engine->GetKey(GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.position.x += cameraSpeed * cameraSin;
            camera.position.z += cameraSpeed * cameraCos;
        }

        //// �}�E�X�̓��͂̎擾
        //double mouseX, mouseY, mouseBeforeX, mouseBeforeY;
        //mouseBeforeX = mouseX;
        //mouseBeforeY = mouseY;
        //glfwGetCursorPos
        //(
        //  window,
        //  &mouseX,
        //  &mouseY
        // );
        //if (mouseBeforeX != mouseX)
        //{
        //    camera.rotation.y += 0.0005f * mouseX - mouseBeforeX;
        //}
        //if (mouseBeforeY != mouseY)
        //{
        //    camera.rotation.x += 0.0005f * mouseY - mouseBeforeY;
        //}

        // �J�����̉�]
        if (engine->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            camera.rotation.y -= 0.0005f;
        }
        if (engine->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            camera.rotation.y += 0.0005f;
        }
        //if (engine->GetKey(GLFW_KEY_UP) == GLFW_PRESS)
        //{
        //  camera.rotation.x += 0.0005f;
        //}
        //if (engine->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
        //{
        //  camera.rotation.x -= 0.0005f;
        //}
    }
};

#endif // !PLAYERCOMPONENT_H_INCLUDED