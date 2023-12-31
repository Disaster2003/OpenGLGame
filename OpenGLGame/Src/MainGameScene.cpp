/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "Engine/Engine.h"
#include <string>
#include <fstream>

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
    floor->texColor = std::make_shared<Texture>("Res/floor.tga");
    floor->meshId = 0;

    // 壁を作成
    auto texWall = std::make_shared<Texture>("Res/wall.tga");
    auto texCrystalBlue = std::make_shared<Texture>("Res/MeshData/crystal_blue.tga");
    for (int y = 0; y < mapSizeY; ++y)
    {
        for (int x = 0; x < mapSizeX; ++x)
        {
            const float posX = static_cast<float>(x + 0.5) * squareSize;
            const float posZ = static_cast<float>(y + 0.5) * squareSize;
            if (GetMapData(x, y) == '#')
            {
                auto wall = engine.Create<GameObject>(
                    "wall", { posX, 0, posZ });
                wall->scale = { squareScale, squareScale, squareScale };
                wall->texColor = texWall;
                wall->meshId = 2;
            }
            else if (GetMapData(x, y) == 'C')
            {
                // クリスタルを配置
                auto crystal = engine.Create<GameObject>("crystal", { posX, 1, posZ });
                crystal->scale = { 0.5f, 0.5f, 0.5f };
                crystal->texColor = texCrystalBlue;
                crystal->meshId = 1;
            }
        } // for x
    } // for y

    return true; // 初期化成功
}