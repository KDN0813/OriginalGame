#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

// インスタンシングモデル
class InstancingModel
{
public:
	struct InstancingData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// 使用中か
		bool dummy[3];
	};
public:
	InstancingModel(const char* filename);
	~InstancingModel() {};

	// 使われていない番号を割り当てて返す
	int AllocateInstancingIndex();

	// 割り当てられた番号を解放する
	void FreeInstancingIndex(int instancingIndex);

	// 行列計算
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// 各種取得・設定関数
	const ModelResource* GetResource() const { return resource.get(); }
	const size_t GetInstanceCount()const { return this->instance_cout; }
	// シェーダーへの設定用バッファ情報取得
	BufferData GetBufferData(const ModelResource::Mesh& mesh) const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<InstancingData> instancing_data;
	size_t instance_cout;
};

