//--------------------------------------------------------------------------------------
// File: Blit.fx
//
// Jonny Rivers
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );

SamplerState samLinear : register( s0 );

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
	float2 DiffuseUV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Position : POSITION, float2 DiffuseUV : TEXCOORD0 )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = Position;
	output.DiffuseUV = DiffuseUV;

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return txDiffuse.Sample( samLinear, input.DiffuseUV );
}
