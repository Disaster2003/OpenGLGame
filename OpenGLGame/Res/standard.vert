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
layout(location=0) out vec4 outColor;       // 頂点色
layout(location=1) out vec2 outTexcoord;    // テクスチャ座標

// プログラムからの入力
// uniform変数
// ->シェーダプログラムに
// C++プログラムから値を渡すための変数
layout(location=0) uniform float timer;

void main()
{
  const vec4 colors[] = {
    { 1, 0, 0, 1 }, // 赤
    { 0, 1, 0, 1 }, // 緑
    { 0, 0, 1, 1 }, // 青
    { 1, 1, 1, 1 }, // 白
  };
  int i = gl_VertexID + int(timer);
  outColor = colors[i % 4];
  outTexcoord = inTexcoord;
  gl_Position = vec4(inPosition, 1);
}