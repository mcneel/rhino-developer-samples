attribute vec4 rglVertex;
attribute vec3 rglColor;

varying vec3 vColor;

void main(void)
{
  gl_Position = rglVertex;
  vColor = rglColor;
}