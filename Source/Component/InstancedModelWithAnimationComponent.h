#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <memory>
#include "Model/ModelCommonData.h"
#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
#endif // _DEBUG

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

// �A�j���[�V�����t�C���X�^���X���f��
class InstancedModelWithAnimationComponent : public Component
{
public:
	struct InstancedModelParam
	{
		UINT anime_index = 0;
		float current_animation_seconds = 0.0f;		// ���݂̍Đ�����
		UINT old_anime_index = 0;
		float old_current_animation_seconds = 0.0f;	// �O��̍Đ�����
		float animation_blend_time = 0.0f;			// �A�j���[�V�����u�����h�̌o�ߎ���
		float animation_blend_seconds = 0.0f;		// �A�j���[�V�����u�����h�̎���
		int anime_speed = 1;
		bool anime_loop = false;
		bool anime_play = false;					// �A�j���[�V�������Đ����ł��邩
		DirectX::XMFLOAT4 base_color = { 1.0f,1.0f ,1.0f, 1.0f };
	};
public:
	// �Đ�����A�j���[�V�����̃p�����[�^
	struct PlayAnimeParam
	{
		UINT anime_index = 0;
		bool loop = false;
		float anime_speed = 1.0f;
	};
public:
	InstancedModelWithAnimationComponent(InstancedModelParam param, const char* filename);

	// ���X�^�[�g����
	void ReStart() override;      // �p�����[�^�̏�����
	void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// �D��x
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::LOW; }

	void PlayAnimation(int animeIndex, bool loop = true, float blend_time = 0.2f);

	void UpdateAnimation(float elapsed_time);

	// �A�j���[�V�������Đ����ł��邩
	bool IsPlayAnime() { return this->param.anime_play; }

	// �e�擾�E�ݒ�֐�
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	UINT GetAnimeFrame();			// ���݂̍Đ��t���[�������擾
	UINT GetOldAnimeFrame();		// �O��̃A�j���̍Đ��t���[�������擾
	DirectX::XMFLOAT4 GetBaseColor()const { return this->param.base_color; }
	float GetAlpha()const { return this->param.base_color.w; }
	float GetCurrentAnimationSeconds() const { return this->param.current_animation_seconds; }
	UINT GetAnimationStartOffset();	// ���ݍĐ����Ă���A�j���[�V�����̃I�t�Z�b�g�l���擾
	UINT GetOldAnimationStartOffset();	// �O��Đ����Ă����A�j���[�V�����̃I�t�Z�b�g�l���擾
	int GetModelId();
	float GetAnimationBlendRate();	// �A�j���[�V�����̕�ԗ����擾

	std::vector<DirectX::BoundingBox> GetBoundingBoxs();

	void SetAlpha(float a) { this->param.base_color.w = a; }
	void GetBaseColor(const DirectX::XMFLOAT4 base_color) { this->param.base_color = base_color; }
protected:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	InstancedModelParam param;
	InstancedModelParam default_param;

protected:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();

	void DrawDebugPrimitive()  override;
	/**
	 * �f�o�b�O�v���~�e�B�u�\���pImGui
	 */
	void DrawDebugPrimitiveGUI()  override;
	bool IsDebugPrimitive() override { return true; }   // DebugPrimitive�����݂��邩
protected:
	std::vector<AABBCorners> AABB_corners_vec;

	std::vector<std::string> animation_name_pool;
	const char* model_filename;
#endif // _DEBUG
};

