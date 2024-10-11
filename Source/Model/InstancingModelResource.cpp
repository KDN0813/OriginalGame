#include "System/MyMath/MYMATRIX.h"
#include "InstancingModelResource.h"
#include "System/Misc.h"
#include "Model/ModelResourceManager.h"

InstancingModelResource::InstancingModelResource(ID3D11Device* device, const char* filename)
{
	auto resource = ModelResourceManager::Instance()->LoadModelResource(device, filename);

	CreateBoneTransformTexture(device, resource.get());
}

#pragma region    �{�[���g�����X�t�H�[���e�N�X�`���쐬�p

void InstancingModelResource::CreateBoneTransformTexture(ID3D11Device* device, ModelResource* resource)
{
	HRESULT hr{ S_OK };

	// �m�[�h���擾
	{
		const std::vector<ModelResource::Node>& resource_nodes = resource->GetNodes();

		node_vec.resize(resource_nodes.size());
		for (size_t nodeIndex = 0; nodeIndex < node_vec.size(); ++nodeIndex)
		{
			auto&& src = resource_nodes.at(nodeIndex);
			auto&& dst = node_vec.at(nodeIndex);

			dst.name = src.name.c_str();
			dst.parent = src.parent_index >= 0 ? &node_vec.at(src.parent_index) : nullptr;
			dst.scale = src.scale;
			dst.rotate = src.rotate;
			dst.translate = src.translate;

			if (dst.parent != nullptr)
			{
				dst.parent->children.emplace_back(&dst);
			}
		}
	}

	// bone_transform_count�̌v�Z
	{
		const size_t mesh_count = resource->GetMeshes().size();
		this->mesh_offsets.resize(mesh_count);
		for (size_t mesh_index = 0; mesh_index < mesh_count; ++mesh_index)
		{
			const ModelResource::Mesh& mesh = resource->GetMeshes()[mesh_index];
			this->mesh_offsets[mesh_index] = this->bone_transform_count;
			const UINT node_indices = static_cast<UINT>(mesh.node_indices.size());
			this->bone_transform_count += static_cast<UINT>((node_indices <= 0) ? 1 : node_indices);
		}
	}

	this->animation_lengths.resize(resource->GetAnimations().size());
	this->animation_offsets.resize(resource->GetAnimations().size());
	UINT animation_length = {};
	UINT animation_length_sum = {};
	// �{�[���g�����X�t�H�[���v�Z
	BoneTransformTextureData BTTdata{};
	{
		// �A�j���[�V�����̐����[�v
		for (int anime_index = 0; anime_index < static_cast<int>(resource->GetAnimations().size()); ++anime_index)
		{
			animation_length = 0;
			PlayAnimation(anime_index);
			// �A�j���[�V�������I������܂Ń��[�v
			while (IsPlayAnimation(resource))
			{
				// �A�j���[�V�����X�V
				float delta_time = 1.0f / 60.0f;

				UpdateAnimation(delta_time, resource);
				UpdateTransform();

				// �{�[���g�����X�t�H�[���v�Z
				// �S�Ẵ��b�V���ŉ�
				for (const ModelResource::Mesh& mesh : resource->GetMeshes())
				{
					if (mesh.node_indices.size() > 0)
					{
						for (size_t i = 0; i < mesh.node_indices.size(); ++i)
						{
							auto& add_data = BTTdata.emplace_back();
							MYMATRIX World_transform = node_vec.at(mesh.node_indices.at(i)).world_transform;
							MYMATRIX Offset_Transform = mesh.offset_transforms.at(i);
							MYMATRIX bone_transform = Offset_Transform * World_transform;
							bone_transform.GetFlaot4x4(add_data);
						}
					}
					else
					{
						auto& add_data = BTTdata.emplace_back();
						add_data = node_vec.at(mesh.node_index).world_transform;
					}
				}

				++animation_length;
			}
			this->animation_lengths[anime_index] = animation_length - 1;
			this->animation_offsets[anime_index] = animation_length_sum * bone_transform_count;
			animation_length_sum += animation_length;
		}
	}

	// bone_transform_buffer�̍쐬
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV �Ƃ��ăo�C���h����
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�������݃A�N�Z�X������
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// �\���̃o�b�t�@�ɐݒ�
		buffer_desc.ByteWidth = (sizeof(BoneTransform) * static_cast<UINT>(BTTdata.size()));	// �o�b�t�@�T�C�Y�ݒ�
		buffer_desc.StructureByteStride = sizeof(BoneTransform);	// �\���̂̊e�v�f�̃T�C�Y�ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = BTTdata.data();	// �����f�[�^�ݒ�

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, bone_transform_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �V�F�[�_�[���\�[�X�r���[�̍쐬
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;	// �v�f�̐擪�C���f�b�N�X
		srvDesc.Buffer.NumElements = static_cast<UINT>(BTTdata.size());	// �v�f�̐�
		BTTdata.clear();

		hr = device->CreateShaderResourceView(bone_transform_buffer.Get(), &srvDesc, this->bone_transform_texture.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void InstancingModelResource::PlayAnimation(int index)
{
	current_animation_index = index;
	current_animation_seconds = 0.0f;	// �o�ߎ���
}

void InstancingModelResource::UpdateAnimation(float elapsed_time, ModelResource* resource)
{
	// �Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation(resource)) return;

	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ���̂����肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(static_cast<size_t>(keyIndex));
		const ModelResource::Keyframe& keyframe1 = keyframes.at(static_cast<size_t>((keyIndex + 1)));

		// �o�ߎ��Ԃ��Đ����ԓ��Ȃ�
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ�����ԗ����Z�o����[17](���ȉ�)
			float rate = (current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(node_vec.size());

			// �Đ����ԂƃL�[�t���[���̎��Ԃ����ԗ����v�Z����
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2�̃L�[�t���[���Ԃ̕�Ԍv�Z[17]
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = node_vec[nodeIndex];

				// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p������

					// �X�P�[���̐��`���
				MYVECTOR3 S0 = key0.scale;
				MYVECTOR3 S1 = key1.scale;
				MYVECTOR3 S = S0.Lerp(S1, rate);
				S.GetFlaot3(node.scale);

				// �p�x�̐��`���
				MYVECTOR4 R0 = key0.rotate;
				MYVECTOR4 R1 = key1.rotate;
				MYVECTOR4 R = R0.SLerp(R1, rate);
				R.GetFlaot4(node.rotate);

				// ���W�̐��`���
				MYVECTOR3 T0 = key0.translate;
				MYVECTOR3 T1 = key1.translate;
				MYVECTOR3 T = T0.Lerp(T1, rate);
				T.GetFlaot3(node.translate);

			}
			break;
		}
	}

	// ���Ԍo��
	current_animation_seconds += elapsed_time;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	if (current_animation_seconds >= animation.seconds_length)
	{
		current_animation_index = -1;
	}
}

bool InstancingModelResource::IsPlayAnimation(ModelResource* resource) const
{
	if (current_animation_index < 0)return false;
	if (current_animation_index >= resource->GetAnimations().size()) return false;
	return true;
}

void InstancingModelResource::UpdateTransform()
{
	DirectX::XMMATRIX Transform = DirectX::XMMatrixIdentity();

	for (Node& node : node_vec)
	{
		// ���[�J���s��Z�o
		MYMATRIX Local_transform;
		Local_transform.SetLocalMatrix(node.scale, node.rotate, node.translate);

		// ���[���h�s��Z�o
		MYMATRIX Parent_transform;
		if (node.parent != nullptr)
		{
			Parent_transform = node.parent->world_transform;
		}
		else
		{
			Parent_transform = Transform;
		}
		MYMATRIX World_transform = Local_transform * Parent_transform;

		// �v�Z���ʂ��i�[
		Local_transform.GetFlaot4x4(node.local_transform);
		World_transform.GetFlaot4x4(node.world_transform);
	}
}

#pragma endregion    �{�[���g�����X�t�H�[���e�N�X�`���쐬�p