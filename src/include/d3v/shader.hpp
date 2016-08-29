#ifndef D3V_SHADER_H
#define D3V_SHADER_H

#include <string>
#include <vector>

namespace d3v {
class Shader {
private:
    GLuint m_ID;
public:
    Shader(char const *program, GLint vertex_type);
    GLuint GetID() { return m_ID; }
    static Shader *FromResource(char const *resource_path,
                                GLint vertex_type = GL_VERTEX_SHADER);
    static Shader *VertexFromResource(char const *resource_path);
    static Shader *FragmentFromResource(char const *resource_path);
};

class ShaderProgram {
private:
    GLuint m_ID;

    GLuint m_model_location;
    GLuint m_view_location;
    GLuint m_projection_location;
    GLuint m_position_location;
    GLuint m_color_location;
    GLuint m_tex_coord_location;
    GLuint m_normal_location;
    
public:
    ShaderProgram(Shader *shader1, ...);
    GLuint GetID() const;
    GLuint GetModelLocation() const;
    GLuint GetPositionLocation() const;
    GLuint GetProjectionLocation() const;
    GLuint GetViewLocation() const;
    GLuint GetTexCoordLocation() const;
    GLuint GetColorLocation() const;
    GLuint GetNormalLocation() const;
};

};


#endif // D3V_SHADER_H
