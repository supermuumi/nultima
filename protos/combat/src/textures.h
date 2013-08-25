#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

// TODO write a proper manager for this


#include <map>
#include <string>

using namespace std;

class TextureManager {
 public:
    TextureManager();
    GLuint addTexture(std::string id, char *fname);
    GLuint get(std::string id);
    
 private:
    std::map<std::string, GLuint> textures;
};

#endif
