#ifndef load_utils_h
#define load_utils_h

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <assert.h>
#include "../stb_image.h"

bool load_texture (const char* file_name, GLuint* tex);
bool load_mesh (const char* file_name, GLuint* vao, int* point_count);
void do_texture_stuff(GLuint shader_programme);

#endif
