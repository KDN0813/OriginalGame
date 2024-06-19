#pragma once

#include <DirectXMath.h>
#include <string>
#include <cereal/archives/binary.hpp>

namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    };
}

// シリアライズ・デシリアライズテストクラス
class CerealTest
{
public:
    CerealTest();
    ~CerealTest(){}

    struct PlayerData
    {
        //std::string name;
        DirectX::XMFLOAT3 pos;
        int hp;

        template<class T>
        void serialize(T& archive)
        {
            archive(pos, hp);
        }
    };

    void Serialize(const char* creal_file_name);
    void Deserialize(const char* creal_file_name);
    
    void DrawImGui();
public:
    PlayerData player_data;
};