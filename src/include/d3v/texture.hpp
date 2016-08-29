#ifndef D3V_TEXTURE_H
#define D3V_TEXTURE_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <cstdint>
#include <vector>
#include <string>



namespace d3v {

class Texture {

private:
    GLuint m_texID;
    std::vector<uint8_t> m_textureData;
    void LoadJpeg(std::string const &file_path, int &width, int &height);
    
public:
    Texture(std::string const &);
    ~Texture();
    GLuint GetID();
};

}; //end namespace d3v

#endif // D3V_TEXTURE_H
