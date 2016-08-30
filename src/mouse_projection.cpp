#include <math.h>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "d3v/mouse_projection.hpp"

using namespace glm;

void d3v::MouseProjection(vec3 &pos, int x, int y,
                          mat4 const &view, mat4 const &proj)
{
    GLdouble ox = 0.0, oy = 0.0, oz = 0.0;
    GLint viewport[4];
    vec3 window;

    glGetIntegerv(GL_VIEWPORT, viewport);
    ivec4 vp = { viewport[0], viewport[1], viewport[2], viewport[3] };

    window.x = x;
    window.y = y = viewport[3] - y;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);
    pos = glm::unProject(window, view, proj, vp);
}
