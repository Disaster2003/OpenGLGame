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

// アスペクト比と視野角による拡大率
layout(location=3) uniform vec2 aspectRatioAndScaleFov;

layout(location=4) uniform vec3 cameraPosition; // カメラの位置
layout(location=5) uniform vec2 cameraSinCosY;  // カメラのY軸回転

void main()
{
	outTexcoord = inTexcoord;

	// ローカル座標系からワールド座標系に変換
	// スケール
	vec3 pos = inPosition * scale;
	
	// Y軸回転
	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;

	//// X軸回転
	//v.zy=vec2(v.z*sinCosX.y+v.y*sinCosX.x,-v.z*sinCosX.x+v.y*sinCosX.y);
	
	// 平行移動
	gl_Position.xyz += position;

	// ワールド座標系からビュー座標系に変換
	// x=x*cos+z*sin
	// z=x*-sin+z*cos
    pos = gl_Position.xyz - cameraPosition;
    float cameraSinY = cameraSinCosY.x;
    float cameraCosY = cameraSinCosY.y;
    gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
    gl_Position.y = pos.y;
    gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

	//// y=z*-sin+y*cos
	//// z=z*cos+y*sin
	//pos=gl_Position.xyz;
	//float cameraSinX=cameraSinCosX.x;
	//float cameraCosX=cameraSinCosX.y;
	//gl_Position.x=pos.x;
	//gl_Position.y=pos.y*cameraCosX+pos.z*-cameraSinX;
	//gl_Position.z=pos.z*cameraCosX+pos.y*cameraSinX;

    // ビュー座標系からクリップ座標系に変換
	gl_Position.xy *= aspectRatioAndScaleFov;

	// 深度値の計算結果が-1〜+1になるようなパラメータA, Bを計算
	const float near = 0.5;
	const float far = 1000;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	// 遠近法の有効化
	gl_Position.w = -gl_Position.z;
	gl_Position.z = -gl_Position.z * B + A; // 深度値の補正
}