#ifndef D3V_SCENE_H
#define D3V_SCENE_H

#include "d3v/object.hpp"
#include "d3v/light.hpp"
#include "d3v/camera.hpp"

namespace d3v {

class Scene {
public:
    int m_button;
    int m_xold, m_yold;

    Camera m_camera;
    Light m_light;

    std::vector<Object*> m_objects;
    std::vector<Texture*> m_textures;
    std::vector<Model*> m_models;
    
public:
    Scene();
    ~Scene();
    void AddModel(Model *m);
    void AddObject(Object *m);
    void AddTexture(Texture *m);
};

}; // end namespace d3v


#endif // D3V_SCENE_H
