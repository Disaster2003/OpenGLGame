/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// シェーダへの入力
layout(location=0) in vec4 inColor;		// 頂点色
layout(location=1) in vec2 inTexcoord;	// テクスチャ座標

// テクスチャサンプラ
// sampler : 「テクスチャ・イメージ・ユニット」に
//            割り当てられたテクスチャを使う
// binding : サンプラ変数が使用するユニットを選択する
layout(binding=0) uniform sampler2D texColor;

// 出力する色データ
// out修飾子 : シェーダからの出力の格納
out vec4 outColor;

void main()
{
	vec4 c = 
		// テクスチャの読み込み
		texture
		(
			texColor,
			inTexcoord
		);
	outColor = c * inColor;
}