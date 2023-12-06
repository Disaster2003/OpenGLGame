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
layout(location=0) uniform vec3 scale;			// 拡大率
layout(location=1) uniform vec3 position;		// 位置
layout(location=2) uniform vec2 sinCosY;		// Y軸回転
layout(location=3) uniform float aspectRatio;	// アスペクト比

void main()
{
	outTexcoord = inTexcoord;
	// スケール
	vec3 pos = inPosition * scale;
	
	// Y軸回転
	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;
	
	// 平行移動
	gl_Position.xyz += position;

	// ワールド座標系からクリップ座標系に変換
	gl_Position.x /= aspectRatio;

	// 遠近法を有効にする
	gl_Position.zw = -gl_Position.zz;
}