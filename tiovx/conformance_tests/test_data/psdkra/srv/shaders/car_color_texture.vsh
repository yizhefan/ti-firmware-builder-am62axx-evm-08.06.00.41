attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

uniform mat4  u_mvp;
uniform mat4  u_world;
uniform mat4  u_view;
uniform mat4  u_viewinv;
uniform mat4  u_projection;
uniform vec3  u_lightdir;
uniform float u_attenuation;

varying vec2 factors;
varying vec2  v_texcoord;

void main()
{
	vec4 pos_world = u_world * vec4(a_vertex, 1.0);
	gl_Position = u_projection * u_view * pos_world;
	vec3 v_worldnormal = (u_world * vec4(a_normal, 1.0)).xyz;
	vec3 v_tolight = u_lightdir - pos_world.xyz;
	vec3 v_tocamera = (u_viewinv * vec4(0.0, 0.0, 0.0, 1.0)).xyz - pos_world.xyz;

	//vec2 factors;
	vec3 unit_normal = normalize(v_worldnormal);
	vec3 unit_tolight = normalize(v_tolight);
	vec3 unit_tocamera = normalize(v_tocamera);
	vec3 fromlight = -unit_tolight;
	vec3 reflectedlight = reflect(fromlight, unit_normal);
	float nDotl = dot(unit_normal, unit_tolight);
	float brightness = max(nDotl, 0.2);
	factors.x = brightness;


	float specularfactor = dot(reflectedlight, unit_tocamera);
	specularfactor = max(specularfactor, 0.0);
	float dampedfactor = pow(specularfactor, u_attenuation);
	factors.y = dampedfactor;
	v_texcoord = a_texcoord;
}

