#version 450                          
out vec4 FragColor;

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
uniform DirectionalLight _DirLights[MAX_DIR_LIGHTS];

#define MAX_PNT_LIGHTS 2
uniform PointLight _PntLights[MAX_PNT_LIGHTS];

#define MAX_SPT_LIGHTS 1
uniform SpotLight _SptLights[MAX_SPT_LIGHTS];

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
    return clamp(pow((theta - maxAngle)/(minAngle - maxAngle), fallOffCurve), 0, 1);
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

    ambientLight *= att;
    diffuseLight *= att;
    specularLight *= att;

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity;
    return lightColor;
}

vec3 calculateSpotLight(SpotLight light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 toLightDir = normalize(light.direction - v_out.WorldPosition);

    float theta = dot(toLightDir, light.direction);
    float att = angularAttenuation(cos(radians(theta)), cos(radians(light.minAngle)), cos(radians(light.maxAngle)), 0.5);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    ambientLight *= att;
    diffuseLight *= att;
    specularLight *= att;

    lightColor = (ambientLight + diffuseLight + specularLight) * light.intensity;
    return lightColor;
}

void main()
{
    vec3 color = vec3(0);
    
    // directional lights
    for(int d = 0; d < MAX_DIR_LIGHTS; d++)
    {
        color += calculateDirLight(_DirLights[d]);
    }

    // point lights
    for(int p = 0; p < MAX_PNT_LIGHTS; p++)
    {
        color += calculatePointLight(_PntLights[p]); 
    }

    // spotlights
    for (int s = 0; s < MAX_SPT_LIGHTS; s++)
    {
        color += calculateDirLight(_DirLights[s]);
    }

    FragColor = vec4(_Material.objColor * color, 1.0f);
}
