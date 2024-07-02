#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

// �C���X�^���V���O���f��
class InstancingModel
{
public:
	struct InstancingData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// �g�p����
		bool dummy[3];
	};
public:
	InstancingModel(const char* filename);
	~InstancingModel() {};

	// �g���Ă��Ȃ��ԍ������蓖�ĂĕԂ�
	int AllocateInstancingIndex();

	// ���蓖�Ă�ꂽ�ԍ����������
	void FreeInstancingIndex(int instancingIndex);

	// �s��v�Z
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// �e��擾�E�ݒ�֐�
	const ModelResource* GetResource() const { return resource.get(); }
	const size_t GetInstanceCount()const { return this->instance_cout; }
	// �V�F�[�_�[�ւ̐ݒ�p�o�b�t�@���擾
	BufferData GetBufferData(const ModelResource::Mesh& mesh) const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<InstancingData> instancing_data;
	size_t instance_cout;
};

