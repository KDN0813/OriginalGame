#pragma once

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// TODO (06/24)開発指針が定まり次第改善
class ModelResource
{
public:
	ModelResource() {}
	virtual ~ModelResource() {}

#pragma region ModelParam

	using NodeId = UINT64;

	struct Node
	{
		NodeId				id;
		std::string			name;
		std::string			path;
		int					parent_index;	//親の番号
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Material
	{
		std::string			name;
		std::string			texture_filename;
		DirectX::XMFLOAT4	color = { 0.8f, 0.8f, 0.8f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Subset
	{
		UINT		start_index = 0;
		UINT		index_count = 0;
		int			material_index = 0;

		Material* material = nullptr;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Vertex
	{
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT3	normal = { 0, 0, 0 };
		DirectX::XMFLOAT3	tangent = { 0, 0, 0 };
		DirectX::XMFLOAT2	texcoord = { 0, 0 };
		DirectX::XMFLOAT4	color = { 1, 1, 1, 1 };
		DirectX::XMFLOAT4	bone_weight = { 1, 0, 0, 0 };
		DirectX::XMUINT4	bone_index = { 0, 0, 0, 0 };

		template<class Archive>
		void serialize(Archive& archive, int version);
	};
	/**
	 * ボーントランスフォームを取得するために必要なパラメータ
	 * \param bone_size 1回の描画で使用するボーントランスフォームの数
	 * \param mesh_first_bone_index 描画するメッシュの先頭のボーントランスフォームのインデックス
	 * \param animation_first_bone_index 使うアニメーションの先頭のボーントランスフォームのインデックス
	 */
	struct BoneTransformData
	{
		UINT bone_size = { 0 };
		UINT mesh_first_bone_index = { 0 };
		UINT animation_first_bone_index = { 0 };
	};

	struct Mesh	// 1パーツ
	{
		std::vector<Vertex>						vertices;
		std::vector<BoneTransformData>			bone_transform_datas;
		std::vector<UINT>						indices;
		std::vector<Subset>						subsets;

		int										node_index;
		std::vector<int>						node_indices;
		std::vector<DirectX::XMFLOAT4X4>		offset_transforms;

		DirectX::XMFLOAT3						bounds_min;
		DirectX::XMFLOAT3						bounds_max;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	bone_transform_data_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct NodeKeyData
	{
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Keyframe
	{
		float						seconds;
		std::vector<NodeKeyData>	node_keys;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Animation
	{
		std::string					name;
		float						seconds_length;	// 再生時間
		std::vector<Keyframe>		keyframes;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

#pragma endregion ModelParam

	// 各種データ取得
	const std::vector<Mesh>& GetMeshes() const { return meshes; }
	const std::vector<Node>& GetNodes() const { return nodes; }
	const std::vector<Animation>& GetAnimations() const { return animations; }
	const std::vector<Material>& GetMaterials() const { return materials; }

	// 読み込み
	void Load(ID3D11Device* device, const char* filename);

protected:
	// モデルセットアップ
	void BuildModel(ID3D11Device* device, const char* dirname);

	// シリアライズ
	void Serialize(const char* filename);

	// デシリアライズ
	void Deserialize(const char* filename);

	// ノードインデックスを取得する
	int FindNodeIndex(NodeId nodeId) const;

protected:
	std::vector<Node>		nodes;
	std::vector<Material>	materials;
	std::vector<Mesh>		meshes;
	std::vector<Animation>	animations;
};
