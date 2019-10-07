
attribute vec3 aPos;
attribute vec2 aTexCoord;

varying vec2 TexCoord;

uniform mat4 transform;

void main()
{
    TexCoord = aTexCoord;
    gl_Position =  transform*vec4(aPos, 1.0);
}
