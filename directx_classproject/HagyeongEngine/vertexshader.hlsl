/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
cbuffer mybuffer : register(b0)
{
	float4x4 mat;
};

struct VS_INPUT 
{
	float3 inPos : POSITION;
	float3 inColor : COLOR;
	float3 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float3 outColor : COLOR;
	float3 outTexCoord : TEXCORD;

};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.outPosition = mul(float4(input.inPos, 1.0f), mat);
	output.outColor = input.inColor;
	output.outTexCoord = input.inTexCoord;

	return output;
}