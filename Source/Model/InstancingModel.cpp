#include "InstancingModel.h"

InstancingModel::InstancingModel(const char* filename)
    :instance_cout()
{
    // TODO (07/01)‡@ƒ‚ƒfƒ‹ì¬
}

int InstancingModel::AllocateInstancingIndex()
{
    // TODO (07/01)‡Ag‚í‚ê‚Ä‚¢‚È‚¢”Ô†‚ğŠ„‚è“–‚Ä‚Ä•Ô‚·
    return 0;
}

void InstancingModel::FreeInstancingIndex(int instancingIndex)
{
    // TODO (07/01)‡BŠ„‚è“–‚Ä‚ç‚ê‚½”Ô†‚ğ‰ğ•ú‚·‚é
}

void InstancingModel::UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform)
{
    // TODO (07/01)‡Cs—ñŒvZ
}

BufferData InstancingModel::GetBufferData(const ModelResource::Mesh& mesh) const
{
    return BufferData();
}
