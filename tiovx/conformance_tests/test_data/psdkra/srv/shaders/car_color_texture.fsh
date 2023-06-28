precision mediump float;

uniform sampler2D  sTexture;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;
uniform float u_opacity;

varying vec2 factors;
varying vec2 v_texcoord;

void main()
{
	float brightness = factors.x;
	float dampedfactor = factors.y;
	vec3 specularcomp = dampedfactor * 1.0 * u_specular;

	gl_FragColor.rgb = u_ambient + specularcomp + brightness * texture2D(sTexture, v_texcoord).bgr;
	gl_FragColor.a = u_opacity;
}
