/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "Engine/Engine.h"
#include <string>
#include <fstream>

/**
* �V�[��������������
*
* @retval true  ����������
* @retval false ���������s
*/

/// <summary>
/// �V�[���̏�����
/// </summary>
/// <returns>true : ����������
/// false : ���������s</returns>
bool MainGameScene::Initialize(Engine& engine)
{
    // �t�@�C������}�b�v�f�[�^���쐬
    std::ifstream ifs("Res/maze00.txt");
    if (!ifs)
    {
        return false; // �}�b�v�t�@�C���̓ǂݍ��݂Ɏ��s
    }

    ifs >> mapSizeX >> mapSizeY; // �}�b�v�̑傫����ǂݍ���
    ifs.ignore(); // ���s���΂�

    // �}�b�v�f�[�^��ǂݍ���
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

   // �����쐬
    auto floor = engine.Create<GameObject>("floor");
    floor->scale = {
      static_cast<float>(mapSizeX) * squareScale, 1,
      static_cast<float>(mapSizeY) * squareScale };
    floor->position = { floor->scale.x, -1, floor->scale.z };
    floor->color[0] = 0.4f;
    floor->color[2] = 0.2f;

    // �ǂ��쐬
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

    return true; // ����������
}