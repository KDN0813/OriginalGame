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
	 * \brief •`‰æˆ—(ŠJn‚©‚çI—¹‚Ü‚Å‚ğ‚Ü‚Æ‚ß‚½ŠÖ”)
	 * 
	 * \param dc
	 * \param rc
	 */
	virtual void Render(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	virtual const char* GetName() = 0;
};
