#pragma once

#include <d3d11.h>
#include "Graphics/RenderContext.h"

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	/**
	 * \fn Render
	 * \brief �`�揈��(�J�n����I���܂ł��܂Ƃ߂��֐�)
	 * 
	 * \param dc
	 * \param rc
	 */
	virtual void Render(ID3D11DeviceContext* dc, const RenderContext& rc) {};

	virtual const char* GetName() = 0;
};

// TODO(08/15) �֐��̉��u���E�s�K�v�Ȃ�폜����
//private:
//	/**
//	 * \fn Begin
//	 * \brief �`��J�n
//	 * 
//	 * \param dc ID3D11DeviceContext�̃|�C���^
//	 * \param rc RenderContext
//	 */
//	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;
//	/**
//	 * \fn Draw
//	 * \brief �`��
//	 * 
//	 * \param dc ID3D11DeviceContext�̃|�C���^
//	 */
//	virtual void Draw(ID3D11DeviceContext* dc) = 0;
//	/**
//	 * \fn End
//	 * \brief �`��I��
//	 * 
//	 * \param context
//	 */
//	virtual void End(ID3D11DeviceContext* context) = 0;
