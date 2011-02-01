//--------------------------------------------------------------------------------------
// File: DiffuseOnly.fx
//
// Jonny Rivers
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );

SamplerState samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix ModelToWorld;
	matrix WorldToProjection;
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 SpecularColor;
	float4 AmbientLightColor;
	float PhongExponent;
	float3 MainLightDir;
	float4 MainLightColor;
	float3 EyeDir;
	float pad0;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
	float2 DiffuseUV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Position : POSITION, float4 Normal : NORMAL, float2 DiffuseUV : TEXCOORD0 )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul( Position, ModelToWorld );
    output.Position = mul( output.Position, WorldToProjection );
    output.Normal = mul( Normal, ModelToWorld );
	output.DiffuseUV = DiffuseUV;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    float4 finalColor = 0;

	// Ambient lighting
	float4 diffuseTextureSample = txDiffuse.Sample( samLinear, input.DiffuseUV );
    finalColor += saturate( diffuseTextureSample * AmbientLightColor );

	// Diffuse lighting
	float minusLightNormalDotProduct = -dot( MainLightDir, input.Normal);
    finalColor += saturate( minusLightNormalDotProduct * MainLightColor * diffuseTextureSample );

	// Specular lighting
	if( minusLightNormalDotProduct > 0 )
	{
		float3 mainLightReflection = reflect(MainLightDir, input.Normal);
		float minusEyeLightDotProduct = -dot( EyeDir, mainLightReflection );
		float specularFactor = pow( minusEyeLightDotProduct, PhongExponent );
		finalColor += saturate( specularFactor * MainLightColor * SpecularColor );
	}
	
	finalColor.a = 1;

    return finalColor;
}
