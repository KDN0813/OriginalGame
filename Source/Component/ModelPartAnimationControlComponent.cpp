#include "Debug/ImGuiHelper.h"
#include "ModelPartAnimationControlComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/ModelComponent.h"

ModelPartAnimationControlComponent::ModelPartAnimationControlComponent(InitAnimeParam init_param)
	:init_param(init_param), main_parts()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	ID3D11Device* device = graphics->GetDevice();
}

void ModelPartAnimationControlComponent::Start()
{
	if (const auto& owner = GetOwner())
	{
		if (const auto& model = owner->GetComponent(this->model_Wptr))
		{
			this->animation_size = static_cast<int>(model->GetResource()->GetAnimations().size());
#ifdef _DEBUG
			for (size_t i = 0; i < this->animation_size; ++i)
			{
				this->animation_name_pool.emplace_back(model->GetResource()->GetAnimations()[i].name);
			}
#endif // DEBUG

			// ���C���p�[�c���ݒ�
			for (ModelResource::NodeId node_id = 0; node_id < static_cast<ModelResource::NodeId>(model->GetNodes().size()); ++node_id)
			{
				this->main_parts.node_index_vec.emplace_back(node_id);
			}
		}
	}

	if (0 <= this->init_param.init_anime_index)
	{
		PlayAnimation(this->init_param.init_anime_index, this->init_param.init_anime_loop, 0.0f);
		UpdateAnimation(this->main_parts, 0.0f);
	}
}

void ModelPartAnimationControlComponent::ReStart()
{
	if (0 <= this->init_param.init_anime_index)
	{
		PlayAnimation(this->init_param.init_anime_index, this->init_param.init_anime_loop, 0.0f);
	}
}

void ModelPartAnimationControlComponent::Update(float elapsed_time)
{
	UpdateAnimation(this->main_parts,elapsed_time);
}

void ModelPartAnimationControlComponent::UpdateAnimation(PartsParam& parts, float elapsed_time)
{
	// �Đ����łȂ��Ȃ�X�V���Ȃ�
	if (!IsPlayAnimation()) return;

	auto owner = GetOwner();
	if (!owner) return;
	auto model = owner->GetComponent<ModelComponent>(this->model_Wptr);
	if (!model) return;
	auto model_resource = model->GetResource();
	if (!model_resource) return;

	// �A�j���[�V�����u�����h��
	float blend_rate = 1.0f;

	if (parts.anime_param.animation_blend_time < parts.anime_param.animation_blend_seconds)
	{
		parts.anime_param.animation_blend_time += elapsed_time;
		if (parts.anime_param.animation_blend_time >= parts.anime_param.animation_blend_seconds)
		{
			parts.anime_param.animation_blend_time = parts.anime_param.animation_blend_seconds;
		}
		//�@�u�����h���v�Z
		blend_rate = parts.anime_param.animation_blend_time / parts.anime_param.animation_blend_seconds;
	}

	const std::vector<ModelResource::Animation>& animations = model_resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(parts.anime_param.current_animation_index);

	std::vector<ModelComponent::Node>& node_vec = model->GetNodes();

	// �L�[�t���[���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int key_count = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < key_count - 1; ++keyIndex)
	{
		// �L�[�t���[���̊Ԃɂ���̂�����
		const ModelResource::Keyframe& keyframe0 = keyframes.at(static_cast<size_t>(keyIndex));
		const ModelResource::Keyframe& keyframe1 = keyframes.at(static_cast<size_t>(keyIndex + 1));

		// �o�ߎ��Ԃ��Đ����ԓ��Ȃ�
		if (parts.anime_param.current_animation_seconds >= keyframe0.seconds &&
			parts.anime_param.current_animation_seconds < keyframe1.seconds)
		{
			// ��ԗ����v�Z
			float rate = (parts.anime_param.current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int node_count = static_cast<int>(node_vec.size());

			for (int node_index = 0; node_index < node_count; ++node_index)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(node_index);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(node_index);

				ModelComponent::Node& node = node_vec[node_index];

				// �A�j���[�V�����u�����h����Ȃ�
				if (blend_rate < 1.0f)
				{
					// ���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕��

					// �X�P�[���̐��`���
					MYVECTOR3 S0 = node.scale;
					MYVECTOR3 S1 = key1.scale;
					MYVECTOR3 S = S0.Lerp(S1, blend_rate);
					S.GetFlaot3(node.scale);

					// �p�x�̋��ʐ��`���
					MYVECTOR4 R0 = node.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// ���W�̐��`���
					MYVECTOR3 T0 = node.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					T.GetFlaot3(node.translate);
				}
				else
				{
					// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p������

					// �X�P�[���̐��`���
					MYVECTOR3 S0 = key0.scale;
					MYVECTOR3 S1 = key1.scale;
					MYVECTOR3 S = S0.Lerp(S1, blend_rate);
					S.GetFlaot3(node.scale);

					// �p�x�̐��`���
					MYVECTOR4 R0 = key0.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// ���W�̐��`���
					MYVECTOR3 T0 = key0.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					T.GetFlaot3(node.translate);
				}
			}
			break;
		}
	}

	// �Đ��I��������
	if (parts.anime_param.animation_end_flag)
	{
		parts.anime_param.animation_end_flag = false;
		return;
	}

	parts.anime_param.current_animation_seconds += elapsed_time;

	// �Đ����Ԃ𒴂�����
	if (parts.anime_param.current_animation_seconds >= animation.seconds_length)
	{
		// ���[�v�Đ�����ꍇ
		if (parts.anime_param.animation_loop_flag)
		{
			// �Đ����Ԃ�߂�
			parts.anime_param.current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ���[�v�Đ����Ȃ��ꍇ
		else
		{
			parts.anime_param.animation_end_flag = true;
		}
	}
}

void ModelPartAnimationControlComponent::PlayAnimation(int index, bool loop, float blend_seconds)
{
	// TODO �n�C�v���p
	if (index == this->main_parts.anime_param.current_animation_index) return;

	this->main_parts.anime_param.current_animation_index = index;
	this->main_parts.anime_param.current_animation_seconds = 0.0f;

	this->main_parts.anime_param.animation_loop_flag = loop;
	this->main_parts.anime_param.animation_end_flag = false;

	this->main_parts.anime_param.animation_blend_time = 0.0f;
	this->main_parts.anime_param.animation_blend_seconds = blend_seconds;
}

bool ModelPartAnimationControlComponent::IsPlayAnimation() const
{
	if (this->main_parts.anime_param.current_animation_index < 0)return false;
	if (this->main_parts.anime_param.current_animation_index >= this->animation_size) return false;
	if (this->main_parts.anime_param.animation_end_flag) return false;
	return true;
}

#ifdef _DEBUG

void ModelPartAnimationControlComponent::DrawDebugGUI()
{
	if (const auto& owner = GetOwner())
	{
		if (const auto& model = owner->GetComponent(this->model_Wptr))
		{
			auto model_resource = model->GetResource();
			if (!model_resource) return;

			int& anime_index = this->main_parts.anime_param.current_animation_index;
			if (anime_index < 0) return;

			if (model_resource->GetAnimations().size())
			{
				const auto& animation = model_resource->GetAnimations()[anime_index];

				std::string play_anime_name = this->animation_name_pool[anime_index];
				ImGui::SliderFloat("Current Animation Seconds", &this->main_parts.anime_param.current_animation_seconds, 0.0f, animation.seconds_length);
				if (ImGui::ComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index))
				{
					PlayAnimation(anime_index, 0.0f, 0.0f);
				}
				ImGui::Checkbox("Animation Loop Flag", &this->main_parts.anime_param.animation_loop_flag);

				ImGui::InputFloat("Animation Blend Seconds", &this->main_parts.anime_param.animation_blend_seconds);
				ImGui::SliderFloat("Animation Blend Time", &this->main_parts.anime_param.animation_blend_time, 0.0f, this->main_parts.anime_param.animation_blend_seconds);
				ImGui::Checkbox("Animation End Flag", &this->main_parts.anime_param.animation_end_flag);
			}
		}
	}
}

#endif // DEBUG