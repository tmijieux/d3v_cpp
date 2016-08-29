#include <glib.h>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/model.hpp"
#include "d3v/shader.hpp"
#include "d3v/util.hpp"
#include "d3v/error.hpp"

using d3v::Model;
using d3v::Vertex;
using d3v::ShaderProgram;
using d3v::exception;

using std::string;
using std::vector;

using namespace glm;

struct ScanInfo {
    bool have_texture;
    bool have_normal;

    unsigned vertex_count;
    unsigned normal_count;
    unsigned tex_coord_count;
    unsigned face_count;
    void zero() { memset(this, 0, sizeof*this); }
};

#define OBJECT_TYPES(TYPE)                      \
    TYPE(OBJECT, "o ")                          \
    TYPE(VERTEX, "v ")                          \
    TYPE(TEX_COORD, "vt")                       \
    TYPE(NORMAL, "vn")                          \
    TYPE(FACE, "f ")                            \

#define TYPE_TO_ENUM(caps_, type_str_)          \
    OBJECT_##caps_,

enum object_type {
    OBJECT_TYPES(TYPE_TO_ENUM)
    MAX_OBJECT_TYPE
};

#define MATCH_OBJECT_TYPE(caps_, type_str_)     \
    do {                                        \
        if (type_str == type_str_)              \
            return OBJECT_##caps_;              \
    } while (0);

static int
get_object_type(string const &type_str)
{
    OBJECT_TYPES(MATCH_OBJECT_TYPE);
    return -1;
}

// returns the count of vertex in the face described in line

static unsigned
face_vertex_count(string const &line)
{
    char *face, *tok, *saveptr = NULL;
    unsigned count = 0;

    face = g_strdup(line.c_str());
    tok = strtok_r(face, " ", &saveptr);
    while (tok != NULL) {
        ++ count;
        tok = strtok_r(NULL, " ", &saveptr);
    }

    g_free(face);
    return count;
}

static int
scan_wavefront_file(GIOChannel *chan, ScanInfo &info)

{
    int err = 0;
    string line;
    info.zero();

    for (;!(err = d3v::readline(chan, line));) {
        if (line[0] == 'v') {
            switch (line[1]) {
            case ' ': ++ info.vertex_count; break;
            case 't': ++ info.tex_coord_count; break;
            case 'n': ++ info.normal_count; break;
            default: return -1; break;
            }
        } else if (line[0] == 'f' && line[1] == ' ') {
            if (face_vertex_count(line.substr(2,string::npos)) != 3)  {
                d3v_error(_("All faces must be triangles."));
                return -1;
            }
            ++ info.face_count;
        }
    }

    if (info.vertex_count)
        info.have_texture = true;
    if (info.normal_count)
        info.have_normal = true;

    return 0;
}

static int
parse_vertex(vec3 *vertex, string const &rep)
{
    vector<string> v;
    boost::split(v, rep, boost::is_any_of(" "));

    if (v.size() != 3)
        return -1;
    vertex->x = g_ascii_strtod(v[0].c_str(), NULL);
    vertex->y = g_ascii_strtod(v[1].c_str(), NULL);
    vertex->z = g_ascii_strtod(v[2].c_str(), NULL);
    return 0;
}

static void
parse_tex_coord(vec2 *tex_coord, string const &rep)
{
    vector<string> v;
    boost::split(v, rep, boost::is_any_of(" "));

    if (v.size() != 2)
        throw exception("tex_coord invalid coord count");
    tex_coord->x = g_ascii_strtod(v[0].c_str(), NULL);
    tex_coord->y = g_ascii_strtod(v[1].c_str(), NULL);
    tex_coord->y = 1 - tex_coord->y;
}

static void
parse_normal(vec3 *normal, string const &rep)
{
    vector<string> v;
    boost::split(v, rep, boost::is_any_of(" "));

    if (v.size() != 3)
        throw exception("invalid normal count "+std::to_string(v.size()));
    
    normal->x = g_ascii_strtod(v[0].c_str(), NULL);
    normal->y = g_ascii_strtod(v[1].c_str(), NULL);
    normal->z = g_ascii_strtod(v[2].c_str(), NULL);
}

static void
parse_vertex_elements(string const &rep, vector<Vertex> &vertices,
                      vec3 *vertex, vec2 *tex_coord, vec3 *normal)
{
    vector<string> v;
    boost::split(v, rep, boost::is_any_of("/"));

    Vertex w;
    if (!v.size())
        throw exception("face element bad size");
    
    {
        int i = atoi(v[0].c_str())-1;
        w.x = vertex[i].x;
        w.y = vertex[i].y;
        w.z = vertex[i].z;
    }

    boost::trim(v[1]);
    if (v.size() >= 2 && v[1] != "") {
        int i = atoi(v[1].c_str())-1;
        w.s = tex_coord[i].x;
        w.t = tex_coord[i].y;
    }
    if (v.size() == 3) {
        int i = atoi(v[2].c_str())-1;
        w.nx = normal[i].x;
        w.ny = normal[i].y;
        w.nz = normal[i].z;
    }
    vertices.push_back(w);
}

static void parse_face(string const &rep, std::vector<Vertex> &vertices,
                       vec3 *vertex, vec2 *tex_coord, vec3 *normal)
{
    vector<string> v;
    boost::split(v, rep, boost::is_any_of(" "));

    if (v.size() != 3)
        throw exception("parse_face not triangle: "+std::to_string(v.size()));

    parse_vertex_elements(v[0], vertices, vertex, tex_coord, normal);
    parse_vertex_elements(v[1], vertices, vertex, tex_coord, normal);
    parse_vertex_elements(v[2], vertices, vertex, tex_coord, normal);
}

#define LINE_IS_COMMENT(line) ((line)[0] == '#')

void Model::LoadObjects(GIOChannel *chan)
{
    ScanInfo info;

    int err = scan_wavefront_file(chan, info);
    if (err)
        throw exception(string("scanning error: ")+ m_filePath.c_str());
    
    g_io_channel_seek_position(chan, 0, G_SEEK_SET, NULL);

    vec3 *vertex, *normal;
    vec2 *tex_coord;
    vertex = new vec3[info.vertex_count];
    normal = new vec3[info.normal_count];
    printf("normal_count: %d\n", info.normal_count);
    tex_coord = new vec2[info.tex_coord_count];

    m_haveTexture = info.have_texture;
    m_haveNormal = info.have_normal;
    printf(_("scanned %d faces\n"), info.face_count);

    unsigned v, vn, vt;
    v = vn = vt =  0;

    string line;
    while (!(err = d3v::readline(chan, line))) {
        string type_str, value;

        if (LINE_IS_COMMENT(line))
            continue;
        if (line.length() < 2)
            throw exception("wavefront invalid line");

        type_str = line.substr(0, 2);
        value = line.substr(2, string::npos);
        boost::trim(value);

        switch (get_object_type(type_str)) {
        case OBJECT_VERTEX:
            parse_vertex(&vertex[v++], value);
            break;
        case OBJECT_TEX_COORD:
            parse_tex_coord(&tex_coord[vt++], value);
            break;
        case OBJECT_NORMAL:
            parse_normal(&normal[vn++], value);
            break;
        case OBJECT_FACE:
            parse_face(value, m_vertices, vertex, tex_coord, normal);
            break;
        case OBJECT_OBJECT:
            printf(_("parsing object \"%s\".\n"), value.c_str());
            break;
        default:
            d3v_warning(_("unexpected object type '%s'.\n"), type_str.c_str());
            break;
        }
    }
    printf(_("parsed %d faces\n"), m_vertices.size());
    delete [] vertex;
    delete [] normal;
    delete [] tex_coord;
}

void Model::MakeVAO(ShaderProgram const &program)
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(),
                 &m_vertices[0], GL_STATIC_DRAW);

    int p = program.GetPositionLocation();
    glEnableVertexAttribArray(p);
    glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

    if (m_haveNormal) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid*) G_STRUCT_OFFSET(Vertex, nx));
    }
    if (m_haveTexture) {
        int t = program.GetTexCoordLocation();
        glEnableVertexAttribArray(t);
        glVertexAttribPointer(t, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid*) G_STRUCT_OFFSET(Vertex, s));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::Model(std::string const &file_path,
             std::string const &model_name,
             ShaderProgram const &program):
    m_filePath(file_path), m_modelName(model_name)
{
    int err = 0;
    GIOChannel *chan = g_io_channel_new_file(file_path.c_str(), "r", NULL);
    if (chan == NULL) {
        perror(file_path.c_str());
        throw exception("cannot open file: "+file_path);
    }
    LoadObjects(chan);
    g_io_channel_unref(chan);
    MakeVAO(program);
}

void Model::Draw(ShaderProgram const &program)
{
    glBindVertexArray(m_vaoID);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

Model::~Model()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
}

void Model::Dump(std::ostream &out)
{
    unsigned i = 0;
    for (auto v : m_vertices) {
        out << boost::format("v%*u : %f %f %f\n") % 5 % i % v.x % v.y % v.z;
        ++ i;
    }
}
