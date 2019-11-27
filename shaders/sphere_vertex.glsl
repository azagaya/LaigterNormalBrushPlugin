attribute vec3 aPos;
attribute vec2 aTexCoords;
attribute vec3 aNormal;

varying vec2 TexCoords;
varying vec3 Normal;

uniform mat4 transform;

void main()
{
    TexCoords = aTexCoords;
    Normal = aNormal;

    gl_Position =  transform*vec4(aPos, 1.0);
}
