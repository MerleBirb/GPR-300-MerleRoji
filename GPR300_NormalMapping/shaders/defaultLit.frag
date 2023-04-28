#version 450                          

out vec4 FragColor;
in vec2 uv;
in mat3 worldTBN;

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

#define MAX_TEXTURES 2
uniform sampler2D _BrickTextures[MAX_TEXTURES];
uniform sampler2D _NormalMaps[MAX_TEXTURES];
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

    diffuseLight = difCoefficient * max(dot(toLightDir, surfaceNormal), 0) * color; // max range 0 - 1

    return diffuseLight;
}

vec3 specular(float specCoefficient, vec3 toLightDir, vec3 surfaceNormal, float shininess, vec3 color)
{
    vec3 specularLight;
    vec3 viewerDir = normalize(_CameraPos - v_out.WorldPosition);
    //vec3 reflectDir = reflect(-toLightDir, surfaceNormal);

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

vec3 calculateDirLight(DirectionalLight light, vec3 normal)
{
    vec3 lightColor = vec3(0);

    vec3 toLightDir = normalize(light.direction - v_out.WorldPosition);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity;
    return lightColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal)
{
    vec3 lightColor = vec3(0);

    vec3 toLightDir = normalize(light.position - v_out.WorldPosition);
    float dist = length(light.position - v_out.WorldPosition);
    float att = attenuation(dist, light.radius);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity * att;
    return lightColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal)
{
    vec3 lightColor = vec3(0);

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

    vec4 tex;
    vec3 normalmap;

    if (_Animated)
    {
        tex = texture(_BrickTextures[_TexChoice], uv + _Time);
        normalmap = texture(_NormalMaps[_TexChoice], uv + _Time).rgb;
        normalmap = normalmap * 2.0 - 1.0; // convert from [0, 1] to [-1, 1]
        normalmap = worldTBN * normalmap;
        normalmap = normalize(normalmap);
    }
    else
    {
        tex = texture(_BrickTextures[_TexChoice], uv);
        normalmap = texture(_NormalMaps[_TexChoice], uv).rgb;
        normalmap = normalmap * 2.0 - 1.0; // convert from [0, 1] to [-1, 1]
        normalmap = worldTBN * normalmap;
        normalmap = normalize(normalmap);
    }
    
    // directional lights
    for(int d = 0; d < _NumDirLights; d++) { color += calculateDirLight(_DirLights[d], normalmap); }

    // point lights
    for(int p = 0; p < _NumPntLights; p++) { color += calculatePointLight(_PntLights[p], normalmap); }

    // spotlights
    for (int s = 0; s < _NumSptLights; s++) { color += calculateSpotLight(_SptLights[s], normalmap); }

    vec3 matColor = vec3(_Material.objColor);
    vec4 totalColor = vec4(matColor * color, 1.0f);

    FragColor = vec4(totalColor * tex);
}
