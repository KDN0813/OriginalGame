#include "ModelResourceManager.h"

std::shared_ptr<ModelResource> ModelResourceManager::LoadModelResource(ID3D11Device* device,const char* filename)
{
    // ���f������
    ModelMap::iterator it = models.find(filename);
    // ���������ꍇ
    if (it != models.end())
    {
        // �����N���؂�Ă��Ȃ��ꍇ
        if (it->second.expired() == false)
        {
            // std::shared_ptr������ĕԂ�
            return it->second.lock();
        }
    }

    // �V�K���f�����\�[�X�쐬���ǂݍ���
    std::shared_ptr<ModelResource>	model;
    model = std::make_shared<ModelResource>();
    model->Load(device, filename);
 
    // �}�b�v�ɓo�^
    // �L�[���o�^����Ă��Ȃ���Α}�����A����Α������
    // �f�[�^�̏㏑���E�V�K�ǉ��������ōs���Ă���
    models[filename] = std::weak_ptr<ModelResource>(model);
    return model;
}