
struct EmitterDatas
{
	float3 pos;
	float temp1;
	float StartSpeed;//最初のスピード
	float3 StartVector;//方向
	float StartLifeTime;//寿命
	float3 StartSize3D;//最初の大きさ
	float3 StartRotation3D;//最初の回転
	float temp2;
	float4 StartColor;//開始時の色
	float GravityModifier;//重力
	//float Time;//全て出すまでの時間
	//int Count;//1フレームに出す量
	//int Cycles;//
	//float Interval;//インターバル
	//float Probabillity;//
	float temp5;//
	float randomSpeed;//ランダムなスピードを追加
	float randomLife;//ランダムな生存時間を追加
	float3 randomSize3D;//ランダムなサイズを追加
	float temp3;
	float3 randomRotate3D;//ランダムな回転を追加
	float temp4;
	float4 randomColor;//ランダムなカラーを追加
	float angle;
	float3 Axis;
};

struct ParticleUpdateDatas
{
	float lifeTime;//寿命
	float3 pos;//位置
	float3 vec;//ベクトル
	float temp1;//メモリ合わせ
	float4 col;//色
	float3 size;//サイズ
	float speed;//スピード
	float3 rotate;//回転
	float gravity;//重力
	float3 axis;//軸
	bool active;//生きているかどうか
};

struct ParticleDrawDatas
{
	float4 svpos;
	float4 color;
	float3 size;
	float3 rotate;
};


struct VSOutput
{
	float4 svpos: SV_POSITION;//システム用頂点座標
	float4 color : COLOR;//カラー
	float3 size : TEXCOORD00;//サイズ
	float3 rotate : TEXCOORD01;//回転
};

struct GSOutput
{
	float4 pos: SV_POSITION;//システム用頂点座標
	float4 color : COLOR;//カラー
	float2 uv : TEXCOORD00;//uv値
};

cbuffer cbuff0:register(b0)
{
	matrix mat;//3D変換行列
	matrix matBillboard;//ビルボード行列
};

float rand(float2 seed)
{
	//fract(sin(dot(float2(-x,x), float2(12.9898, 78.233))) * 43758.5453);
	return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}

float3 rand3(float2 seed)
{
	return 2.0 * (float3(rand(seed * 1), rand(seed * 2), rand(seed * 3)) - 0.5);
}