/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "Engine/Engine.h"
#include <string>
#include <fstream>

/**
* シーンを初期化する
*
* @retval true  初期化成功
* @retval false 初期化失敗
*/

/// <summary>
/// シーンの初期化
/// </summary>
/// <returns>true : 初期化成功
/// false : 初期化失敗</returns>
bool MainGameScene::Initialize(Engine& engine)
{
    // ファイルからマップデータを作成
    std::ifstream ifs("Res/maze00.txt");
    if (!ifs)
    {
        return false; // マップファイルの読み込みに失敗
    }

    ifs >> mapSizeX >> mapSizeY; // マップの大きさを読み込む
    ifs.ignore(); // 改行を飛ばす

    // マップデータを読み込む
    map.resize(mapSizeX * mapSizeY);
    for (int y = 0; y < mapSizeY; ++y)
    {
        std::string s;
        std::getline(ifs, s);
        for (int x = 0; x < mapSizeX; ++x)
        {
            GetMapData(x, y) = s[x];
        } // for x
    } // for y

   // 床を作成
    auto floor = engine.Create<GameObject>("floor");
    floor->scale = {
      static_cast<float>(mapSizeX) * squareScale, 1,
      static_cast<float>(mapSizeY) * squareScale };
    floor->position = { floor->scale.x, -1, floor->scale.z };
    floor->color[0] = 0.4f;
    floor->color[2] = 0.2f;

    // 壁を作成
    for (int y = 0; y < mapSizeY; ++y)
    {
        for (int x = 0; x < mapSizeX; ++x)
        {
            const float posX = static_cast<float>(x + 0.5) * squareSize;
            const float posZ = static_cast<float>(y + 0.5) * squareSize;
            if (GetMapData(x, y) == '#')
            {
                auto wall = engine.Create<GameObject>(
                    "wall", { posX, 0.5f * squareSize, posZ });
                wall->scale = { squareScale, squareScale, squareScale };
            }
        } // for x
    } // for y

    return true; // 初期化成功
}