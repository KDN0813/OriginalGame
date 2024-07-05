#include "InstancingModel.h"
#include "Model/ModelResourceManager.h"

InstancingModel::InstancingModel(const char* filename)
    :instance_cout()
{
    // TODO (07/01)①モデル作成
        // リソース読み込み
    resource = ModelResourceManager::Instance()->LoadModelResource(filename);

    instancing_data.resize(InstancingMax);
}

int InstancingModel::AllocateInstancingIndex()
{
    // TODO (07/01)②使われていない番号を割り当てて返す
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
    // TODO (07/01)③割り当てられた番号を解放する
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
        instancing_data[instancingIndex].exist = false;
}

void InstancingModel::UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform)
{
    // TODO (07/01)④行列計算
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
    {
        for (Node& node : this->instancing_data[instancingIndex].nodes)
        {
            // ローカル行列算出
            DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
            DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
            DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
            DirectX::XMMATRIX LocalTransform = S * R * T;

            // ワールド行列算出
            DirectX::XMMATRIX ParentTransform;
            if (node.parent != nullptr)
            {
                ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
            }
            else
            {
                ParentTransform = transform;
            }
            DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

            // 計算結果を格納
            DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
            DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
        }
    }
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
