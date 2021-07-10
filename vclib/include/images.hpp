#include <GL/gl3w.h> 

#include <stb_image.hpp>

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);