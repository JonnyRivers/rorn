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
    finalColor += saturate( max( -dot( MainLightDir, input.Normal), 0.0 ) * MainLightColor * DiffuseColor );

	// Specular lighting
	float3 mainLightReflection = reflect(MainLightDir, input.Normal);
	float specularFactor = pow( -dot( EyeDir, mainLightReflection ), PhongExponent );
	finalColor += saturate( max( specularFactor , 0.0 ) * MainLightColor * SpecularColor );
	
	finalColor.a = 1;

    return finalColor;
}
