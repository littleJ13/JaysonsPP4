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
float4 main(float4 pos : SV_POSITION, float4 uv : UV, float4 normal : NORMAL) : SV_TARGET
{
	float4 Mask = txt2D.Sample(SS, uv);
	if (Mask.a < 0.5)
	{
		discard;
	}

	float4 SurfaceNormal = normal;
	float4 LightColor = 0;
	float4 LightDirection = 0;

	LightColor.r = 1;
	LightColor.g = 1;
	LightColor.b = 1;
	LightColor.a = 20;
	
	LightDirection.x = 0;
	LightDirection.y = 0;
	LightDirection.z = 10;

	float4 LightRatio = saturate(dot(-LightDirection, SurfaceNormal));
	return LightRatio * LightColor * txt2D.Sample(SS, uv);
}

