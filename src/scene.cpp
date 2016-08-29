#include <string.h>
#include "d3v/scene.hpp"

using d3v::Scene;

Scene::Scene():
    m_camera({0.,0.,0.}, 1., 0.,0.)
{
}

Scene::~Scene()
{
}

void Scene::AddModel(Model *model)
{
    m_models.push_back(model);
}

void Scene::AddObject(Object *object)
{
    m_objects.push_back(object);
}

void Scene::AddTexture(Texture *texture)
{
    m_textures.push_back(texture);
}
