/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "glad/glad.h"
#include "VecMath.h"

/// <summary>
/// 頂点データ形式
/// </summary>
struct Vertex
{
	vec3 position; // 頂点座標
	vec2 texcoord; // テクスチャ座標
};

/// <summary>
/// 描画パラメータ
/// </summary>
struct DrawParams
{
	GLenum mode = GL_TRIANGLES; // プリミティブの種類
	GLsizei count = 0;          // 描画するインデックス数
	const void* indices = 0;    // 描画開始インデックスのバイトオフセット
	GLint baseVertex = 0;       // インデックス0となる頂点配列内の位置
};

#endif // MESH_H_INCLUDED