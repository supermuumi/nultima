#include "textures.h"
#include "gl_utils.h"

using namespace std;

TextureManager::TextureManager() {
}

GLuint TextureManager::addTexture(string id, char *fname) {
    GLuint ret;

    ret = loadTexture(fname);
    textures[id] = ret;

    return ret;
}

GLuint TextureManager::get(string id) {
    std::map<std::string,GLuint>::const_iterator poop = textures.find(id);
    if (poop != textures.end())
	return poop->second;
    return -1;

}
