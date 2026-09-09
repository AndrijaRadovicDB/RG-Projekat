//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec3 viewPos;

// Directional light
uniform vec3 dirLight_direction;
uniform vec3 dirLight_color;

// Point light
uniform vec3 pointLight_position;
uniform vec3 pointLight_color;
uniform float pointLight_const;
uniform float pointLight_linear;
uniform float pointLight_quadratic;

vec3 calc_dir_light(vec3 normal, vec3 viewDir) {
    vec3 dirLight = normalize(-dirLight_direction);

    float diff = max(dot(normal, dirLight), 0.0);

    vec3 halfwayDir = normalize(dirLight + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 ambient = 0.1 * dirLight_color;
    vec3 diffuse = diff * dirLight_color;
    vec3 specular = spec * 0.5 * dirLight_color;

    return ambient + diffuse + specular;
}

vec3 calc_point_light(vec3 normal, vec3 viewDir) {
    vec3 dirLight = normalize(-dirLight_direction);

    float diff = max(dot(normal, dirLight), 0.0);

    vec3 halfwayDir = normalize(dirLight + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    float distance = length(pointLight_position - FragPos);
    float attenuation = 1.0 / (pointLight_const + pointLight_linear * distance + pointLight_quadratic * (distance * distance));

    vec3 ambient = 0.1 * pointLight_color;
    vec3 diffuse = diff * pointLight_color;
    vec3 specular = spec * 0.5 * pointLight_color;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    result += calc_dir_light(normal, viewDir);
    result += calc_point_light(normal, viewDir);

    result *= objectColor;

    //FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);
    FragColor = vec4(objectColor, 1.0);
}