#ifndef D3V_CAMERA_H_
#define D3V_CAMERA_H_

#include <glm/glm.hpp>
#include "d3v/compiler.hpp"
#include "d3v/shader.hpp"

namespace d3v {
using namespace glm;

class Camera {
private:
    float m_distance;
    vec3 m_reye; // camera center relative to look
    vec3 m_look; // point looked at
    vec3 m_yaxis; // up vector
    vec3 m_xaxis; // right vector

    bool m_updated;
    mat4 m_view;
    mat4 m_projection;
    void ComputeView();
    
public:
    Camera();
    Camera(vec3 const &pos, float distance, float angleR, float angleY);
    ~Camera();

    void SetLook(vec3 const &look);
    void SetDistance(float distance);
    void SetRotate(float angleZ, float angleX);
    void Rotate(float angleR, float angleY);
    void Translate(float dx, float dy);
    void AddDistance(float distance);

    void Update();
    void Update(ShaderProgram const &);
};

}; // end namespace d3v

#endif	// D3V_CAMERA_H_
