/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "VecMath.h"

/// <summary>
/// 物体のパラメータ
/// </summary>
class GameObject
{
public:
    vec3 position = { 0, 0, 0 };    // 物体の位置
    vec3 rotation = { 0, 0, 0 };    // 物体の回転角度
    vec3 scale = { 1,1,1 };         // 物体の拡大率
    float color[4] = { 1, 1, 1, 1 };// 物体の色
};

#endif // !GAMEOBJECT_H_INCLUDED