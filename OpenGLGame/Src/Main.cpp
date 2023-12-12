/**
* @file Main.cpp
*/
#include "Engine/Engine.h"

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>    // ウィンドウズ用の関数が定義されているヘッダファイル
#pragma warning(pop)

/// <summary>
/// エントリーポイント
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