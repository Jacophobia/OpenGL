#ifndef RENDEREDOBJECT_H
#define RENDEREDOBJECT_H

#include "GameObject.h"

class RenderedObject :
    public GameObject
{
private:
    const float TO_RADIANS = 3.14159265f / 180.0f;

    Shader shader;
    Mesh mesh;
    Material material;
    glm::mat4 model = glm::mat4(1.0f);
public:
    RenderedObject();

    void useShader();
    void useMesh();
    void useModel();
    void useTexture();
    void useMaterial();

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z, float degrees);

    ~RenderedObject();
};

#endif