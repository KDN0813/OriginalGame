#include "Graphics/Graphics.h"
#include "Model/Model.h"
#include "Model/ModelResource.h"

// �R���X�g���N�^
Model::Model(const char* filename)
	:current_animation_index(-1)
	, current_animation_seconds()
	, animation_blend_time()
	, animation_blend_seconds()
	, animation_loo_flag()
	, animation_end_flag()
	, dummy()
{
	// ���\�[�X�ǂݍ���
#if 1
	resource = std::make_shared<ModelResource>();
	resource->Load(Graphics::Instance().GetDevice(), filename);
#else
	resource = ResourceManager::Instance().LoadModelResource(filename);
#endif

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
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

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
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
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// �A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTIme)
{
	// �Đ����łȂ��Ȃ�X�V���Ȃ�
	if (!IsPlayAnimation()) return;

	// �A�j���[�V�����u�����h��
	float blend_rate = 1.0f;

	if (animation_blend_time < animation_blend_seconds)
	{
		animation_blend_time += elapsedTIme;
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
			
			int nodeCount = static_cast<int>(nodes.size());
			
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				// �A�j���[�V�����u�����h����Ȃ�
				if (blend_rate < 1.0f)
				{
					// ���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕��

					// �X�P�[���̐��`���
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S  = DirectX::XMVectorLerp(S0, S1, rate);
					
					// �p�x�̋��ʐ��`���
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// ���W�̐��`���
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

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
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// ���W�̐��`���
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

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
		animation_end_flag		= false;
		current_animation_index	= -1;
		return;
	}

	current_animation_seconds += elapsedTIme;

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

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop,float blendSeconds)
{
	current_animation_index	= index;
	current_animation_seconds = 0.0f;

	animation_loo_flag		= loop;
	animation_end_flag		= false;

	animation_blend_time		= 0.0f;
	animation_blend_seconds	= blendSeconds;
}

// �A�j���[�V�����Đ���������
bool Model::IsPlayAnimation() const
{
	// �A�j���[�V�����ԍ���0�ȉ��Ȃ�
	if (current_animation_index < 0)return false;
	// �A�j���[�V�����ԍ����ۑ�����Ă���A�j���[�V�����̐��ȏ�Ȃ�
	if (current_animation_index >= resource->GetAnimations().size()) return false;

	return true;
}

// �m�[�h����
Model::Node* Model::FindNode(const char* name)
{
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (Node& node : nodes)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}

	// ������Ȃ�����
	return nullptr;
}
