#pragma once

#include <d3d11.h>

/**
* ���_�V�F�[�_�[�̐���
 * \param device �f�o�C�X
 * \param cso_name �t�@�C����
 * \param vertex_shader �쐬�������_�V�F�[�_�[�̊i�[��̃|�C���^
 * \param input_layout �쐬�������̓��C�A�E�g�̊i�[��̃|�C���^
 * \param input_element_desc D3D11_INPUT_ELEMENT_DESC�̃|�C���^
 * \param num_elements ���̓��C�A�E�g�̐�
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
 * �s�N�Z���V�F�[�_�[�̐���
 * \param device �f�o�C�X
 * \param cso_name �t�@�C����
 * \param pixel_shader �쐬�����s�N�Z���V�F�[�_�[�̊i�[��̃|�C���^
 */
HRESULT CreatePsFromCso(
    ID3D11Device* device
    , const char* cso_name
    , ID3D11PixelShader** pixel_shader
);
