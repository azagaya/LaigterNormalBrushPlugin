in vec2 TexCoords;
in vec3 Normal;

uniform vec3 camPos;

const float PI = 3.14159265359;

void main()
{
  vec3 n = normalize(Normal);
    gl_FragColor = vec4(vec3(0.5*n.xy+0.5,-0.5*n.z+0.5),1.0);
}
