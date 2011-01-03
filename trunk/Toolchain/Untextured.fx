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
	float4 MainLightDir;
	float4 MainLightColor;
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
    finalColor += saturate( AmbientColor * AmbientLightColor );
    finalColor += saturate( dot( (float3)MainLightDir, -input.Normal) * MainLightColor * DiffuseColor );
    finalColor.a = 1;

    return finalColor;
}