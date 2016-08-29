#ifndef D3V_OBJECT_H
#define D3V_OBJECT_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/texture.hpp"
#include "d3v/model.hpp"
#include "d3v/shader.hpp"

namespace d3v {

using namespace glm;

class Object {
private:
    mat4 m_translate;
    mat4 m_rotate;
    mat4 m_scale;
    vec3 m_color;

    mat4 m_modelMat;
    
    Model *m_model;

    GLuint m_textureID;
    GLint m_shademodel;
    
    bool m_updated;
    bool m_textured;
    bool m_colored;
    bool m_hidden;

    ShaderProgram *m_program;
    void ComputeModel();

public:
    Object(Model *m, Texture *t, vec3 const &pos, float yrot, float scale);
    ~Object();

    void Draw();
    void Hide();
    void Reveal();
    void SetShadeModel(GLint shademodel);
    void SetProgram(ShaderProgram *program);
    void Translate(vec3 const &pos);
    void SetPosition(vec3 const &pos);
    void SetShaderProgram(ShaderProgram *prog);
    ShaderProgram *GetProgram();
};

}; // end namespace d3v

#endif // D3V_OBJECT_H
