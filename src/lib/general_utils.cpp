#include "general_utils.h"
#include <stdio.h>

/* Support for high-dpi devices (i.e. Mac) */
void retina_glViewPort() {
	int width, height;
	glfwGetFramebufferSize(g_window, &width, &height);
	glViewport (0, 0, width, height);
}

void render_defaults() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	// depth-testing interprets a smaller value as "closer"
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	retina_glViewPort();
}
