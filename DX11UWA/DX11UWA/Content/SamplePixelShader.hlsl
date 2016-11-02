// Per-pixel color data passed through the pixel shader.

texture2D txt2D : register(t0);
SamplerState SS : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 uv : UV;
	float4 normal : NORMAL;
};

// A pass-through function for the (interpolated) color data.
float4 main(float4 pos : SV_POSITION, float4 uv : UV) : SV_TARGET
{
	float4 Mask = txt2D.Sample(SS, uv);
	if (Mask.a < 0.5)
	{
		discard;
	}

	return txt2D.Sample(SS, uv);
}
