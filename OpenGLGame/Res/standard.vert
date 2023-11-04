/**
* @file standard.vert
*/
#version 450

// シェーダへの入力
layout(location=0) in vec3 inPosition; // 頂点座標

void main()
{
  gl_Position = vec4(inPosition, 1);
}