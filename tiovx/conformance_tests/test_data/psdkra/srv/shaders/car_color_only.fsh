precision mediump float;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;
uniform float u_opacity;

varying vec2 factors;
//varying float coded_factors;

vec2 packFloatToVec2i(const float value)
{
    const vec2 bitSh = vec2(65536.0, 1.0);
    const vec2 bitMsk = vec2(0.0, 1.0/65536.0);
    vec2 res = fract(value * bitSh);
    res -= res.xy * bitMsk;
    return res;
}

void main()
{
    //vec2 factors = packFloatToVec2i(coded_factors);
    float brightness = factors.x;
    float dampedfactor = factors.y;
	vec3 specularcomp = dampedfactor * 1.0 * u_specular;

    vec3 temp_color = u_ambient + specularcomp + brightness * u_diffuse;
    gl_FragColor.rgb = temp_color.bgr;
	gl_FragColor.a = u_opacity;
}
