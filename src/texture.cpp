#include "..\include\texture.h"

#include <SOIL.h>

void Texture::init(const char* filename) {
	glGenTextures(1, &tex);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, tex);

	utils::dimensions d = utils::png_dimensions(filename);

	unsigned char* image = SOIL_load_image(filename, &d.width, &d.height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d.width, d.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::use() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
}


void Texture::destroy() {
	glDeleteTextures(1, &tex);
}