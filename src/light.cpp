#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "d3v/light.hpp"

using d3v::Light;

Light::Light()
{
    m_pos =   vec4 { 2.0,  2.0,  2.0, 0. };
    m_dif =   vec4 { 1.0,  1.0,  1.0, 0.0 };
    m_dir =   vec4 { -1.0, -1.0, -1.0, 0.0 };
    m_Mspec = vec4 { 1.0,  1.0,  1.0, 0.0 };
    m_Mshiny = 80.;

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Light::Update()
{
    const float pos[4] = {
        m_pos.x,
        m_pos.y,
        m_pos.z,
        m_pos.w,
    };

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

