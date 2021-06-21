////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D ShaderAlbedoTexture;
SamplerState SampleAlbedoType;

cbuffer LightBuffer
{
    float4 LightColor;
    float3 LightPosition;
    float3 ViewPosition;
	float2 Padding;
};
//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 world_position : POSITION;
};






////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
    //HACK: DO IT ON CPU
    const float2 VerticallyFlippedUV = float2(input.tex.x, 1 - input.tex.y);
    const float3 AlbedoColor = ShaderAlbedoTexture.Sample(SampleAlbedoType, VerticallyFlippedUV).rgb;
    const float3 Position = input.world_position;

    //Ambient
    const float AmbientFactor = 0.2f; 
    const float3 AmbientColor = AmbientFactor * AlbedoColor; 

    //Diffuse
    const float3 LightDir = normalize(LightPosition - Position);
    const float3 Normal = input.normal;
    const float DiffuseFactor = max(dot(LightDir, Normal), 0.0);
    const float3 DiffuseColor = DiffuseFactor * AlbedoColor;

    //Specular
    const float3 WhiteColor = float3(1.f, 1.f, 1.f);
    const float3 ViewDir = normalize(ViewPosition - Position);
    const float3 ReflectionDir = reflect(-LightDir, Normal);
    const float3 HalfwayDir = normalize(LightDir + ViewDir);
    const float SpecularFactor = pow(max(dot(Normal, HalfwayDir), 0.f), 32.0);
    const float3 SpecularColor = WhiteColor * SpecularFactor;

    const float4 FinalColor = float4(AmbientColor + DiffuseColor + SpecularColor, 1.f);
    return FinalColor;
}