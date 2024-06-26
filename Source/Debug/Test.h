#pragma once

#include <DirectXMath.h>
#include <string>

#include "Model/Model.h"

// シリアライズ・デシリアライズテストクラス
class CerealTest
{
public:
    CerealTest();
    ~CerealTest(){}

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

class TestObject
{
public:
    TestObject(const char* filename);
    ~TestObject() {};
    

    void Update(float elapsedTime);
    void Render();

    void DrawImGUi();
private:
    void UpdateTransform();

private:
    std::unique_ptr<Model> model;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT4X4 transform;
};
