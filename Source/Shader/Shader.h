#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Graphics/RenderContext.h"

class ShaderComponent;
class ModelResource;
class InstancingModelComponent;
class InstancingModelResource;
class InstanceData;

class Shader
{
	friend ShaderComponent;
public:
	Shader() {}
	virtual ~Shader() {}

	void AddShaderComponent(const std::shared_ptr<ShaderComponent>& shader_component);

	/**
	 * \fn Render
	 * \brief �`�揈��(�J�n����I���܂ł��܂Ƃ߂��֐�)
	 * 
	 * \param dc
	 * \param rc
	 */
	virtual void Render(ID3D11DeviceContext* dc, const RenderContext& rc) {};

	/**
	* \fn SetInstancingResource
	* \brief �C���X�^���V���O���\�[�X�̐ݒ�
	* 
	* \param model_resource ���f�����\�[�X�̃|�C���^
	* \param instancing_model_resource �C���X�^���V���O���f�����\�[�X�̃|�C���^
	* \return �������ݒ�ł�����true��Ԃ�
	*/
	virtual bool SetInstancingResource(
		ModelResource* model_resource,
		InstancingModelResource * instancing_model_resource
	) { return false; };
	/**
	 * \fn AddInstance
	 * \brief �C���X�^���X�̒ǉ�
	 */
	virtual void InstancingAdd(const InstanceData) {};

	// TODO(08/14)�V�F�[�_�[�R���|�[�l���g�N���X���쐬����֐����
private:
	/**
	 * \fn Begin
	 * \brief �`��J�n
	 * 
	 * \param dc ID3D11DeviceContext�̃|�C���^
	 * \param rc RenderContext
	 */
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;
	/**
	 * \fn Draw
	 * \brief �`��
	 * 
	 * \param dc ID3D11DeviceContext�̃|�C���^
	 */
	virtual void Draw(ID3D11DeviceContext* dc) = 0;
	/**
	 * \fn End
	 * \brief �`��I��
	 * 
	 * \param context
	 */
	virtual void End(ID3D11DeviceContext* context) = 0;

protected:
	std::vector<std::weak_ptr<ShaderComponent>> shader_component_Wptr_vector;
};