#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/shader.hpp"

using d3v::Shader;
using d3v::ShaderProgram;
using std::string;

Shader::Shader(char const *source, GLint vertex_type)
{
    m_ID = glCreateShader(vertex_type);
    glShaderSource(m_ID, 1, &source, NULL);
    glCompileShader(m_ID);

    int status;
    glGetShaderiv(m_ID, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_len;
        glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = (char*) g_malloc(log_len + 1);
        glGetShaderInfoLog(m_ID, log_len, NULL, buffer);
        printf("shader compilation error: %s\n", buffer);
        g_free(buffer);
        glDeleteShader(m_ID);
        m_ID = 0;
    }
}

Shader *Shader::FromResource(char const *resource_path, GLint vertex_type)
{
    GBytes *source;
    Shader *shader;

    source = g_resources_lookup_data(resource_path, (GResourceLookupFlags)0, NULL);
    shader = new Shader((char*)g_bytes_get_data(source, NULL), vertex_type);
    g_bytes_unref(source);
    return shader;
}

Shader *Shader::VertexFromResource(char const *resource_path)
{
    return FromResource(resource_path);
}

Shader *Shader::FragmentFromResource(char const *resource_path)
{
    return FromResource(resource_path, GL_FRAGMENT_SHADER);
}

ShaderProgram::ShaderProgram(Shader *shader, ...)
{
    va_list ap;
    va_start(ap, shader);

    m_ID = glCreateProgram();
    while (shader != NULL) {
        glAttachShader(m_ID, shader->GetID());
        shader = va_arg(ap, Shader*);
    }

    glLinkProgram(m_ID);
    va_end(ap);

    int status = 0;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_len = 0;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = (char*) g_malloc(log_len + 1);
        glGetProgramInfoLog(m_ID, log_len, NULL, buffer);
        printf("shader linking error: %s\n", buffer);
        g_free(buffer);
        glDeleteProgram(m_ID);
        m_ID = 0;
    }
    if (m_ID) {
        m_model_location =      glGetUniformLocation(m_ID, "model");
        m_view_location =       glGetUniformLocation(m_ID, "view");
        m_projection_location = glGetUniformLocation(m_ID, "projection");
        m_position_location =   glGetAttribLocation(m_ID, "position");
        m_tex_coord_location =  glGetAttribLocation(m_ID, "TexCoord");
        glUseProgram(m_ID);
    } else {
        printf("error creating program\n");
    }
}

GLuint ShaderProgram::GetID() const
{
    return m_ID;
}

GLuint ShaderProgram::GetModelLocation() const
{
    return m_model_location;
}

GLuint ShaderProgram::GetPositionLocation() const
{
    return m_position_location;
}

GLuint ShaderProgram::GetProjectionLocation() const
{
    return m_projection_location;
}

GLuint ShaderProgram::GetViewLocation() const
{
    return m_view_location;
}

GLuint ShaderProgram::GetColorLocation() const
{
    return m_color_location;
}

GLuint ShaderProgram::GetTexCoordLocation() const
{
    return m_tex_coord_location;
}

GLuint ShaderProgram::GetNormalLocation() const
{
    return m_tex_coord_location;
}
