#include "InstancingModelResourceManager.h"
#include "Graphics/Graphics.h"

std::shared_ptr<InstancingModelResource> InstancingModelResourceManager::LoadModelResource(ID3D11Device* device,const char* filename)
{
    // ���f������
    ModelMap::iterator it = instancing_models.find(filename);
    if (it != instancing_models.end())
    {
        // �����N���؂�Ă��Ȃ��ꍇ
        if (it->second.expired() == false)
        {
            // std::shared_ptr������ĕԂ�
            return it->second.lock();
        }
    }

    // �V�K���f�����\�[�X�쐬���ǂݍ���
    std::shared_ptr<InstancingModelResource> instancing_model 
        = std::make_shared<InstancingModelResource>(device, filename);

    // ���f��ID�ݒ�
    instancing_model->SetModelId(ModelIDAllocate());

    // �}�b�v�ɓo�^
    instancing_models[filename] = std::weak_ptr<InstancingModelResource>(instancing_model);
    return instancing_model;
}
