#ifndef D3V_LIGHT_H
#define D3V_LIGHT_H

#include <glm/glm.hpp>

namespace d3v {

using namespace glm;

class Light {
    
private:
    vec4 m_pos;
    vec4 m_dif;
    vec4 m_dir;
    vec4 m_Mspec;
    GLfloat m_Mshiny;
public:
    Light();
    void Update();
};

}; // end namespace d3v

#endif // D3V_LIGHT_H
