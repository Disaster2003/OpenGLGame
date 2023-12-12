/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED	// インクルードガード
#define ENGINE_H_INCLUDED	// インクルードガード
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

/// <summary>
/// ゲームエンジン
/// </summary>
class Engine
{
public:
	Engine() = default;
	~Engine() = default;
	int Run();

private:
};

#endif	// ENGINE_H_INCLUDED(インクルードガード)