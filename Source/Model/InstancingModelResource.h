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
     * @brief �R���X�g��
     * 
     * \param device ID3D11Device�̃|�C���^��Ԃ�
     * \param filename ���f���̃t�@�C����
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
    // �{�[���g�����X�t�H�[���e�N�X�`��
    Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

    // GPU���Ń{�[���g�����X�t�H�[���e�N�X�`���̒l���擾����̂ɕK�v�ȃp�����[�^
    UINT bone_transform_count = {};		    // �{�[���g�����X�t�H�[���̐�
    std::vector<UINT> mesh_offsets;			// BTT(�{�[���g�����X�t�H�[���e�N�X�`��)�Ŏg�p���郁�b�V�����̊J�n�ʒu�܂ł̃I�t�Z�b�g�l
    std::vector<UINT> animation_lengths;	// �A�j���[�V�����̒���(�t���[����)
    std::vector<UINT> animation_offsets;	// BTT�Ŏg�p����A�j���[�V�������̊J�n�ʒu�܂ł̃I�t�Z�b�g�l

    int model_id = -1;  // ���f����ID

    // Hack(meayama) �O���t�@�C���ǂݍ��ނ悤�ɂ���
#pragma region    �{�[���g�����X�t�H�[���e�N�X�`���쐬�p
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
     * \brief �{�[���g�����X�t�H�[���e�N�X�`�����쐬�Ebone_transform_count���̃p�����[�^�ݒ�
     * 
     * \param device ID3D11Device�̃|�C���^
     */
    void CreateBoneTransformTexture(ID3D11Device* device, ModelResource* resource);
    void PlayAnimation(int index);
    void UpdateAnimation(float elapsed_time, ModelResource* resource);
    bool IsPlayAnimation(ModelResource* resource)const;
    void UpdateTransform();

    std::vector<Node> node_vec;
    int current_animation_index = -1;	// �l��(-1)�Ȃ��~���Ă���
    float current_animation_seconds = 0.0f;	// �o�ߎ���
#pragma endregion   �{�[���g�����X�t�H�[���e�N�X�`���쐬�p
};

