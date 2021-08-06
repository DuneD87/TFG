#version 410 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    int hasTexture;
    vec3 mAmbient;
    vec3 mDiffuse;
    vec3 mSpecular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;


struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 128
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int nPointLights;
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
uniform SpotLight spotLight;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 LocalPos;
in mat3 tbn;
in vec3 _viewPos;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
uniform sampler2D texture_diffuse[8];

uniform sampler2D texture_normal1;
uniform int isTerrain;
uniform float hPoint;
uniform float lPoint;

uniform vec3 viewPos;
uniform vec3 upVector;
uniform float pRadius;
uniform Material material;
uniform sampler2D shadowMap;

vec3 saturate(vec3 a)
{
    return clamp(a,0.0,1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(spotLight.position - TangentFragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    if(theta > light.outerCutOff)
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

        // diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        diffuse  *= intensity;
        specular *= intensity;
        vec3 result = ambient + diffuse + specular;
        return result;
    } else
    {
        return vec3(light.ambient * vec3(texture(material.diffuse, TexCoords)));
    }
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec3 lightPos, vec4 text)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 mDiffuse;
    vec3 mSpecular;
    if (material.hasTexture == 1)
    {
        if (isTerrain == 0)
        {
            mDiffuse = vec3(texture(material.diffuse,TexCoords));
            mSpecular = vec3(texture(material.specular,TexCoords));
        } else
        {
            mDiffuse = vec3(text);
            //mSpecular = vec3(text);
        }

    } else
    {
        mDiffuse = material.mDiffuse;
        mSpecular = material.mSpecular;
    }
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;
    vec3 specular = light.specular * spec * mSpecular;

    float shadow = ShadowCalculation(FragPosLightSpace,lightDir);
    return (ambient + (1-shadow)*(diffuse + specular));
}

vec3 _ACESFilmicToneMapping(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return saturate((x*(a*x+b))/(x*(c*x+d)+e));
}


vec3 CalcTerrainLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec3 lightPos,vec4 text)
{

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 mDiffuse;
    vec3 mSpecular;
    if (material.hasTexture == 1)
    {
        mDiffuse = vec3(text);

    } else
    {
        mDiffuse = material.mDiffuse;
        mSpecular = material.mSpecular;
    }
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;
    vec3 specular = light.specular * spec;

    float shadow = ShadowCalculation(FragPosLightSpace,lightDir);
    return (ambient + (1-shadow)*(diffuse));
}

vec4 createTerrainTexture()
{

    float heights[8] = float[8](0.125,0.250,0.375,0.5,0.625,0.750,0.875,1);

    vec4 terrainColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 dir = upVector-FragPos;
    float height = length(dir) - pRadius;
    float regionMin = 0.0;
    float regionMax = 0.0;
    float regionRange = 0.0;
    float regionWeight = 0.0;
    int nTextures = 8;
    for (int i = 0; i < nTextures; i++)
    {
        if (i == 0)
        {
            regionMin = lPoint;
            regionMax = lPoint + abs(heights[i]*hPoint);
        }
        else if (i < nTextures - 1)
        {
            regionMin = lPoint + abs(heights[i-1]*hPoint);
            regionMax = lPoint + abs(heights[i]*hPoint);
        }
        else if (i == nTextures - 1)
        {
            regionMin = lPoint + abs(heights[i-1]*hPoint);
            regionMax = lPoint;
        }

        regionRange = regionMax - regionMin;
        regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
        regionWeight = max(0.0, regionWeight);
        terrainColor += regionWeight * texture(texture_diffuse[i], TexCoords);
    }
   /* // Terrain region 1.
    regionMin = lPoint;
    regionMax = lPoint + abs(0.5*hPoint);
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture(texture_diffuse3, TexCoords);

    // Terrain region 2.
    regionMin = lPoint + abs(0.4*hPoint);;
    regionMax = lPoint + abs(0.7*hPoint);
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture(texture_diffuse1, TexCoords);

    // Terrain region 2.
    regionMin = lPoint + abs(0.6*hPoint);
    regionMax = lPoint + abs(0.8*hPoint);
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture(texture_diffuse2, TexCoords);

    // Terrain region 3.
    regionMin = lPoint + abs(0.6*hPoint);
    regionMax = hPoint + abs(0.6*hPoint);
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture(texture_diffuse4, TexCoords) ;

    // Terrain region 3.
    regionMin = lPoint + abs(0.7*hPoint);
    regionMax = hPoint;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture(texture_diffuse5, TexCoords) ;*/

    return terrainColor;
}

void main()
{
    // properties
    // obtain normal from normal map in range [0,1]
    vec3 norm = normalize(Normal);
    // transform normal vector to range [-1,1]
    //norm = normalize(norm * 2.0 - 1.0);  // this normal is in tangent space

    vec3 viewDir = normalize(viewPos - FragPos);
    // phase 1: Directional lighting
    vec4 text;
    if (isTerrain == 1)
    {
        text = createTerrainTexture();

    }
    vec3 result;
    // phase 2: Point lights
    for(int i = 0; i < nPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, TangentFragPos, viewDir);
    // phase 3: Spot light
    result += CalcDirLight(dirLight, norm, viewDir, FragPosLightSpace,TangentLightPos,text);
    result += CalcSpotLight(spotLight, norm, TangentFragPos, viewDir);
    FragColor = vec4(_ACESFilmicToneMapping(result), 1.0);

}