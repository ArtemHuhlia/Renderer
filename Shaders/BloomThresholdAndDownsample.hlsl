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
    float4 ResultColor = float4(1,1,1,1);
    float3 TexColor = tex0.Sample(texSampler,p.tex).xyz;
    float Brightness = dot(TexColor, float3(0.2126, 0.7152, 0.0722));
    if(Brightness > 0.7)
        ResultColor = float4(TexColor, 1);
    else
        ResultColor = float4(0,0,0,1);

    return ResultColor;
}