/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "VecMath.h"

class GameObject
{
public:
    vec3 position = { 0, 0, 0 };    // •¨‘Ì‚ÌˆÊ’u
    vec3 rotation = { 0, 0, 0 };    // •¨‘Ì‚Ì‰ñ“]Šp“x
    vec3 scale = { 1,1,1 };         // •¨‘Ì‚ÌŠg‘å—¦
    float color[4] = { 1, 1, 1, 1 };// •¨‘Ì‚ÌF
};

#endif // !GAMEOBJECT_H_INCLUDED