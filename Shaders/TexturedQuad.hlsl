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

Texture2D Scene : register(t0);
Texture2D LensFlare : register(t1);

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
    //float4 SceneColor = Scene.Sample(texSampler, p.tex);
    float4 LensFlareColor = LensFlare.Sample(texSampler, p.tex);
    return float4(LensFlareColor.rgb, 0.1);
}