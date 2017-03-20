attribute vec4 rglVertex;
attribute vec3 rglNormal;
attribute vec4 rglColor;

uniform mat4 rglProjectionMatrix;
uniform mat4 rglModelViewMatrix;
uniform mat3 rglNormalMatrix;

uniform vec4 rglDiffuse;
uniform vec4 rglSpecular;
uniform float rglShininess;
uniform vec4 rglLightDiffuse;
uniform vec4 rglLightSpecular;
uniform vec3 rglLightPosition;
uniform bool rglUsesColors;

varying vec4 vDiffuse;
varying vec4 vSpecular;

void main()
{
	vec4 vView   = rglModelViewMatrix * rglVertex;
  vec3 vNormal = rglNormalMatrix * rglNormal;
  
  if ( dot( -vView.xyz, vNormal ) < 0.0 )
    vNormal = -vNormal;
    
  vec3 N = normalize( vNormal );
  vec3 L = normalize( rglLightPosition );  // Assume directional light source...thus, pos = dir. 
  vec3 H = normalize( L + vec3(0, 0, 1) ); // Half vector 
  
  float diff = max( 0.0, dot(N, L) );
  float spec = max( 0.0, dot(N, H) );

  vDiffuse.rgb = rglLightDiffuse.rgb * rglDiffuse.rgb * diff;
  vDiffuse.a   = rglDiffuse.a;
  vSpecular    = rglLightSpecular * rglSpecular * pow( spec, rglShininess );
  	
  if ( rglUsesColors )
    vDiffuse.rgb *= rglColor.rgb;
    
  gl_Position = rglProjectionMatrix * vView;
}