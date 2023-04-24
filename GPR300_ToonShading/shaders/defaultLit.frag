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

vec3 calculateDirLight(DirectionalLight light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 toLightDir = normalize(light.direction - v_out.WorldPosition);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity;
    return lightColor;
}

vec3 calculatePointLight(PointLight light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 toLightDir = normalize(light.position - v_out.WorldPosition);
    float dist = length(light.position - v_out.WorldPosition);
    float att = attenuation(dist, light.radius);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity * att;
    return lightColor;
}

vec3 calculateSpotLight(SpotLight light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 dirToFrag = normalize(v_out.WorldPosition - light.position);

    float theta = dot(dirToFrag, light.direction);
    float att = angularAttenuation(theta, light.minAngle, light.maxAngle, 2);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, dirToFrag, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, dirToFrag, normal, _Material.shininess, light.color);

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity * att;
    return lightColor;
}

void main()
{
    vec3 color = vec3(0);
    vec3 matColor = vec3(_Material.objColor);
    vec4 matLightingColor = vec4(0);
    vec4 totalColor;
    float lightIntensity;
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
        lightIntensity += dot(_DirLights[d].direction, normal);
    }

    // point lights
    for(int p = 0; p < _NumPntLights; p++)
    {
        color += calculatePointLight(_PntLights[p]);
        lightIntensity += dot(_PntLights[p].position, normal);
    }

    // spotlights
    for (int s = 0; s < _NumSptLights; s++)
    {
        color += calculateSpotLight(_SptLights[s]);
        lightIntensity += dot(_SptLights[s].direction, normal);
    }

    matLightingColor = vec4(matColor * color, 1.0);
    lightIntensity /= totalLights;

    if (lightIntensity > 0.95)
    {
        totalColor = matLightingColor * 1;
    }
    else if (lightIntensity > 0.1)
    {
        totalColor = matLightingColor * 0.75;
    }
    else
    {
        totalColor = matLightingColor * 0.1;
    }

    if (_TexChoice == 1)
    {
        FragColor = vec4(totalColor * tex1);
    }
    else if (_TexChoice == 2)
    {
        FragColor = vec4(totalColor * tex2);
    }
}
