#include <imgui.h>
#include "Debug/ImGuiHelper.h"
#include "ModelComponent.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

ModelComponent::ModelComponent(ID3D11Device* device, const char* filename)
{
#ifdef _DEBUG
	this->model_name = filename;
#endif // _DEBUG


	// リソース読み込み
	this->resource = ModelResourceManager::Instance()->LoadModelResource(device, filename);

	// アニメステートの初期設定
	{
		this->anime_state_pool.clear();
		for (AnimeIndex index = 0; index < this->resource->GetAnimations().size(); ++index)
		{
			const auto& animation = this->resource->GetAnimations()[index];
			auto& anime_state = this->anime_state_pool.emplace_back();
			anime_state.anime_index = index;
			anime_state.name = animation.name;
#ifdef _DEBUG
			this->animation_name_pool.emplace_back(animation.name);
#endif // _DEBUG
		}
	}
}

void ModelComponent::Start()
{
	// ノード
	const std::vector<ModelResource::Node>& res_node_vec = this->resource->GetNodes();

	this->node_vec.resize(res_node_vec.size());
	for (size_t node_index = 0; node_index < this->node_vec.size(); ++node_index)
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

	// 行列計算
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
	UpdateAnimationState();

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
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
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

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.local_transform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.world_transform, WorldTransform);
	}
}

void ModelComponent::UpdateAnimation(float elapsed_time)
{
	// 再生中でないなら更新しない
	if (!IsPlayAnimation()) return;

	// アニメーションブレンド率
	float blend_rate = 1.0f;

	if (this->animation_blend_time < this->animation_blend_seconds)
	{
		this->animation_blend_time += elapsed_time;
		if (this->animation_blend_time >= this->animation_blend_seconds)
		{
			this->animation_blend_time = this->animation_blend_seconds;
		}
		//　ブレンド率計算
		blend_rate = this->animation_blend_time / this->animation_blend_seconds;
		blend_rate *= blend_rate;
	}

	const std::vector<ModelResource::Animation>& animations = this->resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

	// キーフレーム取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// キーフレームの間にいるのか判定
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		// 経過時間が再生時間内なら
		if (this->current_animation_seconds >= keyframe0.seconds &&
			this->current_animation_seconds < keyframe1.seconds)
		{
			// 補間率を計算
			float rate = (this->current_animation_seconds - keyframe0.seconds)
				/ (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(this->node_vec.size());

			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = this->node_vec[nodeIndex];

				// アニメーションブレンドするなら
				if (blend_rate < 1.0f)
				{
					// 現在の姿勢と次のキーフレームとの姿勢の補間

					// スケールの線形補間
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blend_rate);

					// 角度の球面線形補間
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blend_rate);

					// 座標の線形補間
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blend_rate);

					// 補間結果を設定
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				else
				{
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
			}
			break;
		}
	}

	// 再生終了したら
	if (this->animation_end_flag)
	{
		this->animation_end_flag = false;
		return;
	}

	this->current_animation_seconds += elapsed_time;

	// 再生時間を超えたら
	if (this->current_animation_seconds >= animation.seconds_length)
	{
		// ループ再生する場合
		if (this->animation_loop_flag)
		{
			// 再生時間を戻す
			this->current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ループ再生しない場合
		else
		{
			this->animation_end_flag = true;
		}
	}
}

void ModelComponent::PlayAnimation(int index, bool loop, float blend_seconds)
{
	this->current_animation_index = index;
	this->current_animation_seconds = 0.0f;

	this->animation_loop_flag = loop;
	this->animation_end_flag = false;

	this->animation_blend_time = 0.0f;
	this->animation_blend_seconds = blend_seconds;
}

void ModelComponent::PlayAnimation(const AnimeState& animation_info, float blend_seconds)
{
	this->current_animation_index = animation_info.anime_index;
	this->current_animation_seconds = 0.0f;

	this->animation_loop_flag = animation_info.loop;
	this->animation_end_flag = false;

	this->animation_blend_time = 0.0f;
	this->animation_blend_seconds = blend_seconds;
}

bool ModelComponent::IsPlayAnimation() const
{
	if (this->current_animation_index < 0)return false;
	if (this->current_animation_index >= this->resource->GetAnimations().size()) return false;
	if (this->animation_end_flag) return false;
	return true;
}

ModelComponent::Node* ModelComponent::FindNode(const char* name)
{
	for (Node& node : this->node_vec)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}
	return nullptr;
}

void ModelComponent::UpdateAnimationState()
{
#ifdef _DEBUG
	if (this->stop_anime_state_update) return;
#endif // DEBUG


	if (this->current_animation_index < 0) return;
	if (this->anime_state_pool.size() <= 0) return;

	for (auto& transition_info : this->anime_state_pool[this->current_animation_index].transition_info_pool)
	{
		if (transition_info->judgement->Judgement())
		{
			PlayAnimation(this->anime_state_pool[transition_info->next_anime_index], transition_info->blend_time);
			break;
		}
	}
}

void ModelComponent::SetAnimationState(AnimeIndex anime_index, bool loop)
{
	auto& anime_state = this->anime_state_pool[anime_index];
	anime_state.loop = loop;
}

void ModelComponent::AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time)
{
	auto& transition_info_pool = this->anime_state_pool[anime_index].transition_info_pool;
	// 遷移するアニメーションステート
	auto& transition_info =  transition_info_pool.emplace_back();
	transition_info = std::make_unique<AnimeTransitionInfo>();
	transition_info->next_anime_index = transition_anime_index;
	transition_info->judgement = std::move(judgement);
	transition_info->blend_time = blend_time;
}

#ifdef _DEBUG

void ModelComponent::DrawDebugGUI()
{
	int& anime_index = this->current_animation_index;
	if (anime_index < 0) return;

	const auto& animation = this->resource->GetAnimations()[anime_index];

	std::string play_anime_name = this->animation_name_pool[anime_index];
	ImGui::Checkbox("Stop Anime State Update", &this->stop_anime_state_update);
	ImGui::SliderFloat("Current Animation Seconds", &this->current_animation_seconds, 0.0f, animation.seconds_length);
	if (ImGuiComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index))
	{
		auto& anime_state = this->anime_state_pool[anime_index];
		PlayAnimation(anime_state, 0.0f);
	}
	ImGui::Checkbox("Animation Loop Flag", &this->animation_loop_flag);

	ImGui::InputFloat("Animation Blend Seconds", &this->animation_blend_seconds);
	ImGui::SliderFloat("Animation Blend Time", &this->animation_blend_time, 0.0f, this->animation_blend_seconds);
	ImGui::Checkbox("Animation End Flag", &this->animation_end_flag);
	
	if (this->is_draw_deletail) DrawDetail();
	else this->is_draw_deletail = ImGui::Button("Draw Animation Deletail");

	ImGui::Text(model_name);
}

void ModelComponent::DrawDetail()
{
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

	ImGui::Begin("Animation Detail", &is_draw_deletail, ImGuiWindowFlags_None);

	std::string select_anime_name = this->animation_name_pool[this->select_animation_index];
	ImGuiComboUI("Select Animation", select_anime_name, this->animation_name_pool, this->select_animation_index);

	auto& selct_anime_state = this->anime_state_pool[this->select_animation_index];
	
	// 再生
	if (ImGui::Button("Play Animation"))
	{
		PlayAnimation(selct_anime_state, 0.0f);
	}

	ImGui::Checkbox("Loop", &selct_anime_state.loop);
	// 遷移情報表示
	int transition_id = 0;
	for (auto& transition_info : selct_anime_state.transition_info_pool)
	{
		bool is_judgement_active = transition_info->judgement->GetIsActive();

		std::string label = "##" + std::to_string(transition_id);
		if (ImGui::Checkbox(label.c_str(), &is_judgement_active))
		{
			transition_info->judgement->SetIsActive(is_judgement_active);
		}

		ImGui::SameLine();
		
		// 非アクティブなら灰色にする
		if (!is_judgement_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

		auto& transition_anime_state = this->anime_state_pool[transition_info->next_anime_index];
		if (ImGui::CollapsingHeader(transition_anime_state.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat("Blend Time", &transition_info->blend_time);
			std::string judgement_name = "Judgement:";
			judgement_name += transition_info->judgement->GetName();
			ImGui::Text(judgement_name.c_str());
			transition_info->judgement->DrawDebugGUI();
		}

		++transition_id;
		if (!is_judgement_active) ImGui::PopStyleColor();
	}

	ImGui::End();
}

#endif // _DEBUG