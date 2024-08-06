#include "InstancingModel.h"
#include "System/Misc.h"
#include "Graphics/Graphics.h"
#include "Model/ModelResourceManager.h"

InstancingModel::InstancingModel(ID3D11Device* device, const char* filename)
	:bone_transform_count(0), mesh_offsets()
{
    // TODO (07/01)①モデル作成
        // リソース読み込み
    resource = ModelResourceManager::Instance()->LoadModelResource(filename);

    transform_datas.resize(InstancingMax);

    // BTT作成
	{
		ID3D11Device* device = Graphics::Instance().GetDevice();
		HRESULT hr{ S_OK };

		// ノード情報取得
		{
			// ノード
			const std::vector<ModelResource::Node>& resource_nodes = resource->GetNodes();

			nodes.resize(resource_nodes.size());
			for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
			{
				auto&& src = resource_nodes.at(nodeIndex);
				auto&& dst = nodes.at(nodeIndex);

				dst.name = src.name.c_str();
				dst.parent = src.parent_index >= 0 ? &nodes.at(src.parent_index) : nullptr;
				dst.scale = src.scale;
				dst.rotate = src.rotate;
				dst.translate = src.translate;

				if (dst.parent != nullptr)
				{
					dst.parent->children.emplace_back(&dst);
				}
			}
		}

#ifdef _DEBUG
		// TODO(08/06)デバッグ用変数
		bool top_anime = true;	// 各アニメーションの先頭のであるか
		bool top_mesh = true;	// 各メッシュの先頭であるか
		bool frame_top = true;	// 各フレームの先頭であるか
		DirectX::XMFLOAT3 color{};
#endif // _DEBUG


		// ボーントランスフォーム計算
		BoneTransformTextureData BTTdata{};
		{
			// アニメーションの数ループ
			for (size_t anime_index = 0; anime_index < resource->GetAnimations().size(); ++anime_index)
			{
#ifdef _DEBUG
				// TODO(08/06)BTT確認用デバッグ処理
				top_anime = true;
#endif // _DEBUG


				PlayAnimation(anime_index);
				// アニメーションが終了するまでループ
				while (IsPlayAnimation())
				{
#ifdef _DEBUG
					// TODO(08/06)フレーム先頭色変え
					frame_top = true;

#endif // _DEBUG


					// アニメーション更新
					UpdateAnimation(0.01f);
					UpdateTransform();

					// ボーントランスフォーム計算
					// 全てのメッシュで回す
					for (const ModelResource::Mesh& mesh : resource->GetMeshes())
					{
#ifdef _DEBUG
						// TODO(08/06)メッシュの先頭色変え
						top_mesh = true;

#endif // _DEBUG

						if (mesh.node_indices.size() > 0)
						{
							for (size_t i = 0; i < mesh.node_indices.size(); ++i)
							{
								auto& add_data = BTTdata.bone_transforms.emplace_back();
								DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.node_indices.at(i)).worldTransform);
								DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offset_transforms.at(i));
								DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
								DirectX::XMStoreFloat4x4(&add_data.transform, boneTransform);

#ifdef _DEBUG
								// TODO(デバッグ用処理)

								if (top_anime)
								{
									color.x = 0.8f;
									top_anime = false;
								}
								if (frame_top)
								{
									color.y = 0.8f;
									frame_top = false;
								}
								if (top_mesh)
								{
									color.z = 0.8f;
									top_mesh = false;
								}
								DirectX::XMFLOAT4X4& transform = add_data.transform;
								transform._11 = { color.x };
								transform._12 = { color.y };
								transform._13 = { color.z };
								transform._14 = { 1.0f };
								
								color = {};
#endif // _DEBUG
							}
						}
						else
						{
							auto& add_data = BTTdata.bone_transforms.emplace_back();
							add_data.transform = nodes.at(mesh.node_index).worldTransform;
						}
					}
				}

			}
		}

		// bone_transform_bufferの作成
		{
			D3D11_BUFFER_DESC buffer_desc{};
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV としてバインドする
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU書き込みアクセスを許可
			buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 構造体バッファに設定
			buffer_desc.ByteWidth = (sizeof(BoneTransform) * BTTdata.bone_transforms.size());	// バッファサイズ設定
			buffer_desc.StructureByteStride = sizeof(BoneTransform);	// 構造体の各要素のサイズ設定
			D3D11_SUBRESOURCE_DATA subresource_data{};
			subresource_data.pSysMem = BTTdata.bone_transforms.data();	// 初期データ設定

			hr = device->CreateBuffer(&buffer_desc, &subresource_data, bone_transform_buffer.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// シェーダーリソースビューの作成
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;	// 要素の先頭インデックス
			srvDesc.Buffer.NumElements = static_cast<UINT>(BTTdata.bone_transforms.size());	// 要素の数
			BTTdata.bone_transforms.clear();

			hr = device->CreateShaderResourceView(bone_transform_buffer.Get(), &srvDesc, this->bone_transform_texture.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
	}

	// instance_data_buffer作成
	{
		const size_t MeshCount = resource->GetMeshes().size();
		InstanceData* instance_data = new InstanceData[MAX_INSTANCES * MeshCount];

		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV としてバインドする
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 構造体バッファに設定
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.ByteWidth = (sizeof(InstanceData) * MAX_INSTANCES * resource->GetMeshes().size());	// バッファサイズ設定
		buffer_desc.StructureByteStride = sizeof(InstanceData);	// 構造体の各要素のサイズ設定
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = instance_data;	// 初期データ設定

		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, this->instance_data_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		delete[] instance_data;
	}

	// instance_data_structured_bufferの作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;	// 要素の先頭インデックス
		srvDesc.Buffer.NumElements = static_cast<UINT>(MAX_INSTANCES);	// 要素の数

		HRESULT hr = device->CreateShaderResourceView(this->instance_data_buffer.Get(), &srvDesc, this->instance_data_structured_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ボーントランスフォームの数計算
	{
		const size_t mesh_count = resource->GetMeshes().size();
		mesh_offsets.resize(mesh_count);
		for (size_t mesh_index = 0; mesh_index < mesh_count; ++mesh_index)
		{
			const ModelResource::Mesh& mesh = resource->GetMeshes()[mesh_index];
			this->bone_transform_count += mesh.node_indices.size();
			this->mesh_offsets[mesh_index] = this->bone_transform_count;
		}
	}
}

int InstancingModel::AllocateInstancingIndex()
{
    // TODO (07/01)②使われていない番号を割り当てて返す
    for (int i = 0; i < InstancingMax; ++i)
    {
        if (!transform_datas[i].exist)
        {
            transform_datas[i].exist = true;
            return i;
        }
    }
    return -1;
}

void InstancingModel::FreeInstancingIndex(int instancingIndex)
{
    // TODO (07/01)③割り当てられた番号を解放する
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
        transform_datas[instancingIndex].exist = false;
}

void InstancingModel::UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform)
{
    // TODO (07/01)④行列計算
    if (0 <= instancingIndex && instancingIndex < InstancingMax)
        this->transform_datas[instancingIndex].transform = transform;
}

void InstancingModel::UpdateInstanceData(ID3D11DeviceContext* dc, int& instancing_count)
{
	// TODO (08/04)続き書く
	instancing_count = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT hr = dc->Map(instance_data_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	this->instance_data = reinterpret_cast<InstanceData*>(mappedResource.pData);

	// world_transformsを更新
	{
		for (int i = 0; i < InstancingMax; ++i)
		{
			if (!transform_datas[i].exist)
				continue;
			this->instance_data[instancing_count].frame = 0;
			this->instance_data[instancing_count].animation_start_offset = 1;
			this->instance_data[instancing_count].world_transform = transform_datas[i].transform;
			++instancing_count;
		}

		dc->Unmap(instance_data_buffer.Get(), 0);
	}
}

void InstancingModel::PlayAnimation(int index)
{
    current_animation_index = index;
    current_animation_seconds = 0.0f;	// 経過時間
}

// TODO(07/24)実行失敗したらfalseを返すようにする
// TODO(07/24)命名規則統一
void InstancingModel::UpdateAnimation(float elapsed_time)
{
	// 再生中でないなら処理しない
	if (!IsPlayAnimation()) return;

	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);	

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるのか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		// 経過時間が再生時間内なら
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間かた補間率を算出する[17](自己解)
			float rate = (current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());

			// 再生時間とキーフレームの時間から補間率を計算する
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2つのキーフレーム間の補間計算[17]
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				// 前のキーフレームと次のキーフレームの姿勢を補間

				// スケールの線形補間
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);

				// 角度の線形補間
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);

				// 座標の線形補間
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

				// 補間結果を設定
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);

			}
			break;
		}
	}

	// 時間経過
	current_animation_seconds += elapsed_time;

	// 再生時間が終端時間を超えたら
	if (current_animation_seconds >= animation.seconds_length)
	{
		current_animation_index = -1;
	}
}

bool InstancingModel::IsPlayAnimation() const
{
	if (current_animation_index < 0)return false;
	if (current_animation_index >= resource->GetAnimations().size()) return false;
	return true;
}

void InstancingModel::UpdateTransform()
{
	DirectX::XMMATRIX Transform = DirectX::XMMatrixIdentity();

	for (Node& node : nodes)
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
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}