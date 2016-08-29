#include <stdio.h>
#include <string.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "d3v/camera.hpp"
using namespace glm;
using d3v::Camera;

static void mat4_print(mat4 const &m)
{
    for (unsigned row = 0; row < 4; ++row) {
        for (unsigned col = 0; col < 4; ++col)
            printf("%f ", m[col][row]);
        printf("\n");
    }
}

#define deg2rad(deg) ((deg) * M_PI / 180.f)

Camera::Camera(vec3 const &look, float distance, float ay, float ax):
    m_distance(distance),
    m_updated(true),
    m_look(look),
    m_reye(0.0f, 0.0f, distance),
    m_yaxis(0.0f, 1.0f, 0.0f),
    m_xaxis(1.0f, 0.0f, 0.0f),
    m_projection(glm::perspective((float) deg2rad(45.0f), 1.0f, 0.1f, 100.0f))
{
    ComputeView();
    printf("cpp projection:\n");
    mat4_print(m_projection);
    
    printf("cpp view:\n");
    mat4_print(m_view);
}

Camera::~Camera()
{
}

void Camera::ComputeView()
{
    m_view = glm::lookAt(m_distance * m_reye + m_look, m_look, m_yaxis);
    m_updated = true;
}

void Camera::SetLook(vec3 const &look)
{
    m_look = look;
    ComputeView();
}

void Camera::SetDistance(float distance)
{
    m_distance = distance > 0 ? distance : 0.;
    ComputeView();
}

void Camera::Translate(float dw, float dh)
{
    vec3 tmp = dw * m_xaxis + dh * m_yaxis; 
    m_look += tmp;
    ComputeView();
}

void Camera::AddDistance(float d)
{
    m_distance += d;
    m_distance = m_distance > 0 ? m_distance : 0.0f;
    ComputeView();
}

void Camera::Rotate(float angleX, float angleY)
{
    mat4 R;
    
    R = glm::rotate(mat4{1.0f}, angleX, m_xaxis);
    m_reye = vec3( R * vec4(m_reye, 1.0) );
    m_yaxis = vec3( R * vec4(m_yaxis, 1.0) );

    R = glm::rotate(mat4{1.0f}, angleY, vec3{0.0,1.0,0.0});
    m_reye = vec3( R * vec4(m_reye, 1.0) );
    m_xaxis = vec3( R * vec4(m_xaxis, 1.0) );
    m_yaxis = vec3( R * vec4(m_yaxis, 1.0) );
    
    ComputeView();
}

void Camera::Update(ShaderProgram const &program)
{
    if (m_updated) {
        int v = program.GetViewLocation();
        int p = program.GetProjectionLocation();
        glUniformMatrix4fv(v, 1, GL_FALSE, &(m_view[0][0]));
        glUniformMatrix4fv(p, 1, GL_FALSE, &(m_projection[0][0]));
        m_updated = false;
    }
}
