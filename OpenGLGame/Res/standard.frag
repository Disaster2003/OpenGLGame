/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// シェーダへの入力
layout(location=0) in vec4 inColor; // 頂点色

// 出力する色データ
// out修飾子 : シェーダからの出力の格納
out vec4 outColor;

void main()
{
  outColor = inColor;
}