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
	unsigned int NumActivePointLights;
	float4 PointLightPositions[16];
	float4 PointLightColors[16];
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 WorldPosition : POSITION;
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

	// main light - diffuse
	float minusLightNormalDotProduct = -dot( MainLightDir, input.Normal);
    finalColor += saturate( minusLightNormalDotProduct * MainLightColor * DiffuseColor );

	// main light - specular
	if( minusLightNormalDotProduct > 0 )
	{
		float3 mainLightReflection = reflect(MainLightDir, input.Normal);
		float minusEyeLightDotProduct = -dot( EyeDir, mainLightReflection );
		float specularFactor = pow( minusEyeLightDotProduct, PhongExponent );
		finalColor += saturate( specularFactor * MainLightColor * SpecularColor );
	}

	// point lights
	for(unsigned int pointLightIndex = 0 ; pointLightIndex < NumActivePointLights ; ++pointLightIndex)
	{
		// diffuse		
		float4 pointLightPosition = PointLightPositions[pointLightIndex];
		float4 pointLightColor = PointLightColors[pointLightIndex];
		float pointLightIntensity = pointLightPosition.w;
		pointLightPosition.w = 1;
		float distanceToPointLight = distance( pointLightPosition, input.WorldPosition );
		float lightIntensity = pointLightIntensity / (12.5663701 * distanceToPointLight * distanceToPointLight);
		float4 lightColor = lightIntensity * pointLightColor;
		float4 lightDirection = normalize(input.WorldPosition - pointLightPosition);
		float minusLightNormalDotProduct = -dot( lightDirection, input.Normal);
		finalColor += saturate( minusLightNormalDotProduct * lightColor * DiffuseColor );

		// specular
		if( minusLightNormalDotProduct > 0 )
		{
			float3 lightReflection = reflect(lightDirection, input.Normal);
			float minusEyeLightDotProduct = -dot( EyeDir, lightReflection );
			float specularFactor = pow( minusEyeLightDotProduct, PhongExponent );
			finalColor += saturate( specularFactor * lightColor * SpecularColor );
		}
	}
	
	finalColor.a = 1;

	return finalColor;
}
