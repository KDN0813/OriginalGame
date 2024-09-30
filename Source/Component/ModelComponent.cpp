#include "ModelComponent.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

ModelComponent::ModelComponent(ID3D11Device* device, const char* filename)
{
	// ���\�[�X�ǂݍ���
	this->resource = ModelResourceManager::Instance()->LoadModelResource(device, filename);
}

void ModelComponent::Initialize()
{
	// �A�j���[�V�����̐��ɃT�C�Y�����킹��
	this->animation_info_vec.resize(this->resource->GetAnimations().size());

	// �m�[�h
	const std::vector<ModelResource::Node>& res_node_vec = this->resource->GetNodes();

	node_vec.resize(res_node_vec.size());
	for (size_t node_index = 0; node_index < node_vec.size(); ++node_index)
	{
		auto&& src = res_node_vec.at(node_index);
		auto&& dst = node_vec.at(node_index);

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

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	UpdateTransform(transform);
}

void ModelComponent::Update(float elapsed_time)
{
	AnimationStateUpdate();

	UpdateAnimation(elapsed_time);

	auto owner = GetOwner();
	DirectX::XMFLOAT4X4 world_transform;
	if (auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
	{
		world_transform = transform->GetTransform();
	}
	else
	{
		DirectX::XMStoreFloat4x4(&world_transform, DirectX::XMMatrixIdentity());
	}

	UpdateTransform(world_transform);
}

void ModelComponent::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : node_vec)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.local_transform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.world_transform, WorldTransform);
	}
}

void ModelComponent::UpdateAnimation(float elapsed_time)
{
	// �Đ����łȂ��Ȃ�X�V���Ȃ�
	if (!IsPlayAnimation()) return;

	// �A�j���[�V�����u�����h��
	float blend_rate = 1.0f;

	if (animation_blend_time < animation_blend_seconds)
	{
		animation_blend_time += elapsed_time;
		if (animation_blend_time >= animation_blend_seconds)
		{
			animation_blend_time = animation_blend_seconds;
		}
		//�@�u�����h���v�Z
		blend_rate = animation_blend_time / animation_blend_seconds;
		blend_rate *= blend_rate;
	}

	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

	// �L�[�t���[���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// �L�[�t���[���̊Ԃɂ���̂�����
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		// �o�ߎ��Ԃ��Đ����ԓ��Ȃ�
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// ��ԗ����v�Z
			float rate = (current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(node_vec.size());

			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = node_vec[nodeIndex];

				// �A�j���[�V�����u�����h����Ȃ�
				if (blend_rate < 1.0f)
				{
					// ���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕��

					// �X�P�[���̐��`���
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);

					// �p�x�̋��ʐ��`���
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// ���W�̐��`���
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					// ��Ԍ��ʂ�ݒ�
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				else
				{
					// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p������

					// �X�P�[���̐��`���
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);

					// �p�x�̐��`���
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// ���W�̐��`���
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					// ��Ԍ��ʂ�ݒ�
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	// �Đ��I��������
	if (animation_end_flag)
	{
		animation_end_flag = false;
		current_animation_index = -1;
		return;
	}

	current_animation_seconds += elapsed_time;

	// �Đ����Ԃ𒴂�����
	if (current_animation_seconds >= animation.seconds_length)
	{
		// ���[�v�Đ�����ꍇ
		if (animation_loo_flag)
		{
			// �Đ����Ԃ�߂�
			current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ���[�v�Đ����Ȃ��ꍇ
		else
		{
			animation_end_flag = true;
		}
	}
}

void ModelComponent::PlayAnimation(int index, bool loop, float blend_seconds)
{
	current_animation_index = index;
	current_animation_seconds = 0.0f;

	animation_loo_flag = loop;
	animation_end_flag = false;

	animation_blend_time = 0.0f;
	animation_blend_seconds = blend_seconds;
}

void ModelComponent::PlayAnimation(AnimationInfo* animation_info , float blend_seconds)
{
	current_animation_index = animation_info->anime_index;
	current_animation_seconds = 0.0f;

	animation_loo_flag = animation_info->loop;
	animation_end_flag = false;

	animation_blend_time = 0.0f;
	animation_blend_seconds = blend_seconds;
}

bool ModelComponent::IsPlayAnimation() const
{
	if (current_animation_index < 0)return false;
	if (current_animation_index >= resource->GetAnimations().size()) return false;
	return true;
}

ModelComponent::Node* ModelComponent::FindNode(const char* name)
{
	for (Node& node : node_vec)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}
	return nullptr;
}

void ModelComponent::AnimationStateUpdate()
{
	for (auto next_anime_info : this->animation_info_vec[this->current_animation_index]->next_animation_vec)
	{
		if (next_anime_info->judgement->Judgement())
		{
			PlayAnimation(this->animation_info_vec[next_anime_info->next_anime_index], next_anime_info ->blend_time);
			break;
		}
	}
}

#ifdef _DEBUG

void ModelComponent::DrawDebugGUI()
{
}

#endif // _DEBUG