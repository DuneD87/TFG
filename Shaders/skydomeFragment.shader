#version 410 core
uniform vec3 cameraPosition;
uniform vec3 planetPosition;
uniform vec3 sunDir;
uniform vec3 C_R;

uniform float K_R;
uniform float K_M;
uniform float E;
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

float miePhase( float g, float c, float cc ) {
    float gg = g * g;

    float a = ( 1.0 - gg ) * ( 1.0 + cc );

    float b = 1.0 + gg - 2.0 * g * c;
    b *= sqrt( b );
    b *= 2.0 + gg;

    return ( 3.0 / 8.0 / PI ) * a / b;
}

float rayleighPhase( float cc ) {
    return 3/2 * ( 1.0 + cc );
}

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
    float len = (e.y - e.x) / fNumInScatter;
    vec3 step = dir * len;
    vec3 p = o + dir * e.x;
    vec3 v = p + dir * (len * 0.5);

    vec3 sum = vec3(0.0);
    for(int i = 0; i < fNumInScatter; i++) {
        vec2 f = rayIntersection(v, l, atmosRadius);
        vec3 u = v + l * f.y;
        float n = (optic(p, v) + optic(v, u))*(PI * 4.0);
        sum += density(v)* exp(-n * ( K_R * C_R + K_M ));
        v += step;
    }

    sum *= len * SCALE_L;
    float c = dot(dir, l);
    float cc = c * c;
    return sum * ( K_R * C_R * rayleighPhase( cc ) + K_M * miePhase( G_M, c, cc ) ) * E;
}

void main (void)
{
    vec3 eye = cameraPosition - planetPosition;
    vec3 dir = normalize(cameraPosition - (FragPos.xyz));
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