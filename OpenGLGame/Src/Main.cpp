/**
* @file Main.cpp
*/
#include "Engine/Engine.h"

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // �E�B���h�E�Y�p�̊֐�����`����Ă���w�b�_�t�@�C��
#pragma warning(pop)

/// <summary>
/// �G���g���[�|�C���g
/// </summary>
int WINAPI WinMain
(
    _In_ HINSTANCE hInstnce,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    Engine engine;
    return engine.Run();
}