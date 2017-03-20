precision highp float;

uniform vec4  rglLightAmbient;

varying vec4  vDiffuse;
varying vec4  vSpecular;

void main()
{
	gl_FragColor = vec4( rglLightAmbient.rgb + vDiffuse.rgb + vSpecular.rgb, vDiffuse.a );
}