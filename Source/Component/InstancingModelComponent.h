#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

/**
* \brief GPU�ɑ���f�[�^
*
* \param animation_start_offset �o�b�t�@���Ŏg�p����A�j���[�V�����̊J�n�ʒu�������I�t�Z�b�g�l
* \param anime_frame ���݂̃t���[��
* \param world_transform ���[���h�g�����X�t�H�[��
*/
struct InstanceData
{
	UINT animation_start_offset;
	UINT anime_frame;
	DirectX::XMFLOAT4X4 world_transform{};
};

class InstancingModelComponent : public Component
{
public:
public:
	InstancingModelComponent(ID3D11Device* device, const char* filename);
	
    void Update(float elapsedTime) override;

	const char* GetName()const override { return "InstancingModelComponent"; }

	void PlayAnimetion(int animeIndex, bool loop = true);

	// �e�擾�E�ݒ�֐�
	const UINT& GetAnimeFrame() { return this->anime_frame; }
	const UINT& GetAnimeFrame() { return this->anime_frame; }
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	const InstanceData GetInstanceData();
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	UINT anime_frame;
	UINT anime_index;
	bool anime_loop;
	bool anime_play;

private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;
};

