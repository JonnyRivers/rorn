//--------------------------------------------------------------------------------------
// File: Untextured.fx
//
// Jonny Rivers
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
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
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Position : POSITION, float4 Normal : NORMAL )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul( Position, ModelToWorld );
    output.Position = mul( output.Position, WorldToProjection );
	Normal.w = 0;
    output.Normal = mul( Normal, ModelToWorld );
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    float4 finalColor = 0;

	// Ambient lighting
    finalColor += saturate( AmbientColor * AmbientLightColor );

	// Diffuse lighting
	float minusLightNormalDotProduct = -dot( MainLightDir, input.Normal);
    finalColor += saturate( minusLightNormalDotProduct * MainLightColor * DiffuseColor );

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
