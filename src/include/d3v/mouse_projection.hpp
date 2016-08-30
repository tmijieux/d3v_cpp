#ifndef D3V_MOUSE_PROJECTION_H
#define D3V_MOUSE_PROJECTION_H

#include <glm/glm.hpp>

namespace d3v {

using namespace glm;
void MouseProjection(vec3 &pos, int x,int y, mat4 const &view, mat4 const &proj);

}; // end namespace d3v

#endif // D3V_MOUSE_PROJECTION_H
