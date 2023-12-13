/**
* @file GameObject.cpp
*/
#include "GameObject.h"
#include <algorithm>

/// <summary>
/// �Q�[���I�u�W�F�N�g����폜�\��̃R���|�[�l���g���폜����
/// </summary>
void GameObject::RemoveDestroyedComponent()
{
    if (components.empty())
    {
        return; // �R���|�[�l���g�������Ă��Ȃ���Ή������Ȃ�
    }

    // �j���\��̗L���ŃR���|�[�l���g�𕪂���
    const auto iter =
        std::stable_partition
        (
            components.begin(), // �͈͂̐擪
            components.end(),   // �͈͂̏I�[
            // �U�蕪������
            [](const auto& p)
            {
                return !p->IsDestroyed();
            });

    // �j���\��̃R���|�[�l���g��ʂ̔z��Ɉړ�
    // ���� : ���ƂɂȂ�C�e���[�^
    std::vector<ComponentPtr> destroyList(
        std::move_iterator(iter),
        std::move_iterator(components.end()));

    // �z�񂩂�ړ��ς݃R���|�[�l���g���폜
    components.erase(iter, components.end());

    // �j���\��̃R���|�[�l���g��OnDestory�����s
    for (auto& e : destroyList)
    {
        e->OnDestroy();
    }

    // �����Ŏ��ۂɃR���|�[�l���g���폜�����(destoyList�̎������I��邽��)
}

/// <summary>
/// �X�^�[�g�C�x���g
/// </summary>
void GameObject::Start()
{
    for (auto& e : components)
    {
        if (!e->isStarted)
        {
            e->Start();
            e->isStarted = true;
        }
    }
}

/// <summary>
/// �X�V�C�x���g
/// </summary>
/// <param name="deltaTime">�O��̍X�V����̌o�ߎ���(�b)</param>
void GameObject::Update(float deltaTime)
{
    for (auto& e : components)
    {
        e->Update(deltaTime);
    }
    RemoveDestroyedComponent();
}

/// <summary>
/// �Փ˃C�x���g
/// </summary>
/// <param name="self">�Փ˂����R���|�[�l���g(����)</param>
/// <param name="other">�Փ˂����R���|�[�l���g(����)</param>
void GameObject::OnCollision(
    const ComponentPtr& self, const ComponentPtr& other)
{
    for (auto& e : components)
    {
        e->OnCollision(self, other);
    }
}

/// <summary>
/// �폜�C�x���g
/// </summary>
void GameObject::OnDestroy()
{
    for (auto& e : components)
    {
        e->OnDestroy();
    }
}