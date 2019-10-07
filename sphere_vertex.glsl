in vec3 aPos;
in vec2 aTexCoords;
in vec3 aNormal;

varying vec2 TexCoords;
varying vec3 Normal;

uniform mat4 transform;

void main()
{
    TexCoords = aTexCoords;
    Normal = aNormal;

    gl_Position =  transform*vec4(aPos, 1.0);
}
