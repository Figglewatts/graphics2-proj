#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
	Shader(void);
	~Shader(void);
	
	//returns what we need for rendering
	GLuint handle(void) const { return m_programObject; } 

	//loads the shader program
	bool load(const std::string name, const char* filename);
	void use() const;

private:
	std::string m_name;
	GLuint m_vertexShader;       //identifier for the vertex shader
	GLuint m_fragmentShader;     //identifier for the fragment shader
	GLuint m_programObject;      //identifier for the program- this is used when rendering.
	GLuint loadShader(const char* filename, const GLenum type) const;
	std::string shaderInfoLog(const GLuint shader) const;
	std::string programInfoLog(const GLuint program) const;
};