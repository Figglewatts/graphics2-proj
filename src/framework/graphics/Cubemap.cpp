#include "Cubemap.h"
#include <lodepng.h>
#include "framework/util/Convenience.h"
#include "framework/util/Logger.h"

namespace Framework
{
	Cubemap::Cubemap(const std::string & filePath, GLint format, GLenum type, GLint minFilter, GLint magFilter)
	{
		glGenTextures(1, &_handle);
		bind();

		for (int i = 0; i < _sides.size(); i++)
		{
			std::vector<unsigned char> imgBuff;
			const std::string texName = removeExtension(filePath) + _sides[i] + ".png";
			unsigned err = lodepng::decode(imgBuff, _width, _height, texName);
			if (err != 0)
			{
				LOG(LogLevel::ERR, "ERROR %d, Could not load texture %s", err, texName.c_str());
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuff.data());
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, magFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		unbind();
	}

	void Cubemap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->_handle);
	}

	void Cubemap::unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
