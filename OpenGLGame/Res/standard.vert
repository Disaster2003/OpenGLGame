/**
* @file standard.vert
*/
#version 450 // GLSL ver. * 100

// シェーダへの入力
// layout修飾子 : 型やステージに固有の属性の設定
// location : 他のプログラムが変数にアクセスするための番号の付与
// in修飾子 : バーテックスプラーステージから送られた値の設定
//            (glVertexArrayAttribPointer関数によって指定した頂点データ)
layout(location=0) in vec3 inPosition; // 頂点座標
layout(location=1) in vec2 inTexcoord; // テクスチャ座標

// シェーダからの出力
layout(location=1) out vec2 outTexcoord;    // テクスチャ座標

// プログラムからの入力
// uniform変数
// ->シェーダプログラムに
// C++プログラムから値を渡すための変数

void main()
{
  outTexcoord = inTexcoord;
  gl_Position = vec4(inPosition, 1.0);
}