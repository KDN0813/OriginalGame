#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

// インスタンシングモデル
// ①nodeがないので必用。
// 　nodeが使えるようにInstancingModelを改造する
// ②Shader側で定数バッファを使って行列を送っているが、
//	定数バッファのサイズが足りなくなる。
//  StructuredBufferを使って行列を送るようにする。
class InstancingModel
{
public:
	struct Node
	{
		Node() = default;
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};
	struct InstancingData
	{
		Node nodes[128];
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

