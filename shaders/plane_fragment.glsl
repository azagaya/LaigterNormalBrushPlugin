varying vec2 TexCoord;

uniform vec4 color;

void main()
{
  if (TexCoord.x >= 0.9 || TexCoord.x <= 0.1 || TexCoord.y >= 0.9 || TexCoord.y <= 0.1)
    gl_FragColor = vec4(color.xyz*0.3,1.0);
  else
    gl_FragColor = vec4(color.xyz,1.0);
}
