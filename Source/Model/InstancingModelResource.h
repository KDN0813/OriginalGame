#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include "Model\InstancingConstants.h"

using BoneTransform = DirectX::XMFLOAT4X4;
using BoneTransformTextureData = std::vector<BoneTransform>;

class ModelResource;

class InstancingModelResource
{
public:
    /**
     * @fn InstancingModelResource
     * @brief コンストラ
     * 
     * \param device ID3D11Deviceのポインタを返す
     * \param filename モデルのファイル名
     */
    InstancingModelResource(ID3D11Device* device, const char* filename);
    ~InstancingModelResource() {};

    const UINT& GetBoneTransformCount() { return this->bone_transform_count; }
    const std::vector<UINT>& GetMeshOffsets() { return this->mesh_offsets; }
    const std::vector<UINT>& GetAnimationLengths() { return this->animation_lengths; }
    const std::vector<UINT>& GetAnimationOffsets() { return this->animation_offsets; }
    ID3D11ShaderResourceView*const* GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
    const int GetModelId() const { return this->model_id; }
    void SetModelId(int model_id) { this->model_id = model_id; }

private:
    // ボーントランスフォームテクスチャ
    Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

    // GPU側でボーントランスフォームテクスチャの値を取得するのに必要なパラメータ
    UINT bone_transform_count = {};		    // ボーントランスフォームの数
    std::vector<UINT> mesh_offsets;			// BTT(ボーントランスフォームテクスチャ)で使用するメッシュ毎の開始位置までのオフセット値
    std::vector<UINT> animation_lengths;	// アニメーションの長さ(フレーム数)
    std::vector<UINT> animation_offsets;	// BTTで使用するアニメーション毎の開始位置までのオフセット値

    int model_id = -1;  // モデルのID

    // Hack(meayama) 外部ファイル読み込むようにする
#pragma region    ボーントランスフォームテクスチャ作成用
private:
    struct Node
    {
        const char* name = nullptr;
        Node* parent = nullptr;
        DirectX::XMFLOAT3	scale = {};
        DirectX::XMFLOAT4	rotate = {};
        DirectX::XMFLOAT3	translate = {};
        DirectX::XMFLOAT4X4	local_transform = {};
        DirectX::XMFLOAT4X4	world_transform = {};

        std::vector<Node*>	children;
    };

    /**
     * \fn CreateBoneTransformTexture
     * \brief ボーントランスフォームテクスチャを作成・bone_transform_count等のパラメータ設定
     * 
     * \param device ID3D11Deviceのポインタ
     */
    void CreateBoneTransformTexture(ID3D11Device* device, ModelResource* resource);
    void PlayAnimation(int index);
    void UpdateAnimation(float elapsed_time, ModelResource* resource);
    bool IsPlayAnimation(ModelResource* resource)const;
    void UpdateTransform();

    std::vector<Node> node_vec;
    int current_animation_index = -1;	// 値が(-1)なら停止している
    float current_animation_seconds = 0.0f;	// 経過時間
#pragma endregion   ボーントランスフォームテクスチャ作成用
};

