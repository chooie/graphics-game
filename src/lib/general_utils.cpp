#include "general_utils.h"

void render_defaults() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	// depth-testing interprets a smaller value as "closer"
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	glViewport (0, 0, g_gl_width*2, g_gl_height*2);
}
