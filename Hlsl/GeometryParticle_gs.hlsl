#include "GeometryParticle.hlsli"

// 1���_����4���_��������
[maxvertexcount(4)]
void main(point GsIn gin[1], inout TriangleStream<PsIn> output)
{
    // �r���{�[�h�����邽�߂ɁA
    // ���_���W�����[���h��ԁ��r���[��Ԃ֕ϊ�
    float4 pos = mul(float4(gin[0].position, 1.0f), view_matrix);

	// �_��ʂɂ��邽��4���_�̍��W�𐶐�
    float rot = gin[0].param.rot;
    float s = sin(rot);
    float c = cos(rot);
    float4 right = float4(c, -s, 0, 0) * (gin[0].size.x * 0.5 * gin[0].param.scale.x);
    float4 up = float4(s, c, 0, 0) * (gin[0].size.y * 0.5 * gin[0].param.scale.y);

    //  4�p�`�|���S���𐶐�
    float4 pos_left_top = pos - right + up;
    float4 pos_left_bottom = pos - right - up;
    float4 pos_right_top = pos + right + up;
    float4 pos_right_bottom = pos + right - up;

	//  UV���W�̐؂���ʒu��type����Z�o
    float w = 1.0;
    float h = 1.0;
    
	//  ����̓_�̈ʒu(�ˉe���W�n)�EUV�E�F���v�Z���ďo��
    PsIn pout = (PsIn) 0;
    pout.color = gin[0].color;
    pout.position = mul(pos_left_top, projection_matrix);
    pout.texcoord = float2(0, 0); //   �e�N�X�`������
    output.Append(pout);
	//  �E��̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_right_top, projection_matrix);
    pout.texcoord = float2(w, 0); //  �e�N�X�`��
    output.Append(pout);

	//  �����̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_left_bottom, projection_matrix);
    pout.texcoord = float2(0, h); //   �e�N�X�`��
    output.Append(pout);

	//  �E���̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
    pout.color = gin[0].color;
    pout.position = mul(pos_right_bottom, projection_matrix);
    pout.texcoord = float2(w, h); //  �e�N�X�`��
    output.Append(pout);

    output.RestartStrip();
}
