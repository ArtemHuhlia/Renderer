struct VertexPosTexCoordIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexPosTexCoordOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D tex0 : register(t0);
SamplerState texSampler : register(s0);

cbuffer BlurParam
{
    int Direction;
    float Padding[3];
};

// vertex shader
VertexPosTexCoordOut VSMain(VertexPosTexCoordIn v)
{
    VertexPosTexCoordOut output;
    output.position = v.position;
    output.tex = v.tex;

    return output;
}


float4 PSMain(VertexPosTexCoordOut p) : SV_TARGET
{
	float weight[5] = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};
	//float weight[5] = {1.0, 0.9, 0.55, 0.18, 0.1};
	float2 TexCoords = p.tex;
    float2 TextureSize;
    tex0.GetDimensions(TextureSize.x, TextureSize.y);
    float2 tex_offset = 1.0 / TextureSize; // gets size of single texel
    float3 result = tex0.Sample(texSampler, TexCoords).rgb * weight[0];
    if(Direction)
    {
        for(int i = 1; i < 5; ++i)
        {
           result += tex0.Sample(texSampler, TexCoords + float2(tex_offset.x * i, 0.0)).rgb * weight[i];
           result += tex0.Sample(texSampler, TexCoords - float2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += tex0.Sample(texSampler, TexCoords + float2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += tex0.Sample(texSampler, TexCoords - float2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    return float4(result, 1.0);
}