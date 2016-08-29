#include <stdio.h>
#include <glib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "d3v/object.hpp"
#include "d3v/model.hpp"
#include "d3v/texture.hpp"
#define deg2rad(x) ((x)*(3.141592f/180.0f))

using namespace glm;
using d3v::Object;
using d3v::ShaderProgram;

Object::Object(Model *model, Texture* texture,
               vec3 const &pos, float yrot, float s):
    m_translate(glm::translate(pos)),
    m_rotate(glm::rotate(deg2rad(yrot), vec3{0.0f,1.0f,0.0f})),
    m_scale(glm::scale(mat4(1.0), vec3{s})),
    m_textured(false),
    m_colored(false),
    m_hidden(false),
    m_updated(false)
{
    m_shademodel = GL_SMOOTH;
    m_model = model;

    if (texture != NULL) {
        m_textureID = texture->GetID();
	m_textured = true;
    }
    ComputeModel();
}

void Object::SetShadeModel(GLint shademodel)
{
    m_shademodel = shademodel;
}

void Object::SetPosition(vec3 const &pos)
{
    m_translate = glm::translate(pos);
    ComputeModel();
}

void Object::Translate(vec3 const &pos)
{
    m_translate = glm::translate(pos) * m_translate;
    ComputeModel();
}

static void mat4_print(mat4 const &m)
{
    for (unsigned row = 0; row < 4; ++row) {
        for (unsigned col = 0; col < 4; ++col)
            printf("%f ", m[col][row]);
        printf("\n");
    }
}

void Object::ComputeModel()
{
    m_modelMat = m_translate * m_rotate * m_scale;
    m_updated = true;
    printf("model: \n");
    mat4_print(m_modelMat);
}

void Object::Draw()
{
    if (m_hidden)
	return;
    //glShadeModel(m_shademodel);

    // if (m_textured)
    //     glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    GLint m = m_program->GetModelLocation();
    glUniformMatrix4fv(m, 1, GL_FALSE, &(m_modelMat[0][0]));
    m_model->Draw(*m_program);
}

void Object::SetShaderProgram(ShaderProgram *prog)
{
    m_program = prog;
}

void Object::Hide()
{
    m_hidden = true;
}

void Object::Reveal()
{
    m_hidden = false;
}

Object::~Object()
{
}

ShaderProgram *Object::GetProgram()
{
    return m_program;
}
