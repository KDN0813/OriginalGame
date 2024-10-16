#pragma once
#include <memory>
#include <vector>

class CircleComponent;

// �~�̓����蔻��
class CircleCollider
{
public:
    CircleCollider() {};
    ~CircleCollider() {};

    // �O���ŌĂԍX�V�֐�
    void Update();

    // �����蔻����s��circle��ǉ�����֐�
    void AddCircle(std::shared_ptr<CircleComponent> circle);
private:
    // �����蔻����s���֐�
    void CheckCollision();

    // ���ɍ폜���ꂽcircle�̃R���e�i�����菜���֐�
    void RemoveDeletedCircle();
private:
    std::vector<std::weak_ptr<CircleComponent>> circle_attacker_pool;   // �Ԃ��鑤
    std::vector<std::weak_ptr<CircleComponent>> circle_deffender_pool;  // �Ԃ����鑤
};

