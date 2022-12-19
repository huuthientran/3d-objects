#pragma once

#define NO_TEXTURE 32

static GLuint textureIDList[NO_TEXTURE];
static const char* texturePathList[] = {
	"Data/texture0.jpg",
	"Data/texture1.jpg",
	"Data/texture2.jpg",
	"Data/texture3.jpg",
	"Data/texture4.jpg",
	"Data/texture5.jpg",
	"Data/texture6.jpg",
	"Data/texture7.jpg",
	"Data/texture8.jpg",
	"Data/texture9.jpg",
	"Data/texture10.jpg",
	"Data/texture11.jpg",
	"Data/texture12.jpg",
	"Data/texture13.jpg",
	"Data/texture14.jpg",
	"Data/texture15.jpg",
	"Data/texture16.jpg",
	"Data/texture17.jpg",
	"Data/texture18.jpg",
	"Data/texture19.jpg",
	"Data/texture20.jpg",
	"Data/texture21.jpg",
	"Data/texture22.jpg",
	"Data/texture23.jpg",
	"Data/texture24.jpg",
	"Data/texture25.jpg",
	"Data/texture26.jpg",
	"Data/texture27.jpg",
	"Data/texture28.jpg",
	"Data/texture29.jpg",
	"Data/texture30.jpg",
	"Data/texture31.jpg"
};

inline int loadGLTexture(const char* texturePath) // Load Bitmaps And Convert To Textures
{
	if (!texturePath) {
		return -1;
	}
	/* load an image file directly as a new OpenGL texture */
	GLuint textureID = SOIL_load_OGL_texture
	(
		texturePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (textureID == 0)
		return -1;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureID;
}

inline void loadGLTextures() {
	for (auto i = 0; i < NO_TEXTURE; ++i) {
		textureIDList[i] = loadGLTexture(texturePathList[i]);
	}
}