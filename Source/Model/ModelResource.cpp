#include <stdlib.h>
#include <fstream>
#include <functional>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <WICTextureLoader.h>
#include "System\MyMath\MYMATRIX.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "System/Misc.h"
#include "System/Logger.h"
#include "Model/ModelResource.h"

#pragma region // シリアライズ

// CEREALバージョン定義
CEREAL_CLASS_VERSION(ModelResource::Node, 1)
CEREAL_CLASS_VERSION(ModelResource::Material, 1)
CEREAL_CLASS_VERSION(ModelResource::Subset, 1)
CEREAL_CLASS_VERSION(ModelResource::Vertex, 1)
CEREAL_CLASS_VERSION(ModelResource::Mesh, 1)
CEREAL_CLASS_VERSION(ModelResource::NodeKeyData, 1)
CEREAL_CLASS_VERSION(ModelResource::Keyframe, 1)
CEREAL_CLASS_VERSION(ModelResource::Animation, 1)
CEREAL_CLASS_VERSION(ModelResource, 1)

namespace DirectX
{
	template<class Archive>
	void serialize(Archive& archive, XMUINT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12), cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22), cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32), cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42), cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}
}

template<class Archive>
void ModelResource::Node::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(id),
		CEREAL_NVP(name),
		CEREAL_NVP(path),
		CEREAL_NVP(parent_index),
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::Material::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(texture_filename),
		CEREAL_NVP(color)
	);
}

template<class Archive>
void ModelResource::Subset::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(start_index),
		CEREAL_NVP(index_count),
		CEREAL_NVP(material_index)
	);
}

template<class Archive>
void ModelResource::Vertex::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(position),
		CEREAL_NVP(normal),
		CEREAL_NVP(tangent),
		CEREAL_NVP(texcoord),
		CEREAL_NVP(color),
		CEREAL_NVP(bone_weight),
		CEREAL_NVP(bone_index)
	);
}

template<class Archive>
void ModelResource::Mesh::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(vertices),
		CEREAL_NVP(indices),
		CEREAL_NVP(subsets),
		CEREAL_NVP(node_index),
		CEREAL_NVP(node_indices),
		CEREAL_NVP(offset_transforms),
		CEREAL_NVP(bounds_min),
		CEREAL_NVP(bounds_max)
	);
}

template<class Archive>
void ModelResource::NodeKeyData::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::Keyframe::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(seconds),
		CEREAL_NVP(node_keys)
	);
}

template<class Archive>
void ModelResource::Animation::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(seconds_length),
		CEREAL_NVP(keyframes)
	);
}

#pragma endregion // シリアライズ

DirectX::BoundingBox ModelResource::GetDefaultBoundingBox(size_t i) const
{
	_ASSERT_EXPR_W(i < this->default_bounding_box_vec.size(), L"indexがボックスの最大値を超えています"); 
	i = (i < this->default_bounding_box_vec.size()) ? i : 0;
	return this->default_bounding_box_vec[i];
}

// 読み込み
void ModelResource::Load(ID3D11Device* device, const char* filename)
{
	// ディレクトリパス取得
	char drive[32], dir[256], dirname[256];
	::_splitpath_s(filename, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
	::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);

	// デシリアライズ
	Deserialize(filename);

	for (const auto animation : this->animation_vec)
	{
		MyHash name(animation.name);
		this->animation_store.AddData(name, animation);
	}

	// モデル構築
	BuildModel(device, dirname);

	// バンディングボックス作成
	{
		this->default_bounding_box_vec.resize(this->mesh_vec.size());

		for (size_t i = 0; i < this->mesh_vec.size(); ++i)
		{
			const auto& mesh = this->mesh_vec[i];

			MYVECTOR3 Center = (MYVECTOR3(mesh.bounds_min) + MYVECTOR3(mesh.bounds_max)) * 0.5f;
			MYVECTOR3 Extents = (MYVECTOR3(mesh.bounds_max) - MYVECTOR3(mesh.bounds_min)) * 0.5f;
			DirectX::XMFLOAT3 center;
			DirectX::XMFLOAT3 extents;
			Center.GetFlaot3(center);
			Extents.GetFlaot3(extents);
			this->default_bounding_box_vec[i] = { center,extents };

			MYMATRIX World_transform;
			const auto& node = node_vec[mesh_vec[i].node_index];
			World_transform.SetLocalMatrix(node.scale, node.rotate, node.translate);

			this->default_bounding_box_vec[i].Transform(this->default_bounding_box_vec[i], World_transform.GetMatrix());
		}
	}
}

// モデル構築
void ModelResource::BuildModel(ID3D11Device* device, const char* dirname)
{
	for (Material& material : material_vec)
	{
		// 相対パスの解決
		char filename[256];
		::_makepath_s(filename, 256, nullptr, dirname, material.texture_filename.c_str(), nullptr);

		// マルチバイト文字からワイド文字へ変換
		wchar_t wfilename[256];
		::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

		// テクスチャ読み込み
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilename, resource.GetAddressOf(), material.shader_resource_view.GetAddressOf());
		if (FAILED(hr))
		{
			// WICでサポートされていないフォーマットの場合（TGAなど）は
			// STBで画像読み込みをしてテクスチャを生成する
			int width, height, bpp;
			unsigned char* pixels = stbi_load(filename, &width, &height, &bpp, STBI_rgb_alpha);
			if (pixels != nullptr)
			{
				D3D11_TEXTURE2D_DESC desc = { 0 };
				desc.Width = width;
				desc.Height = height;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA data;
				::memset(&data, 0, sizeof(data));
				data.pSysMem = pixels;
				data.SysMemPitch = width * 4;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
				HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				hr = device->CreateShaderResourceView(texture.Get(), nullptr, material.shader_resource_view.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				// 後始末
				stbi_image_free(pixels);
			}
			else
			{
				// 読み込み失敗したらダミーテクスチャを作る
				LOG("load failed : %s\n", filename);

				const int width = 8;
				const int height = 8;
				UINT pixels[width * height];
				::memset(pixels, 0xFF, sizeof(pixels));

				D3D11_TEXTURE2D_DESC desc = { 0 };
				desc.Width = width;
				desc.Height = height;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA data;
				::memset(&data, 0, sizeof(data));
				data.pSysMem = pixels;
				data.SysMemPitch = width;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
				HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				hr = device->CreateShaderResourceView(texture.Get(), nullptr, material.shader_resource_view.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
			}
		}
	}

	for (Mesh& mesh : mesh_vec)
	{
		// サブセット
		for (Subset& subset : mesh.subsets)
		{
			subset.material = &material_vec.at(subset.material_index);
		}

		// 頂点バッファ
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = mesh.vertices.data();
			subresource_data.SysMemPitch = 0;
			subresource_data.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// インデックスバッファ
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresource_data.pSysMem = mesh.indices.data();
			subresource_data.SysMemPitch = 0; //Not use for index buffers.
			subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresource_data, mesh.index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
	}
}

// シリアライズ
void ModelResource::Serialize(const char* filename)
{
	std::ofstream ostream(filename, std::ios::binary);
	if (ostream.is_open())
	{
		cereal::BinaryOutputArchive archive(ostream);

		try
		{
			archive(
				CEREAL_NVP(node_vec),
				CEREAL_NVP(material_vec),
				CEREAL_NVP(mesh_vec),
				CEREAL_NVP(animation_vec)
			);
		}
		catch (...)
		{
			LOG("model deserialize failed.\n%s\n", filename);
			return;
		}
	}
}

// デシリアライズ
void ModelResource::Deserialize(const char* filename)
{
	std::ifstream istream(filename, std::ios::binary);
	if (istream.is_open())
	{
		cereal::BinaryInputArchive archive(istream);

		try
		{
			archive(
				CEREAL_NVP(node_vec),
				CEREAL_NVP(material_vec),
				CEREAL_NVP(mesh_vec),
				CEREAL_NVP(animation_vec)
			);
		}
		catch (...)
		{
			LOG("model deserialize failed.\n%s\n", filename);
			return;
		}
	}
	else
	{
		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), "File not found > %s", filename);
		_ASSERT_EXPR_A(false, buffer);
	}
}

// ノードインデックスを取得する
int ModelResource::FindNodeIndex(NodeId nodeId) const
{
	int node_count = static_cast<int>(node_vec.size());
	for (int i = 0; i < node_count; ++i)
	{
		if (node_vec[i].id == nodeId)
		{
			return i;
		}
	}
	return -1;
}
