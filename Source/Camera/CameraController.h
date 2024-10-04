#pragma once
#include <memory>
#include "System/Alias/OwnerAlias.h"

class CameraComponent;

// �J�����R���g���[���[�̊��N���X
class CameraControllerBase
{
public:
    CameraControllerBase(OwnerPtr owner) : owner_Wptr(owner) {}
    virtual ~CameraControllerBase(){}

    // �X�V�֐�
    virtual void Update(float elapsed_time) = 0;
    // ���O�擾
    virtual const char* GetName() const = 0;

    std::shared_ptr<CameraComponent> GetCamera();

    const bool& GetIsActive() { return this->is_active; }
    void SetIsActive(bool is_active) { this->is_active = is_active; }
protected:
    OwnerWPtr owner_Wptr;
    std::weak_ptr<CameraComponent> camera_Wptr;
private:
    bool is_active = true;

#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() = 0;
#endif // _DEBUG
};

