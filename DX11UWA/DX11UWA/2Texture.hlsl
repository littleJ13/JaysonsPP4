// Per-pixel color data passed through the pixel shader.
texture2D txt2D : register(t0);
texture2D txt2D2 : register(t1);
SamplerState SS[2] : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
};

// A pass-through function for the (interpolated) color data.
float4 main(float4 pos : SV_POSITION, float4 uv : UV) : SV_TARGET
{
	float4 Result = (txt2D.Sample(SS[0], uv) * 0.5) + (txt2D2.Sample(SS[1], uv) * 0.5);
	return Result;
}