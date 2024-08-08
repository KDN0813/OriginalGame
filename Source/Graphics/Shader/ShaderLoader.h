#pragma once

#include <d3d11.h>

/**
* 頂点シェーダーの生成
 * \param device デバイス
 * \param cso_name ファイル名
 * \param vertex_shader 作成した頂点シェーダーの格納先のポインタ
 * \param input_layout 作成した入力レイアウトの格納先のポインタ
 * \param input_element_desc D3D11_INPUT_ELEMENT_DESCのポインタ
 * \param num_elements 入力レイアウトの数
 */
HRESULT CreateVsFromCso(
    ID3D11Device* device
    , const char* cso_name
    , ID3D11VertexShader** vertex_shader
    ,ID3D11InputLayout** input_layout
    ,D3D11_INPUT_ELEMENT_DESC* input_element_desc
    ,UINT num_elements
);

/**
 * ピクセルシェーダーの生成
 * \param device デバイス
 * \param cso_name ファイル名
 * \param pixel_shader 作成したピクセルシェーダーの格納先のポインタ
 */
HRESULT CreatePsFromCso(
    ID3D11Device* device
    , const char* cso_name
    , ID3D11PixelShader** pixel_shader
);
