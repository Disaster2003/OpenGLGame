/**
* @file standard.frag
*/
#version 450 // GLSLver * 100

// 出力する色データ
// out修飾子 : シェーダからの出力の格納
out vec4 outColor;

void main()
{
  outColor = vec4(1.0, 1.0, 0.2, 1.0);
}