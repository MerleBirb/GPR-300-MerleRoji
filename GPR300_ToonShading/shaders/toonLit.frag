#version 450                          

out vec4 FragColor;
in vec2 uv;

in struct Vertex
{
    vec3 WorldNormal;
    vec3 WorldPosition;
}v_out;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float radius;
    float intensity;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float minAngle;
    float maxAngle;
};

struct Material
{
    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;
    float shininess;
    vec3 objColor;
};

uniform vec3 _CameraPos;
uniform Material _Material;

#define MAX_DIR_LIGHTS 1
uniform int _NumDirLights = MAX_DIR_LIGHTS;
uniform DirectionalLight _DirLights[MAX_DIR_LIGHTS];

#define MAX_PNT_LIGHTS 2
uniform int _NumPntLights = MAX_PNT_LIGHTS;
uniform PointLight _PntLights[MAX_PNT_LIGHTS];

#define MAX_SPT_LIGHTS 1
uniform int _NumSptLights = MAX_SPT_LIGHTS;
uniform SpotLight _SptLights[MAX_SPT_LIGHTS];

uniform sampler2D _BrickTexture1;
uniform sampler2D _BrickTexture2;
uniform int _TexChoice;
uniform float _Time;
uniform bool _Animated;

uniform int _ToonLevels;
uniform bool _RimLightingOn;
uniform int _RimIntensity;

const float toonScaleFactor = 1.0 / _ToonLevels;

vec3 ambient(float coefficient, vec3 color)
{
    vec3 ambientLight;

    ambientLight = coefficient * color;

    return ambientLight;
}

vec3 diffuse(float difCoefficient, vec3 toLightDir, vec3 surfaceNormal, vec3 color)
{
    vec3 diffuseLight;
    vec3 stepLight;

    diffuseLight = difCoefficient * max(dot(toLightDir, surfaceNormal), 0) * color; // max range 0 - 1

    return diffuseLight;
}

vec3 specular(float specCoefficient, vec3 toLightDir, vec3 surfaceNormal, float shininess, vec3 color)
{
    vec3 specularLight;
    vec3 viewerDir = normalize(_CameraPos - v_out.WorldPosition);

    vec3 halfVector = normalize(viewerDir + toLightDir); // blinn phong

    specularLight = specCoefficient * pow(max(dot(surfaceNormal, halfVector), 0), shininess) * color;

    return specularLight;
}

float attenuation(float dist, float radius)
{
    return clamp(pow((radius / max(radius, dist)), 2), 0, 1);
}

float angularAttenuation(float theta, float minAngle, float maxAngle, float fallOffCurve)
{
    return pow(clamp((theta - maxAngle)/(minAngle - maxAngle), 0, 1), fallOffCurve);
}

float calculateRimLightFactor(vec3 viewerDir, vec3 normal) // calculates the power of rim lighting
{
    // based on: https://youtu.be/h15kTY3aWaY
    float rimFactor = dot(viewerDir, normal); // the result gets closer to 1 as the two vectors become aligned with each other
    rimFactor = 1.0 - rimFactor; // effect increases as the angle from viewerDir and normal vector grows
    rimFactor = max(0.0, rimFactor); // make sure not to get any negative values
    rimFactor = pow(rimFactor, _RimIntensity); // controls falloff of effect
    return rimFactor;
}

vec3 calculateDirLight(DirectionalLight light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 toLightDir = normalize(light.direction - v_out.WorldPosition);
    float diffuseFactor = dot(normal, toLightDir);
    vec3 viewerDir = normalize(_CameraPos - v_out.WorldPosition);

    vec3 ambientLight = vec3(0);
    vec3 diffuseLight = vec3(0);
    vec3 specularLight = vec3(0);
    vec3 rimLight = vec3(0);

    // toon shading starts here
    // based on: https://youtu.be/h15kTY3aWaY
    // keep track of intensity of light hitting the normal
    float intensity = dot(normalize(light.direction), normal);

    // factor step functions
    if (diffuseFactor > 0) // make sure steps never go negative
    {
        diffuseFactor = ceil(diffuseFactor * _ToonLevels) * toonScaleFactor;
        diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color) * diffuseFactor; // step function applied to diffuse light

        if (diffuseFactor >= 1) // if there is no shading, then allow specular light
        {
            if (intensity >= 0.99)
            {
                specularLight = light.color * _Material.specularCoefficient * _Material.shininess;
            }
        }
        
        ambientLight = ambient(_Material.ambientCoefficient, light.color); // continue ambient light and effect it with step
    }

    if (_RimLightingOn) // apply rim lighting only if the option is enabled
    {
        float rimFactor = calculateRimLightFactor(viewerDir, normal);
        rimLight = diffuseLight * rimFactor;
    }

    lightColor = (ambientLight + diffuseLight + specularLight + rimLight) * light.intensity;
    return lightColor;
}

void main()
{
    vec3 color = vec3(0);
    vec3 matColor = vec3(_Material.objColor);
    vec4 totalColor;
    vec3 normal = normalize(v_out.WorldNormal);
    int totalLights;
    vec4 tex1;
    vec4 tex2;

    totalLights = _NumDirLights + _NumPntLights + _NumSptLights;

    if (_Animated)
    {
        tex1 = texture(_BrickTexture1, uv + _Time);
        tex2 = texture(_BrickTexture2, uv + _Time); 
    }
    else
    {
        tex1 = texture(_BrickTexture1, uv);
        tex2 = texture(_BrickTexture2, uv);
    }
    
    // directional lights
    for(int d = 0; d < _NumDirLights; d++)
    {
        color += calculateDirLight(_DirLights[d]);
    }

    totalColor = vec4(matColor * color, 1.0);

    if (_TexChoice == 1)
    {
        FragColor = vec4(totalColor * tex1);
    }
    else if (_TexChoice == 2)
    {
        FragColor = vec4(totalColor * tex2);
    }
}
