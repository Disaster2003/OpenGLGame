/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED	// インクルードガード
#define ENGINE_H_INCLUDED	// インクルードガード
#include "glad/glad.h"
#include "GameObject.h"
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
	GLFWwindow* window = nullptr;           // ウィンドウオブジェクト
	const std::string title = "OpenGLGame"; // ウィンドウタイトル
	GLuint vs = 0;							// 頂点シェーダの管理番号
	GLuint fs = 0;							// フラグメントシェーダの管理番号
	GLuint prog3D = 0;						// シェーダプログラムの管理番号
	GLuint vbo = 0;							// 頂点バッファの管理番号
	GLuint ibo = 0;							// インデックスバッファの管理番号
	GLuint vao = 0;							// 頂点属性配列の管理番号
	GLuint tex = 0;							// テクスチャ

	GameObject camera;						// カメラオブジェクト
	GameObject box0;
	GameObject box1;
};

#endif						// !ENGINE_H_INCLUDED(インクルードガード)