#version 400

in vec3 pos_eye;
in vec3 norm_eye;
// Texture coord
in vec2 st;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D ambient_map;
uniform sampler2D emission_map;

uniform mat4 view;

out vec4 frag_colour;

vec3 light_position_world = vec3 (1.0, 1.0, 10.0);

/* Light properties */

// Specular
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white
// Diffuse
vec3 Ld = vec3 (0.9, 0.9, 0.9); // dull white light
// Ambient
vec3 La = vec3 (0.2, 0.2, 0.2); // grey

// Phong exponent (shininess parameter)
float specular_exponent = 100.0; // specular 'power'

void main() {
	vec3 light_pos_eye = (view * vec4 (light_position_world, 1.0)).xyz;

	// Ambient reflectance
	vec3 Ka = texture (ambient_map, st).rgb;

	// Ambient intensity
	vec3 Ia = vec3 (0.2, 0.2, 0.2) * Ka;

	// Texture pixel
	vec4 texel = texture (diffuse_map, st);

	// Diffuse reflectance
	vec3 Kd = texel.rgb;
	vec3 surface_to_light_eye = normalize (light_pos_eye - pos_eye);

	// Dot Product
	// Diffuse light should be brightest when directly facing the light and
	// not lit at all when the surface is perpendicular to the light.
	float dp = max (0.0, dot (norm_eye, surface_to_light_eye));

	// Diffuse intensity
	vec3 Id = Kd * Ld * dp;

	// Specular reflectance
	vec3 Ks = texture (specular_map, st).rgb;
	vec3 surface_to_viewer_eye = normalize (-pos_eye);
	vec3 half_way_eye = normalize (surface_to_viewer_eye + surface_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, norm_eye), 0.0);
	float specular_factor = pow (dot_prod_specular, specular_exponent);
	vec3 Is = Ls * Ks * specular_factor; // final specular intensity

	vec3 texel_e = texture (emission_map, st).rgb;

	frag_colour = vec4 (Id + Is + Ia + texel_e, 1.0);
}
