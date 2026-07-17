//#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 flameColor;

void main() {
    vec2 center = TexCoords - vec2(0.5);
    float dist = length(center);

    float alpha = smoothstep(0.5, 0.1, dist);

    FragColor = vec4(flameColor, alpha);
}