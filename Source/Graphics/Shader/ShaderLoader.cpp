#include "Graphics/Shader/ShaderLoader.h"

#include "System/Misc.h"
#include <sstream>
#include <memory>
#include <WICTextureLoader.h>

HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");           //�@�t�@�C�����J��
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);                 // �t�@�C���̍ŏ��̈ʒu���t�@�C���̏I�[�Ɉړ�
    long cso_sz{ ftell(fp) };               // �t�@�C���̏I�[�̈ʒu��ۑ�(�f�[�^�̃T�C�Y)
    fseek(fp, 0, SEEK_SET);                 // �t�@�C���̍ŏ��̈ʒu���t�@�C���̐擪�ɖ߂�

    // �J�����f�[�^�̊i�[��
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };

    fread(cso_data.get(), cso_sz, 1, fp);   // �t�@�C���̓ǂݎ��
    fclose(fp);                             // �t�@�C�������

    HRESULT hr{ S_OK };
    // ���_�V�F�[�_�[�̍쐬
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    if (input_layout)
    {
        // ���̓��C�A�E�g�쐬
        hr = device->CreateInputLayout(input_element_desc, num_elements, cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    return hr;
}

HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");                        // �t�@�C�����J��
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);                             // �t�@�C���̍ŏ��̈ʒu���t�@�C���̏I�[�Ɉړ�
    long cso_sz{ ftell(fp) };                           // �t�@�C���̏I�[�̈ʒu��ۑ�
    fseek(fp, 0, SEEK_SET);                             // �t�@�C���̍ŏ��̈ʒu���t�@�C���̐擪�ɖ߂�

    // �J�����f�[�^�̊i�[��
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };

    fread(cso_data.get(), cso_sz, 1, fp);               // �t�@�C���̓ǂݎ��
    fclose(fp);                                         // �t�@�C�������

    HRESULT hr{ S_OK };
    // �s�N�Z���V�F�[�_�[�̍쐬
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    return hr;
}
