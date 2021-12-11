#include "ComputeShader.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_arry[vnum] =
{
	float4(-0.5,-0.5,0,0),//左下
	float4(-0.5,0.5,0,0), //左上
	float4(0.5,-0.5,0,0), //右下
	float4(0.5,0.5,0,0)	  //右上
};

static const float2 uv_array[vnum] =
{
	float2(0,1),//左下
	float2(0,0),//左上
	float2(1,1),//右下
	float2(1,0)//右上
};

[maxvertexcount(vnum)]//ジオメトリシェーダで出力する最大長点数
void GS(
	point VSOutput input[1],//入力データ群
	inout TriangleStream< GSOutput > output//出力オブジェクト(DataType)
)
{
	//授業
	GSOutput o;

	for (uint i = 0; i < vnum; ++i)
	{
		//中心からオフセットをビルボード回転(モデル座標)
		float4 offset;
		//中心からオフセットをスケーリング
		offset.x = offset_arry[i].x * input[0].size.x;
		offset.y = offset_arry[i].y * input[0].size.y;
		offset.z = 0;
		offset.w = 0;
		/*
		//回転用変数
		float x = offset.x;
		float y = offset.y;
		float z = offset.z;

		//z軸回転
		offset.x = x * cos(input[0].rotate.z) - y * sin(input[0].rotate.z);
		offset.y = x * sin(input[0].rotate.z) + y * cos(input[0].rotate.z);

		//y軸回転
		offset.x += x * cos(input[0].rotate.y) + z * sin(input[0].rotate.y);
		offset.z += -x * sin(input[0].rotate.y) + z * cos(input[0].rotate.y);

		//x軸回転
		offset.y += y * cos(input[0].rotate.x) - z * sin(input[0].rotate.x);
		offset.z += y * sin(input[0].rotate.x) + z * cos(input[0].rotate.x);
		*/
		offset = mul(matBillboard, offset);
		//オフセット分ずらす(ワールド座標)
		o.pos = input[0].svpos + offset;
		//ビュープロジェクション変換
		o.pos = mul(mat, o.pos);
		o.color = input[0].color;
		o.uv = uv_array[i];
		output.Append(o);
	}
}