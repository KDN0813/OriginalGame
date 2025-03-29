#include "Debug/ImGuiHelper.h"
#include "ModelAnimationControlComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/ModelComponent.h"

ModelAnimationControlComponent::ModelAnimationControlComponent(InitAnimeParam init_param)
	:init_param(init_param), main_parts()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();
}

bool FindName(const std::vector<ModelResource::Node>& nodes,ModelResource::Node node, std::string name)
{
	if (node.name == name) return true;

	int parent_index = node.parent_index;
	while (0 <= parent_index && parent_index < nodes.size())
	{
		if (nodes[parent_index].name == name)
		{
			return true;
		}
		parent_index = nodes[parent_index].parent_index;
	}
	return false;
}

void ModelAnimationControlComponent::Start()
{
	if (const auto& owner = GetOwner())
	{
		if (const auto& model = owner->GetComponent(this->model_Wptr))
		{
			this->animation_size = static_cast<int>(model->GetResource()->GetAnimations().size());
			// メインパーツ情報設定
			for (ModelResource::NodeId node_id = 0; node_id < static_cast<ModelResource::NodeId>(model->GetNodes().size()); ++node_id)
			{
				this->main_parts.node_index_vec.emplace_back(node_id);
			}

			// サブパーツ情報設定
			for (const auto& name : this->init_param.sub_part_root_name_vec)
			{
				const auto& nodes = model->GetResource()->GetNodes();
				// 名前検索
				for (ModelResource::NodeId node_id = 0; node_id < nodes.size(); ++node_id)
				{
					if (FindName(nodes, nodes[node_id], name))
					{
						this->sub_parts.node_index_vec.emplace_back(node_id);
					}
				}
			}

			// サブに登録したインデックスを削除する
			for (const auto& index : this->sub_parts.node_index_vec)
			{
				const auto& start_it = std::find(this->main_parts.node_index_vec.begin(), this->main_parts.node_index_vec.end(), index);
				if (start_it != this->main_parts.node_index_vec.end())
				{
					this->main_parts.node_index_vec.erase(start_it);
				}
			}

#ifdef _DEBUG
			for (size_t i = 0; i < this->animation_size; ++i)
			{
				this->animation_name_pool.emplace_back(model->GetResource()->GetAnimations()[i].name);
			}
#endif // DEBUG
		}
	}

	if (0 <= this->init_param.init_anime_index)
	{
		PlayMainPartsAnimation(this->init_param.init_anime_index, this->init_param.init_anime_loop, 0.0f);
		UpdateAnimation(this->main_parts, 0.0f);
	}
}

void ModelAnimationControlComponent::ReStart()
{
	if (0 <= this->init_param.init_anime_index)
	{
		PlayMainPartsAnimation(this->init_param.init_anime_index, this->init_param.init_anime_loop, 0.0f);
	}
}

void ModelAnimationControlComponent::Update(float elapsed_time)
{
	UpdateAnimation(this->main_parts, elapsed_time);
	UpdateAnimation(this->sub_parts, elapsed_time);
}

void ModelAnimationControlComponent::UpdateAnimation(PartsParam& parts, float elapsed_time)
{
	// 再生中でないなら更新しない
	if (!IsPlayPartsAnimation(parts)) return;

	auto owner = GetOwner();
	if (!owner) return;
	auto model = owner->GetComponent<ModelComponent>(this->model_Wptr);
	if (!model) return;
	auto model_resource = model->GetResource();
	if (!model_resource) return;

	// アニメーションブレンド率
	float blend_rate = 1.0f;

	if (parts.anime_param.animation_blend_time < parts.anime_param.animation_blend_seconds)
	{
		parts.anime_param.animation_blend_time += elapsed_time;
		if (parts.anime_param.animation_blend_time >= parts.anime_param.animation_blend_seconds)
		{
			parts.anime_param.animation_blend_time = parts.anime_param.animation_blend_seconds;
		}
		//　ブレンド率計算
		blend_rate = parts.anime_param.animation_blend_time / parts.anime_param.animation_blend_seconds;
		blend_rate *= blend_rate;
	}

	const std::vector<ModelResource::Animation>& animations = model_resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(parts.anime_param.current_animation_index);

	std::vector<ModelComponent::Node>& node_vec = model->GetNodes();

	// キーフレーム取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int key_count = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < key_count - 1; ++keyIndex)
	{
		// キーフレームの間にいるのか判定
		const ModelResource::Keyframe& keyframe0 = keyframes.at(static_cast<size_t>(keyIndex));
		const ModelResource::Keyframe& keyframe1 = keyframes.at(static_cast<size_t>(keyIndex + 1));

		// 経過時間が再生時間内なら
		if (parts.anime_param.current_animation_seconds >= keyframe0.seconds &&
			parts.anime_param.current_animation_seconds < keyframe1.seconds)
		{
			// 補間率を計算
			float rate = (parts.anime_param.current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			for (int node_index : parts.node_index_vec)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(node_index);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(node_index);

				ModelComponent::Node& node = node_vec[node_index];

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
	if (parts.anime_param.animation_end_flag)
	{
		parts.anime_param.animation_end_flag = false;
		return;
	}

	parts.anime_param.current_animation_seconds += elapsed_time;

	// 再生時間を超えたら
	if (parts.anime_param.current_animation_seconds >= animation.seconds_length)
	{
		// ループ再生する場合
		if (parts.anime_param.animation_loop_flag)
		{
			// 再生時間を戻す
			parts.anime_param.current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ループ再生しない場合
		else
		{
			parts.anime_param.animation_end_flag = true;
		}
	}
}

bool ModelAnimationControlComponent::IsPlayPartsAnimation(const PartsParam& parts) const
{
	if (parts.anime_param.current_animation_index < 0)return false;
	if (parts.anime_param.current_animation_index >= this->animation_size) return false;
	if (parts.anime_param.animation_end_flag) return false;
	return true;
}

void ModelAnimationControlComponent::PlayMainPartsAnimation(int index, bool loop, float blend_seconds)
{
	this->main_parts.anime_param.current_animation_index = index;
	this->main_parts.anime_param.current_animation_seconds = 0.0f;

	this->main_parts.anime_param.animation_loop_flag = loop;
	this->main_parts.anime_param.animation_end_flag = false;

	this->main_parts.anime_param.animation_blend_time = 0.0f;
	this->main_parts.anime_param.animation_blend_seconds = blend_seconds;
}

bool ModelAnimationControlComponent::IsPlayMainPartsAnimation() const
{
	return IsPlayPartsAnimation(this->main_parts);
}

void ModelAnimationControlComponent::PlaySubPartsAnimation(int index, bool loop, float blend_seconds)
{
	this->sub_parts.anime_param.current_animation_index = index;
	this->sub_parts.anime_param.current_animation_seconds = 0.0f;

	this->sub_parts.anime_param.animation_loop_flag = loop;
	this->sub_parts.anime_param.animation_end_flag = false;

	this->sub_parts.anime_param.animation_blend_time = 0.0f;
	this->sub_parts.anime_param.animation_blend_seconds = blend_seconds;
}

bool ModelAnimationControlComponent::IsPlaySubPartsAnimation() const
{
	return IsPlayPartsAnimation(this->sub_parts);
}

#ifdef _DEBUG

void ModelAnimationControlComponent::DrawPartsDebugGUI(std::string label,PartsParam& parts)
{
	const auto& owner = GetOwner();
	if (owner == nullptr)return;

	// メインパーツ
	if (ImGui::TreeNodeEx(label.c_str()))
	{
		if (const auto& model = owner->GetComponent(this->model_Wptr))
		{
			auto model_resource = model->GetResource();
			if (!model_resource) return;

			int& anime_index = parts.anime_param.current_animation_index;
			if (anime_index < 0) return;

			if (model_resource->GetAnimations().size())
			{
				const auto& animation = model_resource->GetAnimations()[anime_index];

				std::string play_anime_name = this->animation_name_pool[anime_index];
				ImGui::SliderFloat("Current Animation Seconds", &parts.anime_param.current_animation_seconds, 0.0f, animation.seconds_length);
				if (ImGui::ComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index))
				{
					PlayMainPartsAnimation(anime_index, 0.0f, 0.0f);
				}
				ImGui::Checkbox("Animation Loop Flag", &parts.anime_param.animation_loop_flag);

				ImGui::InputFloat("Animation Blend Seconds", &parts.anime_param.animation_blend_seconds);
				ImGui::SliderFloat("Animation Blend Time", &parts.anime_param.animation_blend_time, 0.0f, parts.anime_param.animation_blend_seconds);
				ImGui::Checkbox("Animation End Flag", &parts.anime_param.animation_end_flag);
			}
		}
		ImGui::TreePop();
	}
}

void ModelAnimationControlComponent::DrawDebugGUI()
{
	DrawPartsDebugGUI("MainParts",this->main_parts);
}

#endif // DEBUG