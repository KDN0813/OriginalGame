#include "InstancingModel.h"

InstancingModel::InstancingModel(const char* filename)
    :instance_cout()
{
    // TODO (07/01)�@���f���쐬
}

int InstancingModel::AllocateInstancingIndex()
{
    // TODO (07/01)�A�g���Ă��Ȃ��ԍ������蓖�ĂĕԂ�
    return 0;
}

void InstancingModel::FreeInstancingIndex(int instancingIndex)
{
    // TODO (07/01)�B���蓖�Ă�ꂽ�ԍ����������
}

void InstancingModel::UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform)
{
    // TODO (07/01)�C�s��v�Z
}

BufferData InstancingModel::GetBufferData(const ModelResource::Mesh& mesh) const
{
    return BufferData();
}
