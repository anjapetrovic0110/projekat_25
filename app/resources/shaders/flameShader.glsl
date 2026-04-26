//#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec2 TexCoords;

void main()
{
    vec2 center = TexCoords - vec2(0.5);
    float dist = length(center);

    float alpha = smoothstep(0.5, 0.1, dist);
    vec3 color = vec3(3.0, 1.5, 0.3);

    FragColor = vec4(color, alpha);

    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0)
        BrightColor = vec4(color, alpha);
    else
        BrightColor = vec4(0.0);
}