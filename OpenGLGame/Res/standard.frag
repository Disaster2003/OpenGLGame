/**
* @file standard.frag
*/
#version 450 // GLSL ver. * 100

// シェーダへの入力
layout(location=0) in vec4 inColor; // 頂点色

// テクスチャサンプラ
layout(binding=0) uniform sampler2D texColor;

// 出力する色データ
// out修飾子 : シェーダからの出力の格納
out vec4 outColor;

void main()
{
	vec4 c = 
		texture
		(
			texColor,				// サンプラ変数
			gl_FragCoord.xy * 0.01	// テクスチャ座標
		);
	outColor = c * inColor;
}