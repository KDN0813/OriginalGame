#pragma once

#include <DirectXMath.h>
#include <string>

// デバッグ用シリアライズ・デシリアライズ
class DebugCereal
{
public:
    DebugCereal();
    ~DebugCereal(){}

    struct PlayerData
    {
        std::string name;
        DirectX::XMFLOAT3 pos;
        int hp;

        template<class T>
        void serialize(T& archive);
    };

    void Serialize(const char* creal_file_name);
    void Deserialize(const char* creal_file_name);
    
    void DrawImGui();
public:
    PlayerData player_data;
};