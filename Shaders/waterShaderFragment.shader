#version 430 core
in vec2 TexCoord;
in vec4 worldPos;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse[3];

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;
uniform vec3 viewPos;
uniform float waveSpeed;
uniform float shininess;
out vec4 FragColor;
const float waveStrength = 0.2;

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

    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );

    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;

    vec2 b = smoothstep( 0.25,0.75, fuv );

    return mix( mix( textureGrad( samp, uva, ddxa, ddya ),
    textureGrad( samp, uvb, ddxb, ddyb ), b.x ),
    mix( textureGrad( samp, uvc, ddxc, ddyc ),
    textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 text)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 mDiffuse;
    vec3 mSpecular;
    mDiffuse = vec3(text);
    mSpecular = vec3(text);
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;
    vec3 specular = light.specular * spec * mSpecular;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    float dist = distance(FragPos,viewPos);

    vec2 ndc = ((worldPos.xy/worldPos.w)/2.0)+0.5;
    vec2 distorsion = (texture2D(texture_diffuse[0],vec2(TexCoord.x + waveSpeed,TexCoord.y)*5).rg * 2.0 - 1.0)*waveStrength;
    vec2 reflectCoord = vec2(ndc.x, 1.0 - ndc.y);
    vec2 diffCoord = TexCoord/2;

    reflectCoord += distorsion;
    diffCoord += distorsion;

    vec4 waterTextDiff = textureNoTile(texture_diffuse[1], diffCoord);
    vec4 waterTextRefl = texture2D(texture_diffuse[2], reflectCoord);
    float mixAmount = dist/5000;
    if (mixAmount > 1) mixAmount = 1;
    vec4 waterText = mix(waterTextDiff,waterTextRefl,1-mixAmount);
    vec3 result = vec3(0);
    result += CalcDirLight(dirLight, Normal, viewDir,waterText);
    float alphaAmount = dist/2000;
    const float C = 10.0;
    const float far = 10000000.0;
    const float offset = 1.0;
    gl_FragDepth = (log(C * worldPos.z + offset) / log(C * far + offset));
    FragColor = vec4(result,clamp(1.0,0.8,alphaAmount));
}