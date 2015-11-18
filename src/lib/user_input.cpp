#include "user_input.h"

void handle_user_input(
  Camera &cam, float elapsed_seconds
) {

  // control keys
	if (glfwGetKey (g_window, GLFW_KEY_A)) {
    cam.strafe_left(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_D)) {
    cam.strafe_right(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_MINUS)) {
		cam.strafe_down(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_EQUAL)) {
		cam.strafe_up(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_W)) {
    cam.strafe_backwards(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_S)) {
    cam.strafe_forwards(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
		cam.yaw_left(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
		cam.yaw_right(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_UP)) {
		cam.pitch_down(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_DOWN)) {
    cam.pitch_up(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_Q)) {
		cam.roll_left(elapsed_seconds);
	}
	if (glfwGetKey (g_window, GLFW_KEY_E)) {
		cam.roll_right(elapsed_seconds);
	}

	if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose (g_window, 1);
	}
}
