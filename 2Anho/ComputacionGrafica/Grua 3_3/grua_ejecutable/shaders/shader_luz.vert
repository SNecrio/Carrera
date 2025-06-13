#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out vec3 ourNormal;
out vec3 FragPos;
out vec3 ourColor;
out vec2 TexCoord;

uniform vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
   FragPos = vec3(model * vec4(aPos, 1.0));
   ourNormal = Normal;
   ourColor = Color;
   TexCoord = aTexCoord;
}