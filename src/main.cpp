#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v.hpp"

#define WINDOW_X 200
#define WINDOW_Y 200
#define HEIGHT  700
#define WIDTH   700

using namespace d3v;

int window_key;
static Scene *gl_scene;

static void
d3v_set_scene(Scene *scene)
{
    gl_scene = scene;
}

static void draw_basis(void)
{
    glNormal3f(0., 1., 0.);
    glBegin(GL_LINES);

    glColor3f(1., 0., 0.);	// x red
    glVertex3f(0., 0., 0.);
    glVertex3f(1., 0., 0.);

    glColor3f(.0, 1., 0.);	// y green
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 1., 0.);

    glColor3f(0., 0., 1.);	// z blue
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 0., 1.);
    glEnd();
    glColor3f(1., 1., 1.);	// reset color
}

static void draw_cb(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draw_basis();

    if (gl_scene->m_objects.size())
        gl_scene->m_camera.Update(*gl_scene->m_objects[0]->GetProgram());
    //gl_scene->m_light.Update();
    for (auto obj : gl_scene->m_objects)
	obj->Draw();

    glutSwapBuffers();
}

static void key_input_cb(unsigned char key, int x, int y)
{
    switch (key) {
    case 27: /* touche ESC */
	glutLeaveMainLoop();
	break;
    }
}

static void
mouse_cb(int button, int state, int x, int y)
{
    gl_scene->m_xold = x; gl_scene->m_yold = y;
    switch (button) {
    case GLUT_LEFT_BUTTON:
	switch (state) {
	case GLUT_DOWN: gl_scene->m_button = 1; break;
	case GLUT_UP:
	    gl_scene->m_button = 0;
	    vec3 pos;  MouseProjection(pos, x , y);
	    printf("%f %f %f\n", pos.x, pos.y, pos.z);
	    break;
	} break;
    case GLUT_RIGHT_BUTTON:
	switch (state) {
	case GLUT_DOWN: gl_scene->m_button = 2; break;
	case GLUT_UP:   gl_scene->m_button = 0; break;
	} break;
    case 3: gl_scene->m_camera.AddDistance(-0.4); break;
    case 4: gl_scene->m_camera.AddDistance(0.4);  break;
    }
    glutPostRedisplay();
}

static void
mouse_motion_cb(int x, int y)
{
    if (gl_scene->m_button == 1)// left button -> translate
    {
	gl_scene->m_camera.Translate(
            (double) (gl_scene->m_xold - x) / 70.,
            (double) (y - gl_scene->m_yold) / 70.);
    }
    else if (gl_scene->m_button == 2) // right button -> rotate
    {
	gl_scene->m_camera.Rotate(
            (double) -(y - gl_scene->m_yold) / 70,
            (double) -(x - gl_scene->m_xold) / 70);
    }

    gl_scene->m_xold = x;
    gl_scene->m_yold = y;
    glutPostRedisplay();
}

static void
d3v_glut_init(const char *title, int *argc, char **argv[])
{
    glutInit(argc, *argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
    
    window_key = glutCreateWindow(title);

    glutDisplayFunc(&draw_cb);
    glutKeyboardFunc(&key_input_cb);
    glutMouseFunc(&mouse_cb);
    glutMotionFunc(&mouse_motion_cb);
}

static void
d3v_opengl_init(void)
{
    glClearColor(.5, .5, .5, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

static void
d3v_init(int *argc, char **argv[], char const *window_title)
{
    d3v_glut_init(window_title, argc, argv);
    d3v_opengl_init();
}

static void init_locale(void)
{
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain(PACKAGE);
}

int main(int argc, char *argv[])
{
    init_locale();

    if (argc != 2) {
	fprintf(stderr, _("Usage: %s penguin_count\n"), argv[0]);
	exit(EXIT_FAILURE);
    }
    d3v_init(&argc, &argv, _("penguins"));
    
    Scene mainScene;    
    Model *penguinModel;
    Texture *penguinTexture;
    Shader *vertexShader, *fragmentShader;
    ShaderProgram *program;
    
    vertexShader = Shader::VertexFromResource("/org/d3v/vertex.glsl");
    fragmentShader = Shader::FragmentFromResource("/org/d3v/fragment.glsl");
    program = new ShaderProgram(vertexShader, fragmentShader, NULL);
    penguinModel = new Model(OBJDIR "penguin.obj", _("penguin"), *program);
    penguinTexture = new Texture(TEXDIR "penguin_black.jpg");
    
    int n = atoi(argv[1]);
    vec3 pos = { 0., 0., 0. };
    for (int i = 0; i < n; ++i)
    {
        Object *penguin;
        pos.x = 0.3 * i;
        penguin = new Object(penguinModel, penguinTexture, pos, 90., 0.1);
        penguin->SetShaderProgram(program);
        
        penguin->SetShadeModel((i % 2) ? GL_FLAT : GL_SMOOTH);
        mainScene.AddObject(penguin);
    }

    d3v_set_scene(&mainScene);
    glutMainLoop();
    printf(_("Main loop exited ...\n"));
    delete penguinModel;
    delete program;
    return EXIT_SUCCESS;
}
