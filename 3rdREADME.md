# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.図形の大小の変更,位置の制御
### 1-1.図形の大小の変更
`standard.vert`
```diff
 // プログラムからの入力
 // uniform変数
 // ->シェーダプログラムに
 // C++プログラムから値を渡すための変数
+layout(location=0) uniform vec3 scale; // 拡大率
 
 void main()
 {
   outTexcoord = inTexcoord;
+  vec3 pos = inPosition * scale;
   gl_Position = 
 	vec4
 	(
 		pos,
 		1.0
 	);
 }
```
`Main.cpp`
```diff
     // 1番目の頂点属性を設定
     glVertexAttribPointer
     (
         1,                              // 頂点属性配列のインデックス
         2,                              // データの要素数
         GL_FLOAT,                       // データの型
         GL_FALSE,                       // 正規化の有無
         sizeof(Vertex),                 // 次のデータまでのバイト数
         reinterpret_cast<const void*>   // 最初のデータの位置
         (
             // 構造体の先頭から特定のメンバまでの
             // バイト数の計算
             offsetof
             (
                 Vertex,     // 構造体名
                 texcoord    // メンバ名
             )
         )
     );
 #pragma endregion
 
 #pragma region 物体のパラメータ
     class GameObject
     {
     public:
+        vec3 scale = { 1,1,1 };          // 物体の拡大率
         float color[4] = { 1, 1, 1, 1 }; // 物体の色
     };
     GameObject box0;
+    box0.scale = { 0.1f,0.1f,0.1f };
 #pragma endregion
 
 #pragma region テクスチャの作成
```
```diff
 // 変数ユニフォームにデータワット
 glProgramUniform4fv
 (
     prog3D,     // プログラムオブジェクトの管理番号
     100,        // 送り先ロケーション番号
     1,          // データ数
     box0.color  // データのアドレス
 );
+glProgramUniform3fv
+(
+    prog3D,         // プログラムオブジェクトの管理番号
+    0,              // 送り先ロケーション番号
+    1,              // データ数
+    &box0.scale.x   // データのアドレス
+);
 
 // 描画に使うテクスチャを
 // (テクスチャ・イメージ・ユニットに)割り当て
 glBindTextures
 (
```

## 課題01
内容

図形のX, Y, Z軸すべての大きさを

0.2にしなさい.
`Main.cpp`
```C++
     // 1番目の頂点属性を設定
     glVertexAttribPointer
     (
         1,                              // 頂点属性配列のインデックス
         2,                              // データの要素数
         GL_FLOAT,                       // データの型
         GL_FALSE,                       // 正規化の有無
         sizeof(Vertex),                 // 次のデータまでのバイト数
         reinterpret_cast<const void*>   // 最初のデータの位置
         (
             // 構造体の先頭から特定のメンバまでの
             // バイト数の計算
             offsetof
             (
                 Vertex,     // 構造体名
                 texcoord    // メンバ名
             )
         )
     );
 #pragma endregion
 
 #pragma region 物体のパラメータ
     class GameObject
     {
     public:
         vec3 scale = { 1,1,1 };         // 物体の拡大率
         float color[4] = { 1, 1, 1, 1 };// 物体の色
     };
     GameObject box0;
     box0.scale = { 0.2f,0.2f,0.2f };
 #pragma endregion
 
 #pragma region テクスチャの作成
```

### 1-2.平行移動
`standard.vert`
```diff
 // シェーダからの出力
 layout(location=1) out vec2 outTexcoord;    // テクスチャ座標
 
 // プログラムからの入力
 // uniform変数
 // ->シェーダプログラムに
 // C++プログラムから値を渡すための変数
 layout(location=0) uniform vec3 scale; // 拡大率
+layout(location=1) uniform vec3 position; // 位置
 
 void main()
 {
   outTexcoord = inTexcoord;
+  vec3 pos = inPosition * scale + position;
   gl_Position = 
 	vec4
 	(
 		pos,
 		1.0
 	);
 }
```
`Main.cpp`
```diff
     // 1番目の頂点属性を設定
     glVertexAttribPointer
     (
         1,                              // 頂点属性配列のインデックス
         2,                              // データの要素数
         GL_FLOAT,                       // データの型
         GL_FALSE,                       // 正規化の有無
         sizeof(Vertex),                 // 次のデータまでのバイト数
         reinterpret_cast<const void*>   // 最初のデータの位置
         (
             // 構造体の先頭から特定のメンバまでの
             // バイト数の計算
             offsetof
             (
                 Vertex,     // 構造体名
                 texcoord    // メンバ名
             )
         )
     );
 #pragma endregion
 
 #pragma region 物体のパラメータ
     class GameObject
     {
     public:
+        vec3 position = { 0, 0, 0 };    // 物体の位置
         vec3 scale = { 1,1,1 };         // 物体の拡大率
         float color[4] = { 1, 1, 1, 1 };// 物体の色
     };
     GameObject box0;
     box0.scale = { 0.2f,0.2f,0.2f };
+    box0.position = { -0.6f,-0.6f,0 };
 #pragma endregion
 
 #pragma region テクスチャの作成
```
```diff
 glProgramUniform3fv
 (
     prog3D,         // プログラムオブジェクトの管理番号
     0,              // 送り先ロケーション番号
     1,              // データ数
     &box0.scale.x   // データのアドレス
 );
+glProgramUniform3fv
+(
+    prog3D,             // プログラムオブジェクトの管理番号
+    1,                  // 送り先ロケーション番号
+    1,                  // データ数
+    &box0.position.x    // データのアドレス
+);
 
 // 描画に使うテクスチャを
 // (テクスチャ・イメージ・ユニットに)割り当て
 glBindTextures
 (
     0,      // 割り当て開始インデックス
     1,      // 割り当てる個数
     &tex    // テクスチャ管理番号配列のアドレス
 );
```

## 課題02
内容

図形の座標を変更し,

画面の好きな位置に表示しなさい.
```C++
     // 1番目の頂点属性を設定
     glVertexAttribPointer
     (
         1,                              // 頂点属性配列のインデックス
         2,                              // データの要素数
         GL_FLOAT,                       // データの型
         GL_FALSE,                       // 正規化の有無
         sizeof(Vertex),                 // 次のデータまでのバイト数
         reinterpret_cast<const void*>   // 最初のデータの位置
         (
             // 構造体の先頭から特定のメンバまでの
             // バイト数の計算
             offsetof
             (
                 Vertex,     // 構造体名
                 texcoord    // メンバ名
             )
         )
     );
 #pragma endregion
 
 #pragma region 物体のパラメータ
     class GameObject
     {
     public:
         vec3 position = { 0, 0, 0 };    // 物体の位置
         vec3 scale = { 1,1,1 };         // 物体の拡大率
         float color[4] = { 1, 1, 1, 1 };// 物体の色
     };
     GameObject box0;
     box0.scale = { 0.2f,0.2f,0.2f };
     box0.position = { ?,?,0 };
 #pragma endregion
 
 #pragma region テクスチャの作成
```