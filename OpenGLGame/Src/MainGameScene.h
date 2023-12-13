/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Engine/Scene.h"
#include <vector>

/**
* ���C���Q�[�����
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

	std::vector<char> map; // �}�b�v�f�[�^
	int mapSizeX = 11;     // �}�b�v�̍L��(��)
	int mapSizeY = 11;     // �}�b�v�̍L��(�c)
	float squareSize = 2;  // �}�b�v��1�}�X�̑傫��
	float squareScale = squareSize / 2; // �ǁE���̊g�嗦
};

#endif // MAINGAMESCENE_H_INCLUDED