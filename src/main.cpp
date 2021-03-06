/******************************************************************************\
| Asteroid Game - Code is modified from Anton's OpenGL Tutorials (See below).  |
| Author: Charlie Hebert                                                       |
| Copyright Charlie Hebert                                                     |
|******************************************************************************|
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries separate legal notices                              |
|******************************************************************************|
| Quaternion Camera                                                            |
| Importing Meshes                                                             |
| Using Textures for Lighting Coefficients                                     |
\******************************************************************************/

#include "gl_utils.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "assert.h"

// My Stuff
#include "lib/general_utils.h"
#include "lib/load_utils.h"
#include "lib/camera.h"
#include "lib/user_input.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());

	GLuint monkey_vao;
	int monkey_point_count;
	assert (load_mesh ("meshes/monkey.obj", &monkey_vao, &monkey_point_count));

	GLuint planet_vao;
	int planet_point_count;
	assert(load_mesh("meshes/planet.obj", &planet_vao, &planet_point_count));

	/* Shaders */
	GLuint shader_programme = create_programme_from_files (
		"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"
	);

	do_texture_stuff(shader_programme);

	int model_mat_location = glGetUniformLocation (shader_programme, "model");
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");

	/* Create a Camera instance */
	Camera cam(view_mat_location, proj_mat_location);

	mat4 model;

	render_defaults();

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		cam.reset_control();

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

		handle_user_input(cam, elapsed_seconds);

		cam.update();

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
