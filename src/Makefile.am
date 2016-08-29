include $(top_srcdir)/dirs.mk

bin_PROGRAMS = d3v

d3v_SOURCES = \
    mouse_projection.cpp \
    error.cpp \
    light.cpp \
    object.cpp \
    main.cpp \
    scene.cpp \
    model.cpp \
    camera.cpp \
    texture.cpp \
    shader.cpp \
    util.cpp

d3v_CXXFLAGS = \
    -Iinclude $(GLIB_CFLAGS) $(GLUT_CFLAGS) $(JPEG_CFLAGS) $(GIO_CFLAGS) \
    -DOBJDIR=\"$(objdir)\" \
    -DTEXDIR=\"$(texdir)\" \
    -DLOCALEDIR=\"$(localedir)\"

d3v_LDFLAGS = \
    $(GLIB_LIBS) $(GLUT_LIBS) $(JPEG_LIBS) $(GIO_LIBS) -lm -lGLU -lGL

nodist_d3v_SOURCES = \
	resources.cpp \
	resources.h

d3v.$(OBJEXT): resources.cpp resources.h

CLEANFILES = \
	resources.cpp \
	resources.h

GLIB_BUILD_RESOURCE = glib-compile-resources
RESOURCES_DEP= \
	$(srcdir)/fragment.glsl \
	$(srcdir)/vertex.glsl

$(srcdir)/d3v.gresources.xml: $(RESOURCES_DEP)
# technically nothing can be 'rebuilt' here so just update modification time
	touch $@

resources.h: $(srcdir)/d3v.gresources.xml
	$(GLIB_BUILD_RESOURCE) $(srcdir)/d3v.gresources.xml \
	--sourcedir=$(srcdir) \
	--target=$@ \
	--generate-header

resources.cpp: resources.h $(srcdir)/d3v.gresources.xml
	$(GLIB_BUILD_RESOURCE) $(srcdir)/d3v.gresources.xml \
	--sourcedir=$(srcdir) \
	--target=$@ \
	--generate-source

