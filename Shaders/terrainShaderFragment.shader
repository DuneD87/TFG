#version 410 core

#define maxText 8
#define maxBiomes 6

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct TextHeight {
    int index;//4
    float hStart;//4
    float hEnd;//4
};

struct Biome {
    float latStart;//4  0
    float latEnd;//4    4
    int nTextBiome;//4  8
    TextHeight[maxText] textIndex;//12*16=192   200
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 LocalPos;
in vec4 worldPos;

uniform int nTextures;
uniform int nBiomes;
uniform DirLight dirLight;
uniform sampler2D texture_diffuse[maxText];
uniform Biome biomes[maxBiomes];
uniform float pRadius;
uniform float depthBlend;
uniform float hPoint;
uniform float lPoint;
uniform vec3 pPosition;

out vec4 FragColor;

const float PI = 3.14159265359;

vec3 saturate(vec3 a)
{
    return clamp(a,0.0,1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 text)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 mDiffuse;
    mDiffuse = vec3(text);
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;

    return (ambient + diffuse);
}

vec3 _ACESFilmicToneMapping(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return saturate((x*(a*x+b))/(x*(c*x+d)+e));
}
vec3 blend(vec4 texture1, float a1, vec4 texture2, float a2)
{
    float ma = max(texture1.a + a1, texture2.a + a2) - depthBlend;
    float b1 = max(texture1.a + a1 - ma, 0);
    float b2 = max(texture2.a + a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}
vec4 hash4( vec2 p )
{
    return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)),
                    2.0+dot(p,vec2(11.0,47.0)),
                    3.0+dot(p,vec2(41.0,29.0)),
                    4.0+dot(p,vec2(23.0,31.0))))*103.0);
}
vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    ivec2 iuv = ivec2( floor( uv ) );
    vec2 fuv = fract( uv );
    vec4 ofa = hash4( iuv + ivec2(0,0) );
    vec4 ofb = hash4( iuv + ivec2(1,0) );
    vec4 ofc = hash4( iuv + ivec2(0,1) );
    vec4 ofd = hash4( iuv + ivec2(1,1) );
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // transform per-tile uvs
    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );

    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;

    // fetch and blend
    vec2 b = smoothstep( 0.25,0.75, fuv );

    return mix( mix( textureGrad( samp, uva, ddxa, ddya ),
    textureGrad( samp, uvb, ddxb, ddyb ), b.x ),
    mix( textureGrad( samp, uvc, ddxc, ddyc ),
    textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}

vec4 createTerrainTextHeight(vec4 loadedTextures[maxText],Biome biome, float height)
{
    vec4 terrainColor = vec4(0.0, 0.0, 0.0, 1.0);
    int nText = biome.nTextBiome;
    for (int i = 0; i < nText; i++)
    {
        float a = smoothstep(biome.textIndex[i].hStart,biome.textIndex[i].hEnd,height);
        if (height >= biome.textIndex[i].hStart && height <  biome.textIndex[i].hEnd)
        {
            if (i == 0)
                terrainColor += vec4(blend(loadedTextures[biome.textIndex[0].index],1-a,
                loadedTextures[biome.textIndex[0].index],a),0.0);
            else
                terrainColor += vec4(blend(loadedTextures[biome.textIndex[i - 1].index],1-a,
                loadedTextures[biome.textIndex[i].index],a),0.0);
        }

    }
    return terrainColor;
}

vec4 biomeInterpolation(vec4 loadedTextures[maxText], int index, float lat, float height)
{
    vec4 result = vec4(0.0);
    if (lat <= biomes[index].latStart && lat > biomes[index].latEnd)
    {
        float a = smoothstep(biomes[index].latStart, biomes[index].latEnd,lat);
        if (index == 0)
            result += createTerrainTextHeight(loadedTextures,biomes[0], height);
        else
            result += vec4(blend(createTerrainTextHeight(loadedTextures,biomes[index - 1], height), 1-a,
            createTerrainTextHeight(loadedTextures,biomes[index], height), a), 0.0);
    }
    return result;
}

void main()
{
    vec3 norm = Normal;
    vec4 text = vec4(0,0,0,1);
    vec4 loadedTextures[maxText];
    for (int i = 0; i < nTextures; i++)
    {
        loadedTextures[i] = textureNoTile(texture_diffuse[i],TexCoords);
    }

    vec3 dir = pPosition-FragPos;
    float absHeigth = (abs(lPoint) + abs(hPoint));
    float height = abs((length(dir) - pRadius) - lPoint) / absHeigth;

    vec3 pos = (dir)/pRadius;
    float lat = atan(pos.y,sqrt(pos.x*pos.x+pos.z*pos.z)) + PI/2.0;//convert to positive 0..3.14
    lat = lat / PI;

    for (int i = 0; i < nBiomes; i++)
    {
        text += biomeInterpolation(loadedTextures,i,lat,height);
    }

    vec3 result = vec3(0.0);
    const float C = 10.0;
    const float far = 10000000.0;
    const float offset = 1.0;
    gl_FragDepth = (log(C * worldPos.z + offset) / log(C * far + offset));
    result += CalcDirLight(dirLight, norm,text);
    FragColor = vec4(_ACESFilmicToneMapping(result), 1.0);
}