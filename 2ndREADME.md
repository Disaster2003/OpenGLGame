# OpenGLGame
-tn-mai講師に習ったことをout-putする場所-

[tn-mai講師:今年度のtext](https://github.com/tn-mai/OpenGL3D2023)

## 1.グラフィックス・パイプライン
<p align="center">
<img src="Image/graphics_pipeline.png" width="100%" />
</p>

## 1-1.VertexPuller
GPUメモリからひとつずつ頂点データ

(VertexShaderが実際に必要とする情報

(「頂点アトリビュート」で設定))を取り出し,

VertexShaderに渡す

## 1-2.VertexShader
VertexPullerから受け取った頂点データで

プリミティブの回転,拡大縮小,移動のほか,

座標系の変換,頂点単位のライティングなど,

様々な操作を行う

## 1-3.TessellationControlShader
新たに作成する小さなプリミティブの頂点データを,

どこに,どのように作成するかを指定する

## 1-4.TessellationPrimitiveGeneration
TessellationControlShaderからの情報で,

実際に小さなプリミティブへと分割し,

TessellationEvaluationShaderに渡す

## 1-5.TessellationEvaluationShader
TessellationPrimitiveGenerationからの情報で,

プリミティブの座標や色を変形し,

実際に複雑なディテールを加える

## 1-6.GeometryShader
ひとつのプリミティブの全頂点データを受け取り,

0個以上のプリミティブの頂点データを生成し,

PrimitiveAssemblyに渡す

## 1-7.PrimitiveAssembly
GeometryShaderで変換された

1つ以上の頂点データを蓄積し,

プリミティブを構成できる数になった段階で,

Rasterizationに渡す

## 1-8.Rasterization(RS)
PrimitiveAssemblyからのプリミティブをもとに,

フラグメントと呼ばれるピクセル片を生成し,

FragmentShaderに渡す

また,裏向きのプリミティブを除去(カリング)や,

プリミティブの画面に表示されない部分を

切り捨てる(クリッピング)

また,「ビューポート」で

「クリッピング空間」から,

「スクリーン空間」への変換を行う

## 1-9.FragmentShader
Rasterizationから送られてきた

フラグメントから,実際の色を計算する

## 1-10.Per-FragmentOperations
FragmentShaderから送られてきた

色情報と深度情報を,

「RenderBuffer」や「DepthBuffer」,

「StencilBuffer」と呼ばれる,

メモリ上の領域に書き込む

## 1-11.Render Buffer(描画した画像を格納するメモリ上の領域)
プリミティブを描画する

## 2.シェーダ
OpenGLのシェーダは

GLSL(OpenGL Shading Language)という

言語で記述する

GLSLはC言語をベースにして,

シェーダにとって必要な機能や型を追加し,

不要な機能を削除した言語である

### 2-1.シェーダの記述
ctrl+shift+A

->`プロジェクト名\Res\standard.vert`

->`追加`をクリック

->shift+alt+A

->`Res\standard.vert`をクリック

->`追加`をクリック

`standard.vert`
```diff
+/**
+* @file standard.vert
+*/
+#version 450 // GLSLver * 100
+
+// シェーダへの入力
+// layout修飾子 : 型やステージに固有の属性の設定
+// location : 他のプログラムが変数にアクセスするための番号の付与
+// in修飾子 : バーテックスプラーステージから送られた値の設定
+//            (glVertexArrayAttribPointer関数によって指定した頂点データ)
+layout(location=0) in vec3 inPosition; // 頂点座標
+
+void main()
+{
+  gl_Position = vec4(inPosition, 1);
+}
```

ctrl+shift+A

->`プロジェクト名\Res\standard.frag`

->`追加`をクリック

->shift+alt+A

->`Res\standard.frag`をクリック

->`追加`をクリック

`standard.frag`
```diff
+/**
+* @file standard.frag
+*/
+#version 450 // GLSLver * 100
+
+// 出力する色データ
+// out修飾子 : シェーダからの出力の格納
+out vec4 outColor;
+
+void main()
+{
+  outColor = vec4(1.0, 1.0, 0.2, 1.0);
+}
```

### 2-2.シェーダのコンパイルとリンク
作成したシェーダファイルを

グラフィックス・パイプラインで

使えるようにするには...

>1. 頂点シェーダファイルを読み込む
>2. 頂点シェーダをコンパイルする
>3. フラグメントシェーダファイルを読み込む
>4. フラグメントシェーダをコンパイルする
>5. 頂点シェーダとフラグメントシェーダをリンクする

`Main.cpp`
```diff
 #include <string>       // 文字列型や文字列操作関数などが定義されているヘッダファイル
+#include <vector>       // 動的配列型"vectorクラス"が定義されているヘッダファイル
+#include <fstream>      // ifstreamクラスが定義されているヘッダファイル
+#include <filesystem>   // filesystem名前空間が定義されているヘッダファイル
+#include <stdio.h>      // 標準入出力ライブラリの関数が定義されているヘッダファイル
 
 /// <summary>
 /// OpenGLからのメッセージを処理するコールバック関数
 /// </summary>
```

```diff
   // 出力ウィンドウへの表示
   // 引数 : 出力する文字列(エラーメッセージ)
   OutputDebugStringA(s.c_str());
 }
 
+/// <summary>
+/// シェーダファイルを読み込んでコンパイルする
+/// </summary>
+/// <param name="type">シェーダの種類</param>
+/// <param name="filename">シェーダファイル名</param>
+/// <returns>シェーダの管理番号</returns>
+GLuint CompileShader
+(
+    GLenum type,
+    const char* filename
+)
+{
+    // ファイルを開く
+    // std::ios::binary : バイナリモードの指定
+    std::ifstream file
+    (
+        filename,
+        std::ios::binary
+    );
+    // ファイルが正常に開けない(false)時,
+    // エラーメッセージを出力して,
+    // 0(オブジェクトが存在しない)を返して終了
+    if (!file)
+    {
+        char s[256];
+        snprintf
+        (
+            s,
+            256,
+            "[ERROR] %s: %sを開けません\n",
+            __func__,
+            filename
+        );
+        OutputDebugStringA(s);
+        return 0;
+    }
+
+    // ファイルサイズの取得
+    // 引数 : ファイルパス
+    const size_t filesize = std::filesystem::file_size(filename);
+    // ファイルサイズ分の領域を用意
+    std::vector<char> buffer(filesize);
+    // ファイルの読み込み
+    file.read
+    (
+        buffer.data(),  // バッファアドレス
+        filesize        // 読み込むバイト数
+    );
+    // ファイルを閉じる
+    file.close();
+
+    const char* source[] = { buffer.data() };       // bufferの先頭アドレス
+    const GLint length[] = { int(buffer.size()) };  // bufferのサイズ
+    // 「シェーダを管理するオブジェクト」の作成
+    // 引数 : 作成するシェーダの種類
+    const GLuint object = glCreateShader(type);
+    // シェーダ文字列の設定
+    glShaderSource
+    (
+        object, // シェーダの管理番号
+        1,      // 文字列の数
+        source, // 文字列の配列
+        length  // 文字列長の配列
+    );
+    // シェーダ文字列のコンパイル
+    // 引数 : シェーダの管理番号
+    glCompileShader(object);
+    return object;
+}

 /// <summary>
 /// エントリーポイント
 /// </summary>
 int WINAPI WinMain
 (
```

```diff
 #pragma region メッセージコールバックの設定
     glDebugMessageCallback
     (
         DebugCallback,  // 呼び出す関数のアドレス
         nullptr         // 関数のuserParam引数に渡される値
     );
 #pragma endregion
 
+#pragma region シェーダのコンパイルとリンク
+    // シェーダを読み込んでコンパイル
+    const GLuint vs = 
+        CompileShader
+        (
+            GL_VERTEX_SHADER,
+            "Res/standard.vert"
+        );
+    const GLuint fs = 
+        CompileShader
+        (
+            GL_FRAGMENT_SHADER,
+            "Res/standard.frag"
+        );
+
+    // プログラムオブジェクトの作成
+    GLuint prog3D = glCreateProgram();
+    // 頂点シェーダの指定
+    glAttachShader
+    (
+        prog3D, // プログラムオブジェクトの管理番号
+        vs      // 頂点シェーダの管理番号
+    );
+    // フラグメントシェーダの指定
+    glAttachShader
+    (
+        prog3D, // プログラムオブジェクトの管理番号
+        fs      // フラグメントシェーダの管理番号
+    );
+    // 指定した2つのシェーダのリンク
+    // 引数 : プログラムオブジェクトの管理番号
+    glLinkProgram(prog3D);
+#pragma endregion
 
 #pragma region 頂点データをGPUメモリにコピー
     // 頂点データ(x,y,z座標が-1~+1の座標系における座標)
     const float vertexData[][3] =
     {
```

### 2-3.シェーダの使用
`Main.cpp`
```diff
 // VAOをOpenGLコンテキストに割り当てる    
 // 引数 : 割り当てる頂点属性配列の管理番号
 glBindVertexArray(vao);

+// 描画に使うシェーダを指定
+glUseProgram(prog3D);

 // 図形を描画
 glDrawElementsInstanced
 (
```

### 2-4.シェーダ-シェーダ,C++-シェーダのデータ移動
`アウト変数とイン変数の組み合わせについて,`

`ロケーション番号と型が一致しない場合,`

`シェーダのリンクに失敗する`

`standard.vert`
```diff
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
 
+// シェーダからの出力
+layout(location=0) out vec4 outColor; // 頂点色
+
+// プログラムからの入力
+// uniform変数
+// ->シェーダプログラムに
+// C++プログラムから値を渡すための変数
+layout(location=0) uniform float timer;
 
 void main()
 {
+  const vec4 colors[] = {
+    { 1, 0, 0, 1 }, // 赤
+    { 0, 1, 0, 1 }, // 緑
+    { 0, 0, 1, 1 }, // 青
+  };
+  int i = gl_VertexID + int(timer);
+  outColor = colors[i % 3];
 
   gl_Position = vec4(inPosition, 1);
 }
```

`standard.frag`
```diff
 /**
 * @file standard.frag
 */
 #version 450 // GLSL ver. * 100
 
+// シェーダへの入力
+layout(location=0) in vec4 inColor; // 頂点色
 
 // 出力する色データ
 // out修飾子 : シェーダからの出力の格納
 out vec4 outColor;
 
 void main()
 {
-  outColor = vec4(1.0, 1.0, 0.2, 1.0);
+  outColor = inColor;
 }
```

`Main.cpp`
```diff
 // 描画に使うシェーダを指定
 glUseProgram(prog3D);
 
+// ユニフォーム変数にデータをコピー
+// アプリ起動時からの経過時間の取得
+const float timer = static_cast<float>(glfwGetTime());
+// 変数ユニフォームにデータワット
+glProgramUniform1f
+(
+    prog3D,         // プログラムオブジェクトの管理番号
+    0,              // 送り先ロケーション番号
+    timer           // 送るデータ
+);
 
 // 図形を描画
 glDrawElementsInstanced
 (
```

## 課題01
内容

colors配列に白色の要素を追加して,

四角形が4色で塗られるようにしましょう.

`standard.vert`
```C++
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

  gl_Position = vec4(inPosition, 1);
```

## 課題02
内容

頂上の色が2.0秒間で切り替わるように,

C++プログラムまたはシェーダを変更しましょう.

`Main.cpp`
```C++
// 描画に使うシェーダを指定
glUseProgram(prog3D);

// ユニフォーム変数にデータをコピー
// アプリ起動時からの経過時間の取得
const float timer = static_cast<float>(glfwGetTime());
// 変数ユニフォームにデータワット
glProgramUniform1f
(
    prog3D,         // プログラムオブジェクトの管理番号
    0,              // 送り先ロケーション番号
    timer * 0.5f    // 送るデータ
);

// 図形を描画
glDrawElementsInstanced
(
```

## 3.テクスチャ
物体の表面の細かな模様や凹凸まで全てを

GL_LINESやGL_TRIANGLESで作成し,

色などの属性を指定すれば,

理論上はあらゆる物体を表現できる.

が,この方法では信じられない数の

プリミティブが必要である.

そこで,プリミティブに画像を貼り付ける

「テクスチャ・マッピング」という技法が考案される.

texture(テクスチャ) =「織物の出来栄え,手触り,物の質感」

テクスチャとシェーダを組み合わせることで,

物体の色だけでなく,凹凸や物体の濡れ具合といった

さまざまな状態を表すことができる.

### 3-1.テクスチャ元のダウンロード
PNG,JPG = データが「圧縮」されている(複雑)

JPG,BMP = 透明度なし

◎TGA = 読み込むだけ && 透明度あり

[box.png](https://github.com/tn-mai/OpenGL3D2023/blob/main/res/box.png)

box.pngをダウンロード

->VisualStudioにドラッグ&ドロップ

->`ファイル\名前を付けて保存`

->ファイルの種類を`TGAファイル(*.tga)`にして

->保存

### 3-2.テクスチャの表示
