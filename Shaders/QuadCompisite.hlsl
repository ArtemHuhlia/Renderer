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
Texture2D tex1 : register(t1);

cbuffer CompositeParams: register(b0)
{
    float coefficient;
}

SamplerState texSampler : register(s0);

// vertex shader
VertexPosTexCoordOut VSMain(VertexPosTexCoordIn v)
{
    VertexPosTexCoordOut output;
    output.position = v.position;
    output.tex = v.tex;

    return output;
}

// pixel shader
float4 PSMain(VertexPosTexCoordOut p) : SV_TARGET
{
    const float gamma = 1.8;
    float3 SceneColor = tex0.Sample(texSampler, p.tex).rgb;
    float3 BloomColor = tex1.Sample(texSampler, p.tex).rgb;
    SceneColor += BloomColor;

    float3 Result = float3(1.f,1.f,1.f) - exp(-SceneColor * 0.6);
    Result = pow(Result, float3(1.f/gamma,1.f/gamma, 1.f/gamma));
    return float4(Result, 1.f);
    // output: tex0 + coefficient * tex1
    //return mad(coefficient, tex1.Sample(texSampler, p.tex), tex0.Sample(texSampler, p.tex));
}