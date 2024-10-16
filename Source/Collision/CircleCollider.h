#pragma once
#include <memory>
#include <vector>
#include <set>
#include "Collision/CollisionParam.h"

class CircleComponent;

// �~�̓����蔻��
class CircleCollider
{
public:
    CircleCollider();
    ~CircleCollider() {};

    // �O���ŌĂԍX�V�֐�
    void Update();

    // �����蔻����s��circle��ǉ�����֐�
    void AddCircle(std::shared_ptr<CircleComponent>& circle);
private:
    // �����蔻����s���֐�
    void CheckCollision();

    // ���ɍ폜���ꂽcircle�̃R���e�i�����菜���֐�
    void RemoveDeletedCircle();
private:
    // ����OBJECT_TYPE (��: PLAYER��ENEMY) �ɑ�����
    // �Ԃ��鑤 (ATTACKER) �ƂԂ����鑤 (DEFENDER) �̃f�[�^��ێ�����\����
    struct CircleCollision
    {
        std::vector<std::weak_ptr<CircleComponent>> circle_attacker_pool;   // �Ԃ��鑤 (ATTACKER)
        std::vector<std::weak_ptr<CircleComponent>> circle_defender_pool;   // �Ԃ����鑤 (DEFENDER)
    };
private:
    std::vector<CircleCollision> circle_collision_pool;
};

