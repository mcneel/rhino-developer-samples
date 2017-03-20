uniform sampler2D Image;

varying mediump vec2 TexCoord0;

void main(void)
{
  gl_FragColor = texture2D( Image, TexCoord0 );
}