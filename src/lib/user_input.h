#ifndef user_input_h
#define user_input_h

#include <GLFW/glfw3.h> // GLFW helper library
#include "Camera.h"

extern GLFWwindow* g_window;

void handle_user_input(Camera &cam, float elapsed_seconds);

#endif
