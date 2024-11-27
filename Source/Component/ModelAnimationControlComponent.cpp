#include "Debug/ImGuiHelper.h"
#include "ModelAnimationControlComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/ModelComponent.h"

ModelAnimationControlComponent::ModelAnimationControlComponent(const char* filename)
	:param()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();
}

void ModelAnimationControlComponent::Start()
{
	if (const auto& onwer = GetOwner())
	{
		if (const auto& model = onwer->EnsureComponentValid(this->model_Wptr))
		{
			this->animation_size = static_cast<int>(model->GetResource()->GetAnimations().size());
#ifdef _DEBUG
			for (size_t i = 0; i < this->animation_size; ++i)
			{
				this->animation_name_pool.emplace_back(model->GetResource()->GetAnimations()[i].name);
			}
#endif // DEBUG
		}
	}
}

void ModelAnimationControlComponent::ReStart()
{
}

void ModelAnimationControlComponent::Update(float elapsed_time)
{
	UpdateAnimation(elapsed_time);
}

void ModelAnimationControlComponent::UpdateAnimation(float elapsed_time)
{
	// 再生中でないなら更新しない
	if (!IsPlayAnimation()) return;

	auto owner = GetOwner();
	if (!owner) return;
	auto model = owner->EnsureComponentValid<ModelComponent>(this->model_Wptr);
	if (!model) return;
	auto model_resource = model->GetResource();
	if (!model_resource) return;

	// アニメーションブレンド率
	float blend_rate = 1.0f;

	if (this->param.animation_blend_time < this->param.animation_blend_seconds)
	{
		this->param.animation_blend_time += elapsed_time;
		if (this->param.animation_blend_time >= this->param.animation_blend_seconds)
		{
			this->param.animation_blend_time = this->param.animation_blend_seconds;
		}
		//　ブレンド率計算
		blend_rate = this->param.animation_blend_time / this->param.animation_blend_seconds;
		blend_rate *= blend_rate;
	}

	const std::vector<ModelResource::Animation>& animations = model_resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(this->param.current_animation_index);

	std::vector<ModelComponent::Node>& node_vec = model->GetNodes();

	// キーフレーム取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// キーフレームの間にいるのか判定
		const ModelResource::Keyframe& keyframe0 = keyframes.at(static_cast<size_t>(keyIndex));
		const ModelResource::Keyframe& keyframe1 = keyframes.at(static_cast<size_t>(keyIndex + 1));

		// 経過時間が再生時間内なら
		if (this->param.current_animation_seconds >= keyframe0.seconds &&
			this->param.current_animation_seconds < keyframe1.seconds)
		{
			// 補間率を計算
			float rate = (this->param.current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(node_vec.size());

			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				ModelComponent::Node& node = node_vec[nodeIndex];

				// アニメーションブレンドするなら
				if (blend_rate < 1.0f)
				{
					// 現在の姿勢と次のキーフレームとの姿勢の補間

					// スケールの線形補間
					MYVECTOR3 S0 = node.scale;
					MYVECTOR3 S1 = key1.scale;
					MYVECTOR3 S = S0.Lerp(S1, blend_rate);
					S.GetFlaot3(node.scale);

					// 角度の球面線形補間
					MYVECTOR4 R0 = node.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// 座標の線形補間
					MYVECTOR3 T0 = node.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					T.GetFlaot3(node.translate);
				}
				else
				{
					// 前のキーフレームと次のキーフレームの姿勢を補間

					// スケールの線形補間
					MYVECTOR3 S0 = key0.scale;
					MYVECTOR3 S1 = key1.scale;
					MYVECTOR3 S = S0.Lerp(S1, blend_rate);
					S.GetFlaot3(node.scale);

					// 角度の線形補間
					MYVECTOR4 R0 = key0.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// 座標の線形補間
					MYVECTOR3 T0 = key0.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					T.GetFlaot3(node.translate);
				}
			}
			break;
		}
	}

	// 再生終了したら
	if (this->param.animation_end_flag)
	{
		this->param.animation_end_flag = false;
		return;
	}

	this->param.current_animation_seconds += elapsed_time;

	// 再生時間を超えたら
	if (this->param.current_animation_seconds >= animation.seconds_length)
	{
		// ループ再生する場合
		if (this->param.animation_loop_flag)
		{
			// 再生時間を戻す
			this->param.current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ループ再生しない場合
		else
		{
			this->param.animation_end_flag = true;
		}
	}
}

void ModelAnimationControlComponent::PlayAnimation(int index, bool loop, float blend_seconds)
{
	this->param.current_animation_index = index;
	this->param.current_animation_seconds = 0.0f;

	this->param.animation_loop_flag = loop;
	this->param.animation_end_flag = false;

	this->param.animation_blend_time = 0.0f;
	this->param.animation_blend_seconds = blend_seconds;
}

bool ModelAnimationControlComponent::IsPlayAnimation() const
{
	if (this->param.current_animation_index < 0)return false;
	if (this->param.current_animation_index >= this->animation_size) return false;
	if (this->param.animation_end_flag) return false;
	return true;
}

#ifdef _DEBUG

void ModelAnimationControlComponent::DrawDebugGUI()
{
	if (const auto& onwer = GetOwner())
	{
		if (const auto& model = onwer->EnsureComponentValid(this->model_Wptr))
		{
			auto model_resource = model->GetResource();
			if (!model_resource) return;

			int& anime_index = this->param.current_animation_index;
			if (anime_index < 0) return;

			if (model_resource->GetAnimations().size())
			{
				const auto& animation = model_resource->GetAnimations()[anime_index];

				std::string play_anime_name = this->animation_name_pool[anime_index];
				ImGui::SliderFloat("Current Animation Seconds", &this->param.current_animation_seconds, 0.0f, animation.seconds_length);
				if (ImGui::ComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index))
				{
					PlayAnimation(anime_index, 0.0f, 0.0f);
				}
				ImGui::Checkbox("Animation Loop Flag", &this->param.animation_loop_flag);

				ImGui::InputFloat("Animation Blend Seconds", &this->param.animation_blend_seconds);
				ImGui::SliderFloat("Animation Blend Time", &this->param.animation_blend_time, 0.0f, this->param.animation_blend_seconds);
				ImGui::Checkbox("Animation End Flag", &this->param.animation_end_flag);
			}
		}
	}
}

#endif // DEBUG