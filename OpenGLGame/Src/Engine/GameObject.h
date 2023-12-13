/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "Component.h"
#include "VecMath.h"
#include <string>       // ������^�╶���񑀍�֐��Ȃǂ���`����Ă���w�b�_�t�@�C��
#include <vector>       // ���I�z��^"vector�N���X"����`����Ă���w�b�_�t�@�C��
#include <memory>

// ��s�錾
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>; // �Q�[���I�u�W�F�N�g�̃|�C���^
using GameObjectList = std::vector<GameObjectPtr>; // �Q�[���I�u�W�F�N�g�|�C���^�̔z��

/// <summary>
/// �Q�[���ɓo�ꂷ��l�X�ȃI�u�W�F�N�g��\����{�N���X
/// </summary>
class GameObject
{
    friend Engine;
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    // �Q�[���G���W�����擾
    Engine* GetEngine() const
    {
        return engine;
    }

    // �Q�[���I�u�W�F�N�g���G���W������폜����
    void Destroy()
    {
        isDestroyed = true;
    }

    // �Q�[���I�u�W�F�N�g���j�󂳂�Ă�����true��Ԃ�
    bool IsDestroyed() const
    {
        return isDestroyed;
    }

    // �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
    template<typename T>
    std::shared_ptr<T> AddComponent()
    {
        auto p = std::make_shared<T>();
        p->owner = this;
        components.push_back(p);
        p->Awake();
        return p;
    }

    // �Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
    void RemoveDestroyedComponent();

    // �C�x���g����
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void OnCollision(
        const ComponentPtr& self, const ComponentPtr& other);
    virtual void OnDestroy();

    std::string name;                   // �I�u�W�F�N�g��
    vec3 position = { 0, 0, 0 };        // ���̂̈ʒu
    vec3 rotation = { 0, 0, 0 };        // ���̂̉�]�p�x
    vec3 scale = { 1,1,1 };             // ���̂̊g�嗦
    float color[4] = { 1, 1, 1, 1 };    // ���̂̐F

private:
    Engine* engine = nullptr;             // �G���W���̃A�h���X
    bool isDestroyed = false;             // ���S�t���O
    std::vector<ComponentPtr> components; // �R���|�[�l���g�z��
};

#endif // !GAMEOBJECT_H_INCLUDED