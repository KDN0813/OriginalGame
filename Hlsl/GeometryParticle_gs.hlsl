#include "GeometryParticle.hlsli"
#include "./ParticleDisp.h"

Texture2D color_map : register(t0);

// 1���_����4���_��������
[maxvertexcount(4)]
void main(point GsIn gin[1], inout TriangleStream<PsIn> output)
{
    float texture_width = 0.0f;
    float texture_height = 0.0f;
    
    // �e�N�X�`���T�C�Y�擾
    color_map.GetDimensions(texture_width, texture_height);
    
    // �r���{�[�h�����邽�߂ɁA
    // ���_���W�����[���h��ԁ��r���[��Ԃ֕ϊ�
    float4 pos = mul(float4(gin[0].position, 1.0f), view_matrix);

	// �_��ʂɂ��邽��4���_�̍��W�𐶐�
    float rot = gin[0].param.rot;
    float s = sin(rot);
    float c = cos(rot);
    float4 right = float4(c, -s, 0, 0) * (texture_width * 0.5 * gin[0].param.scale.x);
    float4 up = float4(s, c, 0, 0) * (texture_height * 0.5 * gin[0].param.scale.y);

    //  4�p�`�|���S���𐶐�
    float4 pos_left_top = pos - right + up;
    float4 pos_left_bottom = pos - right - up;
    float4 pos_right_top = pos + right + up;
    float4 pos_right_bottom = pos + right - up;

	//  UV���W�̐؂���ʒu��type����Z�o
    float w = 1.0;
    float h = 1.0;
    
    float2 tex_pos;
    if (gin[0].param.texture_no == EFFECT_HIT)
    { // �a��
        tex_pos.x = TEX_P_SIZE * 0.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 0.0f / TEX_SIZE;
    }
    else if (gin[0].param.texture_no == EFFECT_HIT2)
    { // �q�b�g�G�t�F�N�g
        tex_pos.x = TEX_P_SIZE * 1.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 0.0f / TEX_SIZE;
    }
    else if (gin[0].param.texture_no == EFFECT_SLASH)
    { // �q�b�g�G�t�F�N�g
        tex_pos.x = TEX_P_SIZE * 0.0f / TEX_SIZE;
        tex_pos.y = TEX_P_SIZE * 1.0f / TEX_SIZE;
    }
    float tex_psize = TEX_P_SIZE / TEX_SIZE;
    
	//  ����̓_�̈ʒu(�ˉe���W�n)�EUV�E�F���v�Z���ďo��
    PsIn pout = (PsIn) 0;
    pout.color = gin[0].color;
    pout.position = mul(pos_left_top, projection_matrix);
    pout.texcoord = tex_pos; //   �e�N�X�`������
    output.Append(pout);
	//  �E��̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_right_top, projection_matrix);
    pout.texcoord = tex_pos + float2(tex_psize, 0); //  �e�N�X�`��
    output.Append(pout);

	//  �����̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_left_bottom, projection_matrix);
    pout.texcoord = tex_pos + float2(0, tex_psize); //   �e�N�X�`��
    output.Append(pout);

	//  �E���̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_right_bottom, projection_matrix);
    pout.texcoord = tex_pos + float2(tex_psize, tex_psize); //  �e�N�X�`��
    output.Append(pout);

    output.RestartStrip();
}
