#include "ShaderStructs.hlsli"
#include "Lighting.hlsli"

cbuffer ExternalData : register(b0) {
	float4 colorTint;
	float3 cameraPosition;
	float roughness;
	float3 ambient;
	Light directionalLight1;
	Light redLight;
	Light greenLight;
	Light bluePoint;
	Light yellowPoint;
}

// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float3 view = normalize(cameraPosition - input.worldPosition);

	float4 first = Directional(directionalLight1, view, input.normal, roughness, colorTint);
	float4 red = Directional(redLight, view, input.normal, roughness, colorTint);
	float4 green = Directional(greenLight, view, input.normal, roughness, colorTint);
	float4 blue = Point(bluePoint, view, input.normal, roughness, colorTint, input.worldPosition);
	float4 yellow = Point(yellowPoint, view, input.normal, roughness, colorTint, input.worldPosition);

	return first + red + green + blue + yellow + float4(ambient, 1) * colorTint;
}