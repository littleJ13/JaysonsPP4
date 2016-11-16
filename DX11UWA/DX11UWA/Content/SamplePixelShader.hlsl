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

	//Added Directional Light
	float4 SurfaceNormal = normal;
	float4 SurfacePos = pos;
	float4 LightColor = 0;
	float4 LightDirection = 0;
	float4 LightPos = 0;
	float4 ConeDir = 0;
	float ConeRatio = .9999;

	LightColor.r = 5;
	LightColor.g = 1;
	LightColor.b = 1;
	LightColor.a = 20;
	
	LightDirection.x = 0;
	LightDirection.y = 0;
	LightDirection.z = -10;


	float4 LightRatio = saturate(dot(-LightDirection, SurfaceNormal));
	float4 Result = LightRatio * LightColor * txt2D.Sample(SS, uv);

	//Added Point Light
	LightColor.r = 1;
	LightColor.g = 1;
	LightColor.b = 5;
	LightColor.a = 20;

	LightPos.x = -15;
	LightPos.y = 0;
	LightPos.z = 10;

	float4 LightDir = normalize(LightPos - SurfacePos);
	LightRatio = saturate(dot(LightDirection, SurfaceNormal));
	float4 Result2 = LightRatio * LightColor * txt2D.Sample(SS, uv);

	//Added Spot Light
	LightColor.r = 1;
	LightColor.g = 10;
	LightColor.b = 1;
	LightColor.a = 20;

	LightPos.x = 0;
	LightPos.y = 0;
	LightPos.z = 0;

	ConeDir.x = 1.1;
	ConeDir.y = .1;
	ConeDir.z = 0;

	LightDirection.x = 0;
	LightDirection.y = 0;
	LightDirection.z = 0;

	//LightDir = normalize(LightPos - SurfacePos);
	//float4 SurfaceRatio = saturate(dot(-LightDir, ConeDir));
	//float4 SpotFactor = (SurfaceRatio > ConeRatio) ? 1 : 0;
	//float4 Result3 = SpotFactor * LightRatio * LightColor * txt2D.Sample(SS, uv);

	return Result + Result2;
	//return Result + Result2 + Result3;
}

