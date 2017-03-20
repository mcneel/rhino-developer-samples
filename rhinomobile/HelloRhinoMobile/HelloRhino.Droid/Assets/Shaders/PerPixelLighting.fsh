precision highp float;

uniform vec4  rglDiffuse;
uniform vec4  rglSpecular;
uniform float rglShininess;
uniform vec4  rglLightAmbient;
uniform vec4  rglLightDiffuse;
uniform vec4  rglLightSpecular;
uniform vec3  rglLightPosition;

varying vec3  vNormal;
varying vec4  vColor;

void main()
{
  vec3 N = normalize( vNormal );
    
  vec3 L = normalize( rglLightPosition );
  vec3 H = normalize( L + vec3(0, 0, 1) );
  
  float diff = max( 0.0, dot(N, L) );
  float spec = max( 0.0, dot(N, H) );
  spec = pow( spec, rglShininess );

  lowp vec3 color = rglLightAmbient.rgb + 
                    (rglLightDiffuse.rgb  * rglDiffuse.rgb  * vColor.rgb * diff) + 
                    (rglLightSpecular.rgb * rglSpecular.rgb * spec);

  gl_FragColor = vec4( color, rglDiffuse.a );
}