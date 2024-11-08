#pragma once

#include <d3d11.h>

namespace CreateShader
{
    HRESULT VsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
    HRESULT GsFromCso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
    HRESULT PsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);
}