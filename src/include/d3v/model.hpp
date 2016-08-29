#ifndef D3V_MODEL_H
#define D3V_MODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glib.h>
#include "d3v/shader.hpp"

namespace d3v {

using namespace glm;

struct Vertex {
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat s, t;
};

class Model {
private:
    std::string m_filePath;
    std::string m_modelName;
    bool m_haveTexture;
    bool m_haveNormal;
    std::vector<Vertex> m_vertices;
    
    GLuint m_vaoID;
    GLuint m_vboID;

    void LoadObjects(GIOChannel *chan);
    void MakeVAO(ShaderProgram const &prog);
    
public:
    Model(std::string const &filePath,
          std::string const &modelName,
          ShaderProgram const &program);
    ~Model();
    
    void Draw(ShaderProgram const &program);
    void Dump(std::ostream &out);

    static Model *GetByName(std::string const &name);
};

}; // end namespace d3v


#endif // D3V_MODELS_H
