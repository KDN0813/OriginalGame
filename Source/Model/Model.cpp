#include "Graphics/Graphics.h"
#include "Model/Model.h"
#include "Model/ModelResource.h"

// コンストラクタ
Model::Model(const char* filename)
	:current_animation_index(-1)
	, current_animation_seconds()
	, animation_blend_time()
	, animation_blend_seconds()
	, animation_loo_flag()
	, animation_end_flag()
	, dummy()
{
	// リソース読み込み
#if 1
	resource = std::make_shared<ModelResource>();
	resource->Load(Graphics::Instance().GetDevice(), filename);
#else
	resource = ResourceManager::Instance().LoadModelResource(filename);
#endif

	// ノード
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

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

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

// アニメーション更新処理
void Model::UpdateAnimation(float elapsedTIme)
{
	// 再生中でないなら更新しない
	if (!IsPlayAnimation()) return;

	// アニメーションブレンド率
	float blend_rate = 1.0f;

	if (animation_blend_time < animation_blend_seconds)
	{
		animation_blend_time += elapsedTIme;
		if (animation_blend_time >= animation_blend_seconds)
		{
			animation_blend_time = animation_blend_seconds;
		}
			//　ブレンド率計算
		blend_rate = animation_blend_time / animation_blend_seconds;
		blend_rate *= blend_rate;
	}

	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
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
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// 補間率を計算
			float rate = (current_animation_seconds - keyframe0.seconds) 
				/ (keyframe1.seconds - keyframe0.seconds);
			
			int nodeCount = static_cast<int>(nodes.size());
			
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				// アニメーションブレンドするなら
				if (blend_rate < 1.0f)
				{
					// 現在の姿勢と次のキーフレームとの姿勢の補間

					// スケールの線形補間
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S  = DirectX::XMVectorLerp(S0, S1, rate);
					
					// 角度の球面線形補間
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// 座標の線形補間
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

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
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// 座標の線形補間
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

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
	if (animation_end_flag)
	{
		animation_end_flag		= false;
		current_animation_index	= -1;
		return;
	}

	current_animation_seconds += elapsedTIme;

	// 再生時間を超えたら
	if (current_animation_seconds >= animation.seconds_length)
	{
		// ループ再生する場合
		if (animation_loo_flag)
		{
			// 再生時間を戻す
			current_animation_seconds -= animation.seconds_length;
			return;
		}
		// ループ再生しない場合
		else
		{
			animation_end_flag = true;
		}
	}
}

// アニメーション再生
void Model::PlayAnimation(int index, bool loop,float blendSeconds)
{
	current_animation_index	= index;
	current_animation_seconds = 0.0f;

	animation_loo_flag		= loop;
	animation_end_flag		= false;

	animation_blend_time		= 0.0f;
	animation_blend_seconds	= blendSeconds;
}

// アニメーション再生中か判定
bool Model::IsPlayAnimation() const
{
	// アニメーション番号が0以下なら
	if (current_animation_index < 0)return false;
	// アニメーション番号が保存されているアニメーションの数以上なら
	if (current_animation_index >= resource->GetAnimations().size()) return false;

	return true;
}

// ノード検索
Model::Node* Model::FindNode(const char* name)
{
	// 全てのノードを総当たりで名前比較する
	for (Node& node : nodes)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}

	// 見つからなかった
	return nullptr;
}
