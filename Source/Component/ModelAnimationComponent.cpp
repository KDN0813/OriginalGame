#include "Debug/ImGuiHelper.h"
#include "ModelAnimationComponent.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/ModelComponent.h"

ModelAnimationComponent::ModelAnimationComponent(ID3D11Device* device, const char* filename)
{
	// リソース読み込み
	auto model_resource = ModelResourceManager::Instance()->LoadModelResource(device, filename);
	this->model_resource_Wptr = model_resource;

	this->animation_size = static_cast<int>(model_resource->GetAnimations().size());

	// アニメステートの初期設定
	{

		this->anime_state_pool.clear();
		for (AnimeIndex index = 0; index < this->animation_size; ++index)
		{
			const auto& animation = model_resource->GetAnimations()[index];
			auto& anime_state = this->anime_state_pool.emplace_back();
			anime_state.anime_index = index;
			anime_state.name = animation.name;
#ifdef _DEBUG
			this->animation_name_pool.emplace_back(animation.name);
#endif // _DEBUG
		}
	}
}

void ModelAnimationComponent::Update(float elapsed_time)
{
	UpdateAnimationState();

	UpdateAnimation(elapsed_time);
}

void ModelAnimationComponent::UpdateAnimation(float elapsed_time)
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

	const std::vector<ModelResource::Animation>& animations = model_resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

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
		if (this->current_animation_seconds >= keyframe0.seconds &&
			this->current_animation_seconds < keyframe1.seconds)
		{
			// 補間率を計算
			float rate = (this->current_animation_seconds - keyframe0.seconds)
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
					node.scale = S.GetFlaot3();

					// 角度の球面線形補間
					MYVECTOR4 R0 = node.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// 座標の線形補間
					MYVECTOR3 T0 = node.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					node.translate = T.GetFlaot3();
				}
				else
				{
					// 前のキーフレームと次のキーフレームの姿勢を補間

					// スケールの線形補間
					MYVECTOR3 S0 = key0.scale;
					MYVECTOR3 S1 = key1.scale;
					MYVECTOR3 S = S0.Lerp(S1, blend_rate);
					node.scale = S.GetFlaot3();

					// 角度の線形補間
					MYVECTOR4 R0 = key0.rotate;
					MYVECTOR4 R1 = key1.rotate;
					MYVECTOR4 R = R0.SLerp(R1, blend_rate);
					R.GetFlaot4(node.rotate);

					// 座標の線形補間
					MYVECTOR3 T0 = key0.translate;
					MYVECTOR3 T1 = key1.translate;
					MYVECTOR3 T = T0.Lerp(T1, blend_rate);
					node.translate = T.GetFlaot3();
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

void ModelAnimationComponent::PlayAnimation(int index, bool loop, float blend_seconds)
{
	this->current_animation_index = index;
	this->current_animation_seconds = 0.0f;

	this->animation_loop_flag = loop;
	this->animation_end_flag = false;

	this->animation_blend_time = 0.0f;
	this->animation_blend_seconds = blend_seconds;
}

void ModelAnimationComponent::PlayAnimation(const AnimeState& animation_info, float blend_seconds)
{
	this->current_animation_index = static_cast<int>(animation_info.anime_index);
	this->current_animation_seconds = 0.0f;

	this->animation_loop_flag = animation_info.loop;
	this->animation_end_flag = false;

	this->animation_blend_time = 0.0f;
	this->animation_blend_seconds = blend_seconds;
}

bool ModelAnimationComponent::IsPlayAnimation() const
{
	if (this->current_animation_index < 0)return false;
	if (this->current_animation_index >= this->animation_size) return false;
	if (this->animation_end_flag) return false;
	return true;
}

bool ModelAnimationComponent::IsTransitionReady()
{
	// 再生中でなければ準備完了
	if (!IsPlayAnimation()) return true;

	auto& anime_state = this->anime_state_pool[this->current_animation_index];

	if (anime_state.transition_ready_time >= 0.0f &&
		anime_state.transition_ready_time <= this->current_animation_seconds) return true;

	return false;
}

bool ModelAnimationComponent::PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen)
{
	if (!judgemen) return false;
#ifdef _DEBUG
	if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG
	auto owner = GetOwner();
	if (!owner) return false;

	// 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
	if (judgemen->GetRequireTransitionReady() && !IsTransitionReady()) return false;

	return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

void ModelAnimationComponent::UpdateAnimationState()
{
#ifdef _DEBUG
	if (this->stop_anime_state_update) return;
#endif // DEBUG


	if (this->current_animation_index < 0) return;
	if (this->anime_state_pool.size() <= 0) return;

	for (auto& transition_info : this->anime_state_pool[this->current_animation_index].transition_info_pool)
	{
		if (PerformTransitionJudgement(transition_info->judgement.get()))
		{
			PlayAnimation(this->anime_state_pool[transition_info->next_anime_index], transition_info->blend_time);
			break;
		}
	}
}

void ModelAnimationComponent::SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time)
{
	if (!this->anime_state_pool.size()) return;

	auto& anime_state = this->anime_state_pool[anime_index];
	anime_state.loop = loop;
	anime_state.transition_ready_time = transition_ready_time;
}

void ModelAnimationComponent::AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time)
{
	if (!this->anime_state_pool.size()) return;
	auto& transition_info_pool = this->anime_state_pool[anime_index].transition_info_pool;
	// 遷移するアニメーションステート
	auto& transition_info = transition_info_pool.emplace_back();
	transition_info = std::make_unique<AnimeTransitionInfo>();
	transition_info->next_anime_index = transition_anime_index;
	transition_info->judgement = std::move(judgement);
	transition_info->blend_time = blend_time;
}

#ifdef _DEBUG

void ModelAnimationComponent::DrawDebugGUI()
{
	auto model_resource = model_resource_Wptr.lock();
	if (!model_resource) return;

	int& anime_index = this->current_animation_index;
	if (anime_index < 0) return;

	if (model_resource->GetAnimations().size())
	{
		const auto& animation = model_resource->GetAnimations()[anime_index];

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
	}
}

void ModelAnimationComponent::DrawDetail()
{
	auto model_resource = model_resource_Wptr.lock();
	if (!model_resource) return;

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

	// アニメーション情報
	if (ImGui::CollapsingHeader("Animation Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Loop", &selct_anime_state.loop);
		ImGui::InputFloat("Transition Ready Time", &selct_anime_state.transition_ready_time);

		// 再生終了時間
		const auto& anime = model_resource->GetAnimations()[this->select_animation_index];
		float seconds_length = anime.seconds_length;
		ImGui::InputFloat("Seconds Length", &seconds_length);
	}
	// 遷移情報表示
	if (ImGui::CollapsingHeader("Transition Info", ImGuiTreeNodeFlags_DefaultOpen))
	{

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
			if (ImGui::CollapsingHeader(transition_anime_state.name.c_str(), ImGuiTreeNodeFlags_None))
			{
				ImGui::InputFloat("Blend Time", &transition_info->blend_time);
				std::string judgement_name = "Judgement:";
				judgement_name += transition_info->judgement->GetName();
				ImGui::Text(judgement_name.c_str());
				transition_info->judgement->DrawCommonDebugGUI(transition_id);
				transition_info->judgement->DrawDebugGUI(transition_id);
			}

			++transition_id;
			if (!is_judgement_active) ImGui::PopStyleColor();
		}
	}

	ImGui::End();
}

#endif // DEBUG