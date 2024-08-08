#include "Graphics/Shader/ShaderLoader.h"

#include "System/Misc.h"
#include <sstream>
#include <memory>
#include <WICTextureLoader.h>

HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");           //　ファイルを開く
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);                 // ファイルの最初の位置をファイルの終端に移動
    long cso_sz{ ftell(fp) };               // ファイルの終端の位置を保存(データのサイズ)
    fseek(fp, 0, SEEK_SET);                 // ファイルの最初の位置をファイルの先頭に戻す

    // 開いたデータの格納先
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };

    fread(cso_data.get(), cso_sz, 1, fp);   // ファイルの読み取り
    fclose(fp);                             // ファイルを閉じる

    HRESULT hr{ S_OK };
    // 頂点シェーダーの作成
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    if (input_layout)
    {
        // 入力レイアウト作成
        hr = device->CreateInputLayout(input_element_desc, num_elements, cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    return hr;
}

HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");                        // ファイルを開く
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);                             // ファイルの最初の位置をファイルの終端に移動
    long cso_sz{ ftell(fp) };                           // ファイルの終端の位置を保存
    fseek(fp, 0, SEEK_SET);                             // ファイルの最初の位置をファイルの先頭に戻す

    // 開いたデータの格納先
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };

    fread(cso_data.get(), cso_sz, 1, fp);               // ファイルの読み取り
    fclose(fp);                                         // ファイルを閉じる

    HRESULT hr{ S_OK };
    // ピクセルシェーダーの作成
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    return hr;
}
