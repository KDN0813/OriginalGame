#include "Graphics/Graphics.h"
#include "ModelResourceManager.h"

std::shared_ptr<ModelResource> ModelResourceManager::LoadModelResource(const char* filename)
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
    model->Load(Graphics::Instance()->GetDevice(), filename);
 
    // �}�b�v�ɓo�^
    // �L�[���o�^����Ă��Ȃ���Α}�����A����Α������
    // �f�[�^�̏㏑���E�V�K�ǉ��������ōs���Ă���
    models[filename] = std::weak_ptr<ModelResource>(model);
    return model;
}