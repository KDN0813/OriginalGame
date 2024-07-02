#include "InstancingModel.h"
#include "Model/ModelResourceManager.h"

InstancingModel::InstancingModel(const char* filename)
    :instance_cout()
{
    // TODO (07/01)�@���f���쐬
        // ���\�[�X�ǂݍ���
    resource = ModelResourceManager::Instance()->LoadModelResource(filename);

    instancing_data.resize(InstancingMax);
}

int InstancingModel::AllocateInstancingIndex()
{
    // TODO (07/01)�A�g���Ă��Ȃ��ԍ������蓖�ĂĕԂ�
    for (int i = 0; i < InstancingMax; ++i)
    {
        if (!instancing_data[i].exist)
        {
            instancing_data[i].exist = true;
            return i;
        }
    }
    return -1;
}

void InstancingModel::FreeInstancingIndex(int instancingIndex)
{
    // TODO (07/01)�B���蓖�Ă�ꂽ�ԍ����������
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
        instancing_data[instancingIndex].exist = false;
}

void InstancingModel::UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform)
{
    // TODO (07/01)�C�s��v�Z
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
        this->instancing_data[instancingIndex].transform = transform;
}

BufferData InstancingModel::GetBufferData(const ModelResource::Mesh& mesh) const
{
    BufferData	bufferData(mesh);
    for (int i = 0; i < InstancingMax; ++i)
    {
        if (!this->instancing_data[i].exist)
            continue;

        bufferData.transform[bufferData.instancingCount++] = this->instancing_data[i].transform;
    }
    return bufferData;
}
