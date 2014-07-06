
varying vec2 texCoord;

uniform sampler2D screen;

void main()
{
	gl_FragColor = vec4(texture2D(screen, texCoord).xyz / 8.0, 1.0);
}