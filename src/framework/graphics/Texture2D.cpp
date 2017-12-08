#include "Texture2D.h"
#include <vector>
#include <lodepng.h>

#include "framework/util/Logger.h"

namespace Framework
{
	Texture2D::Texture2D(const std::string & filePath, GLint format, GLenum type, GLint minFilter, GLint magFilter, GLint sWrap, GLint tWrap)
	{
		std::vector<unsigned char> imgBuff;
		unsigned err = lodepng::decode(imgBuff, _width, _height, filePath);
		if (err != 0)
		{
			LOG(LogLevel::ERR, "ERROR %d, Could not load texture %s", err, filePath.c_str());
			throw std::runtime_error("Error loading texture");
		}

		std::size_t row_size = _width * 4; // 32-bit RGBA png
		std::size_t num_bytes = _width * _height * 4;
		std::size_t num_rows = num_bytes / row_size;

		// flip the texture vertically for OpenGL
		for (int i = 0; i < num_rows / 2; i++)
		{
			for (int j = 0; j < row_size; j++)
			{
				unsigned char temp = imgBuff[((num_rows - i - 1) * row_size) + j];
				imgBuff[((num_rows - i - 1) * row_size) + j] = imgBuff[(i * row_size) + j];
				imgBuff[(i * row_size) + j] = temp;
			}
		}
		
		glGenTextures(1, &_handle);
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuff.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glGenerateMipmap(GL_TEXTURE_2D);
		unbind();
	}

	void Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, this->_handle);
	}

	void Texture2D::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
