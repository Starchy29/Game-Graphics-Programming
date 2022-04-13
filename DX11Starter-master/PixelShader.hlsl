#include "ShaderStructs.hlsli"
#include "Lighting.hlsli"

cbuffer ExternalData : register(b0) {
	float4 colorTint;
	float3 cameraPosition;
	float roughness;
	float uvScale;
	float3 ambient;
	Light directionalLight1;
	Light redLight;
	Light greenLight;
	Light bluePoint;
	Light yellowPoint;
}

Texture2D SurfaceTexture : register(t0); // "t" registers for textures
Texture2D NormalMap : register(t1);
SamplerState DefaultSampler : register(s0); // "s" registers for samplers

// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 unpackedNormal = NormalMap.Sample(DefaultSampler, input.uv).rgb * 2 - 1;

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	input.tangent = normalize(input.tangent - input.normal * dot(input.tangent, input.normal)); // Gram-Schmidt assumes T&N are normalized!
	float3 biTangent = cross(input.tangent, input.normal);
	float3x3 TBN = float3x3(input.tangent, biTangent, input.normal);
	input.normal = mul(unpackedNormal, TBN); // Note multiplication order!

	input.uv.x *= uvScale;
	input.uv.y *= uvScale;
	float3 view = normalize(cameraPosition - input.worldPosition);

	float4 surfaceColor = SurfaceTexture.Sample(DefaultSampler, input.uv).rgba * colorTint;

	float4 first = Directional(directionalLight1, view, input.normal, roughness, surfaceColor);
	float4 red = Directional(redLight, view, input.normal, roughness, surfaceColor);
	float4 green = Directional(greenLight, view, input.normal, roughness, surfaceColor);
	float4 blue = Point(bluePoint, view, input.normal, roughness, surfaceColor, input.worldPosition);
	float4 yellow = Point(yellowPoint, view, input.normal, roughness, surfaceColor, input.worldPosition);
	float4 totalColor = first + red + green + blue + yellow + float4(ambient, 1) * surfaceColor;

	return float4(pow(totalColor, 1.0f / 2.2f).rgb, 1);
}