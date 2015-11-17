/* NOTE: this shader is for GLSL 4.2.0 (OpenGL 4.2)
 to convert it to an earlier version, for example on Apple, you'll need to
 remove layout (binding = x) for each texture, and instead explicitly
 set glUniform1i() for each texture in C with these values */

#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

// uniform vec3 vertex_position;
// uniform vec3 vertex_normal;
// uniform vec2 texture_coord;

uniform mat4 model, view, proj;

out vec3 pos_eye;
out vec3 norm_eye; // positions and normals in eye space
out vec2 st; // texture coordinates passed through from from vertex shader

void main() {
	st = texture_coord;
	norm_eye = (view * vec4 (vertex_normal, 0.0)).xyz;;
	pos_eye = (view * vec4 (vertex_position, 1.0)).xyz;
	gl_Position = proj * view * model * vec4 (vertex_position, 1.0);
}
