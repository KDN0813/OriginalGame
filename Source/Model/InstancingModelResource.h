#pragma once
#include <memory>
#include "Model/ModelResource.h"

using BoneTransform = DirectX::XMFLOAT4X4;
using BoneTransformTextureData = std::vector<BoneTransform>;

class InstancingModelComponent;

class InstancingModelResource
{
public:
    static const int MAX_INSTANCES = 512;
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

private:
    std::shared_ptr<ModelResource>	resource;

    // ボーントランスフォームテクスチャ
    Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

    // GPU側でボーントランスフォームテクスチャの値を取得するのに必要なパラメータ
    UINT bone_transform_count;				// ボーントランスフォームの数
    std::vector<UINT> mesh_offsets;			// BTTで使用するメッシュ毎の開始位置までのオフセット値
    std::vector<UINT> animation_lengths;	// アニメーションの長さ(フレーム数)
    std::vector<UINT> animation_offsets;	// BTTで使用するアニメーション毎の開始位置までのオフセット値

    std::vector<InstancingModelComponent*> models;  // 描画するモデル
#pragma region    ボーントランスフォームテクスチャ作成用
private:
    struct Node
    {
        const char* name;
        Node* parent;
        DirectX::XMFLOAT3	scale;
        DirectX::XMFLOAT4	rotate;
        DirectX::XMFLOAT3	translate;
        DirectX::XMFLOAT4X4	localTransform;
        DirectX::XMFLOAT4X4	worldTransform;

        std::vector<Node*>	children;
    };

    /**
     * \fn CreateBoneTransformTexture
     * \brief ボーントランスフォームテクスチャを作成・bone_transform_count等のパラメータ設定
     * 
     * \param device ID3D11Deviceのポインタ
     */
    void CreateBoneTransformTexture(ID3D11Device* device);
    void PlayAnimation(int index);
    void UpdateAnimation(float elapsed_time);
    bool IsPlayAnimation()const;
    void UpdateTransform();

    std::vector<Node> nodes;
    int current_animation_index = -1;	// 値が(-1)なら停止している
    float current_animation_seconds = 0.0f;	// 経過時間
#pragma endregion   ボーントランスフォームテクスチャ作成用
};

