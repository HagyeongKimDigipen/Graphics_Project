/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
cbuffer alphaBuffer : register(b0)
{
	float alpha;
}
struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float3 inColor : COLOR;
	float2 inTexCoord : TEXCOORD;

};

Texture2D objTexture : TEXTURE: register(t0);
SamplerState objSamplerState : SAMPLER: register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	return float4(pixelColor, alpha);
}