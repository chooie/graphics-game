/******************************************************************************\
| Asteroid Game - Code is modified from Anton's OpenGL Tutorials.              |
| Original Header:                                                             |                                                      |
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries separate legal notices                              |
|******************************************************************************|
| Importing Meshes                                                             |
| Using Textures for Lighting Coefficients                                     |
\******************************************************************************/
#include "maths_funcs.h"
#include "stb_image.h"
#include "gl_utils.h"
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

#include "lib/load_utils.cpp"
#include "lib/user_input.cpp"
#include "lib/camera.cpp"

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	// depth-testing interprets a smaller value as "closer"
	glDepthFunc (GL_LESS);

	GLuint monkey_vao;
	int monkey_point_count;
	assert (load_mesh ("meshes/monkey.obj", &monkey_vao, &monkey_point_count));
	// Set up Monkey
	glBindVertexArray (monkey_vao);

	GLuint planet_vao;
	int planet_point_count;
	assert(load_mesh("meshes/planet.obj", &planet_vao, &planet_point_count));
	// Set up Planet
	glBindVertexArray (planet_vao);

	GLuint shader_programme = create_programme_from_files (
		"shaders/test_vs.glsl", "shaders/test_fs.glsl"
	);

	GLint diffuse_map_loc, specular_map_loc, ambient_map_loc, emission_map_loc;
	diffuse_map_loc = glGetUniformLocation (shader_programme, "diffuse_map");
	specular_map_loc = glGetUniformLocation (shader_programme, "specular_map");
	ambient_map_loc = glGetUniformLocation (shader_programme, "ambient_map");
	emission_map_loc = glGetUniformLocation (shader_programme, "emission_map");
	assert (diffuse_map_loc > -1);
	assert (specular_map_loc > -1);
	assert (ambient_map_loc > -1);
	assert (emission_map_loc > -1);
	glUseProgram (shader_programme);
	glUniform1i (diffuse_map_loc, 0);
	glUniform1i (specular_map_loc, 1);
	glUniform1i (ambient_map_loc, 2);
	glUniform1i (emission_map_loc, 3);

	// load texture
	GLuint tex_diff, tex_spec, tex_amb, tex_emiss;
	glActiveTexture (GL_TEXTURE0);
	assert (load_texture ("images/boulder_diff.png", &tex_diff));
	glActiveTexture (GL_TEXTURE1);
	assert (load_texture ("images/boulder_spec.png", &tex_spec));
	glActiveTexture (GL_TEXTURE2);
	assert (load_texture ("images/ao.png", &tex_amb));
	glActiveTexture (GL_TEXTURE3);
	assert (load_texture ("images/tileable9b_emiss.png", &tex_emiss));

	create_camera();

	glUseProgram (shader_programme);
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

  mat4 model = identity_mat4();
	int matrix_location = glGetUniformLocation(shader_programme, "model");

	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width*2, g_gl_height*2);

		glUseProgram (shader_programme);
		model = translate(identity_mat4(), vec3(2.5, 0.0, 0.0));
		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
		glBindVertexArray (monkey_vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, monkey_point_count);

		glUseProgram (shader_programme);
		model = translate(identity_mat4(), vec3(-2.5, 0.0, 0.0));
		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
		glBindVertexArray (planet_vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, planet_point_count);

		// update other events like input handling
		glfwPollEvents ();

		handle_user_input(
			cam_pos, elapsed_seconds, cam_speed, cam_yaw, cam_yaw_speed,
			view_mat_location
		);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
