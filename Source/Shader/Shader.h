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
	virtual void Render() = 0;

	virtual const char* GetName() = 0;

#ifdef _DEBUG
public:
	virtual void DrawDebugGUI() = 0;

#endif // _DEBUG
};
