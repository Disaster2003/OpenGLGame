/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Engine/Scene.h"
#include <vector>

/**
* メインゲーム画面
*/
class MainGameScene : public Scene
{
public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;
	virtual bool Initialize(Engine& engine) override;

private:
	char GetMapData(int x, int y) const
	{
		return map[y * mapSizeX + x];
	}
	char& GetMapData(int x, int y)
	{
		return map[y * mapSizeX + x];
	}

	std::vector<char> map; // マップデータ
	int mapSizeX = 11;     // マップの広さ(横)
	int mapSizeY = 11;     // マップの広さ(縦)
	float squareSize = 2;  // マップの1マスの大きさ
	float squareScale = squareSize / 2; // 壁・床の拡大率
};

#endif // MAINGAMESCENE_H_INCLUDED