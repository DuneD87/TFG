#version 330 core


uniform float time;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;
uniform vec3 planetPosition;
uniform mat4 test;
uniform vec3 sunDir;

uniform float K_R;
uniform float K_M;
uniform float E;
uniform vec3 C_R;
uniform float G_M;
uniform float MAX;
uniform float H;
uniform float L;
uniform float planetRadius;
uniform float atmosRadius;

uniform float fNumOutScatter;
uniform float fNumInScatter;

in vec4 worldPos;
in vec4 FragPos;
in vec3 Normal;


out vec4 FragColor;

const float PI = 3.14159265359;
const float degToRad = PI / 180.0;

const float DEG_TO_RAD = PI / 180.0;



float SCALE_H = H / (atmosRadius - planetRadius);
float SCALE_L = L / (atmosRadius - planetRadius);




vec2 rayIntersection(vec3 p, vec3 dir, float radius ) {
    float b = dot( p, dir );
    float c = dot( p, p ) - (radius * radius);

    float d = b * b - c;
    if ( d < 0.0 ) {
        return vec2( MAX, -MAX );
    }
    d = sqrt( d );

    float near = -b - d;
    float far = -b + d;

    return vec2(near, far);
}

// Mie
// g : ( -0.75, -0.999 )
//      3 * ( 1 - g^2 )               1 + c^2
// F = ----------------- * -------------------------------
//      2 * ( 2 + g^2 )     ( 1 + g^2 - 2 * g * c )^(3/2)
float miePhase( float g, float c, float cc ) {
    float gg = g * g;

    float a = ( 1.0 - gg ) * ( 1.0 + cc );

    float b = 1.0 + gg - 2.0 * g * c;
    b *= sqrt( b );
    b *= 2.0 + gg;

    return ( 3.0 / 8.0 / PI ) * a / b;
}

// Reyleigh
// g : 0
// F = 3/4 * ( 1 + c^2 )
float rayleighPhase( float cc ) {
    return 0.75 * ( 1.0 + cc );
}

//exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
float density(vec3 p) {
    return exp(-(length(p) - planetRadius) * SCALE_H);
}

float optic(vec3 p, vec3 q) {
    vec3 step = (q - p) / fNumOutScatter;
    vec3 v = p + step * 0.5;

    float sum = 0.0;
    for(int i = 0; i < fNumOutScatter; i++) {
        sum += density(v);
        v += step;
    }
    sum *= length(step)*SCALE_L;
    return sum;
}

vec3 inScatter(vec3 o, vec3 dir, vec2 e, vec3 l) {
    //fSampleLength = fFar / fSamples;
    float len = (e.y - e.x) / fNumInScatter;

    //v3SampleRay = v3Ray * fSampleLength;
    vec3 step = dir * len;
    //v3Start = v3CameraPos + v3Ray * fNear;
    vec3 p = o + dir * e.x;

    //fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
    //v3SamplePoint = v3Start + v3SampleRay * 0.5;
    vec3 v = p + dir * (len * 0.5);

    vec3 sum = vec3(0.0);
    for(int i = 0; i < fNumInScatter; i++) {

        vec2 f = rayIntersection(v, l, atmosRadius);

        //fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
        vec3 u = v + l * f.y;

        //fScatter = scale() * scale()
        float n = (optic(p, v) + optic(v, u))*(PI * 4.0);

        //density() = //exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        //v3Attenuate = * exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
        sum += density(v)* exp(-n * ( K_R * C_R + K_M ));
        //v3SamplePoint += v3SampleRay;
        v += step;
    }
    sum *= len * SCALE_L;
    float c = dot(dir, -l);
    float cc = c * c;
    return sum * ( K_R * C_R * rayleighPhase( cc ) + K_M * miePhase( G_M, c, cc ) ) * E;
}

vec3 applyFog( in vec3  rgb,      // original color of the pixel
in float distance, // camera to point distance
in vec3  rayOri,   // camera position
in vec3  rayDir )  // camera to point vector
{
    float c = 0.02;
    float b = 0.01;
    float fogAmount = c*exp(-rayOri.y*b)*(1.0-exp(-distance*rayDir.y*b))/rayDir.y;
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix( rgb, fogColor, fogAmount );
}

void main (void)
{
    vec3 dir = normalize(cameraPosition - (FragPos.xyz));
    vec3 eye = cameraPosition - planetPosition;

    vec3 l = normalize(sunDir);
    vec2 e = rayIntersection(eye, dir, atmosRadius);
    vec2 f = rayIntersection(eye, dir, planetRadius);

    e.y = min(e.y, f.x);

    vec3 I = inScatter(eye, dir, e, -l);

    const float C = 10.0;
    const float far = 10000000.0;
    const float offset = 1.0;
    gl_FragDepth = (log(C * worldPos.z + offset) / log(C * far + offset));
    FragColor = vec4(I, 1.0);
}