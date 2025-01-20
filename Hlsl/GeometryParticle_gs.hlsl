#include "GeometryParticle.hlsli"
#include "./ParticleDisp.h"

Texture2D color_map : register(t0);

// 1頂点から4頂点生成する
[maxvertexcount(4)]
void main(point GsIn gin[1], inout TriangleStream<PsIn> output)
{
    float texture_width = 0.0f;
    float texture_height = 0.0f;
    
    // テクスチャサイズ取得
    color_map.GetDimensions(texture_width, texture_height);
    
    // ビルボード化するために、
    // 頂点座標をワールド空間＞ビュー空間へ変換
    float4 pos = mul(float4(gin[0].position, 1.0f), view_matrix);

	// 点を面にするため4頂点の座標を生成
    float rot = gin[0].param.rot;
    float s = sin(rot);
    float c = cos(rot);
    float4 right = float4(c, -s, 0, 0) * (texture_width * 0.5 * gin[0].param.scale.x);
    float4 up = float4(s, c, 0, 0) * (texture_height * 0.5 * gin[0].param.scale.y);

    //  4角形ポリゴンを生成
    float4 pos_left_top = pos - right + up;
    float4 pos_left_bottom = pos - right - up;
    float4 pos_right_top = pos + right + up;
    float4 pos_right_bottom = pos + right - up;

	//  UV座標の切り取り位置をtypeから算出
    float w = 1.0;
    float h = 1.0;
    
    float2 tex_pos;
    if (gin[0].param.texture_no == EFFECT_HIT)
    { // 斬撃
        tex_pos.x = TEX_P_SIZE * 0.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 0.0f / TEX_SIZE;
    }
    else if (gin[0].param.texture_no == EFFECT_HIT2)
    { // ヒットエフェクト
        tex_pos.x = TEX_P_SIZE * 1.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 0.0f / TEX_SIZE;
    }
    else if (gin[0].param.texture_no == EFFECT_SLASH)
    { // ヒットエフェクト
        tex_pos.x = TEX_P_SIZE * 0.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 1.0f / TEX_SIZE;
    }
    float tex_psize = TEX_P_SIZE / TEX_SIZE;
    
	//  左上の点の位置(射影座標系)・UV・色を計算して出力
    PsIn pout = (PsIn) 0;
    pout.color = gin[0].color;
    pout.position = mul(pos_left_top, projection_matrix);
    pout.texcoord = tex_pos; //   テクスチャ左上
    output.Append(pout);
	//  右上の点の位置(射影座標系) とテクスチャ座標の計算をして出力
    pout.color = gin[0].color;
    pout.position = mul(pos_right_top, projection_matrix);
    pout.texcoord = tex_pos + float2(tex_psize, 0); //  テクスチャ
    output.Append(pout);

	//  左下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
    pout.color = gin[0].color;
    pout.position = mul(pos_left_bottom, projection_matrix);
    pout.texcoord = tex_pos + float2(0, tex_psize); //   テクスチャ
    output.Append(pout);

	//  右下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
    pout.color = gin[0].color;
    pout.position = mul(pos_right_bottom, projection_matrix);
    pout.texcoord = tex_pos + float2(tex_psize, tex_psize); //  テクスチャ
    output.Append(pout);

    output.RestartStrip();
}
