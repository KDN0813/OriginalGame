#include "GeometryParticle.hlsli"


// 初期化用入力バッファ(構造化バッファ。読み込み専用)
StructuredBuffer<ParticleData> Input : register(t0);
// 出力バッファ(構造化バッファ。読み書き可能)
RWStructuredBuffer<ParticleData> Result : register(u0);

//スレッドの数
#define TH_X PERTICLES_COMP_NO
#define TH_Y 1
#define TH_Z 1

// 入力情報を出力バッファに追加する関数
[numthreads(TH_X, TH_Y, TH_Z)] // スレッド グループのスレッド数
void main(uint3 Gid : SV_GroupID, //グループID　ディスパッチ側で指定
	uint3 GTid : SV_GroupThreadID //スレッドID　ここで指定
)
{

};