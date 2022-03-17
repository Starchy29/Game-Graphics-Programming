#ifndef __GGP_LIGHTING_INCLUDES__ // Each .hlsli file needs a unique identifier! 
#define __GGP_LIGHTING_INCLUDES__ 

#define LIGHT_TYPE_DIRECTIONAL 0 
#define LIGHT_TYPE_POINT  1 
#define LIGHT_TYPE_SPOT  2
#define MAX_SPECULAR_EXPONENT 256.0f

struct Light {
	int type;
	float3 direction;
	float range;
	float3 position;
	float intensity;
	float3 color;
	float spotFallOff;
	float3 padding;
};

float Attenuate(Light light, float3 worldPos) {
	float dist = distance(light.position, worldPos);
	float att = saturate(1.0f - (dist * dist / (light.range * light.range)));
	return att * att;
}

float4 Diffuse(Light light, float3 normal)
{
	float3 directionToLight = -normalize(light.direction);
	float diffuse = saturate(dot(directionToLight, normal));
	return float4(light.color * diffuse, 1);
}

float4 Specular(Light light, float3 view, float3 normal, float roughness) {
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	if(specExponent < 0.05) {
		return float4(0, 0, 0, 0);
	}

	float3 reflection = reflect(light.direction, normal);
	float spec = pow(saturate(dot(reflection, view)), specExponent);
	return float4(light.color * spec, 1);
}

float4 Directional(Light light, float3 view, float3 normal, float roughness, float4 colorTint) {
	return Specular(light, view, normal, roughness) + Diffuse(light, normal) * colorTint;
}

float4 Point(Light light, float3 view, float3 normal, float roughness, float4 colorTint, float3 worldPosition) {
	light.direction = normalize(light.position - worldPosition);
	return Directional(light, view, normal, roughness, colorTint) * Attenuate(light, worldPosition);
}

#endif