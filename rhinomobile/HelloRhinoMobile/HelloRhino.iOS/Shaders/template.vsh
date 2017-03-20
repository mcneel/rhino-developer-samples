attribute vec4 rglVertex;
uniform mat4 rglModelViewProjectionMatrix;

void main()
{
	gl_Position = rglModelViewProjectionMatrix * rglVertex;
}