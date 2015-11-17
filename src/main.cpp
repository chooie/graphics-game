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
#include "lib/camera.cpp"
#include "lib/user_input.cpp"

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());

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

	/* Shaders */
	GLuint shader_programme = create_programme_from_files (
		"shaders/test_vs.glsl", "shaders/test_fs.glsl"
	);

	do_texture_stuff(shader_programme);

	int model_mat_location = glGetUniformLocation (shader_programme, "model");
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");

	/* Camera */
	Camera cam;

	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, cam.view_mat.m);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, cam.proj_mat.m);

	mat4 model = identity_mat4();

	/* Rendering Defaults */

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	// depth-testing interprets a smaller value as "closer"
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	glViewport (0, 0, g_gl_width*2, g_gl_height*2);

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw Monkey
		glUseProgram (shader_programme);
		model = translate(identity_mat4(), vec3(2.5, 0.0, 0.0));
		glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model.m);
		glBindVertexArray (monkey_vao);
		glDrawArrays (GL_TRIANGLES, 0, monkey_point_count);

		// Draw Planet
		glUseProgram (shader_programme);
		model = translate(identity_mat4(), vec3(-2.5, 0.0, 0.0));
		glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model.m);
		glBindVertexArray (planet_vao);
		glDrawArrays (GL_TRIANGLES, 0, planet_point_count);

		model = model = translate(identity_mat4(), vec3(-10, -10, 0.0));
		glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model.m);
		glDrawArrays (GL_TRIANGLES, 0, planet_point_count);

		// update other events like input handling
		glfwPollEvents ();

		handle_user_input(cam, elapsed_seconds, view_mat_location);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
