attribute vec4 rglVertex;
attribute vec2 rglTexCoord0;

varying vec2 TexCoord0;

void main(void)
{
  gl_Position = rglVertex;
  TexCoord0 = rglTexCoord0;
}