#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <glib/gstdio.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <cstdint>
#include "d3v/texture.hpp"

using d3v::Texture;

/** Chargement d'une image au format JPG **/
void Texture::LoadJpeg(std::string const &file_path, int &width, int &height)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    uint8_t *line;

    FILE *f = g_fopen(file_path.c_str(), "r");
    if (f == NULL) {
	perror(file_path.c_str());
	exit(EXIT_FAILURE);
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, f);
    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
	fprintf(stdout, "Erreur: %s doit etre de type RGB\n", file_path.c_str());
	exit(EXIT_FAILURE);
    }

    width = cinfo.image_width;
    height = cinfo.image_height;
    m_textureData.resize(height * width * 3);

    jpeg_start_decompress(&cinfo);
    line = &m_textureData[0];
    while (cinfo.output_scanline < cinfo.output_height) {
	line = &m_textureData[0] + 3 * width * cinfo.output_scanline;
	jpeg_read_scanlines(&cinfo, &line, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(f);
}

Texture::Texture(std::string const &path, ShaderProgram const &program)
{
    glGenTextures(1, &m_texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    int width, height;
    LoadJpeg(path, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		 0, GL_RGB, GL_UNSIGNED_BYTE, &m_textureData[0]);
    int s = program.GetSamplerLocation();
    glUniform1i(s, GL_TEXTURE0);
}

GLuint Texture::GetID()
{
    return m_texID;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texID);
}
