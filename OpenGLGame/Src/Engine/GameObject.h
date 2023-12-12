/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "VecMath.h"

class GameObject
{
public:
    vec3 position = { 0, 0, 0 };    // ���̂̈ʒu
    vec3 rotation = { 0, 0, 0 };    // ���̂̉�]�p�x
    vec3 scale = { 1,1,1 };         // ���̂̊g�嗦
    float color[4] = { 1, 1, 1, 1 };// ���̂̐F
};

#endif // !GAMEOBJECT_H_INCLUDED