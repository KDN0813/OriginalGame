struct VS_OUT
{
    float4 position : SV_POSITION;      // 頂点座標
    float4 color : COLOR;               // 色
    float2 texcoord : TEXCOORD;         // テクスチャ
};
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;           // ワールド行列
    float4 material_color;              // マテリアルのカラー
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection; // ビュー行列
    float4 camera_position;             // カメラの位置
};