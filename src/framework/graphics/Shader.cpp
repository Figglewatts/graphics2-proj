#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "framework/util/Logger.h"

Shader::Shader(void)
{
	m_name = "";
	m_vertexShader = 0;
	m_fragmentShader = 0;
	m_programObject = 0;
}

Shader::~Shader(void)
{
	glUseProgram(0);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_programObject);
}


bool Shader::load(const std::string name, const char* filename)
{
	m_name = name;
	GLint success = 0;
	
	//here we create a vertex shader and set the shader source
	m_vertexShader = loadShader(filename, GL_VERTEX_SHADER);
	
	//next compile the vertex shader
	glCompileShader(m_vertexShader);
	
	//check if it compiled ok.
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		LOG(LogLevel::ERR,"Error compiling vertex shader: %s", filename);
		LOG(LogLevel::ERR, "Shader info log: \n%s", shaderInfoLog(m_vertexShader).c_str());
		
		return false;
	}

	else
	{
		//create a fragment shader and set the shader source
		m_fragmentShader = loadShader(filename, GL_FRAGMENT_SHADER);

		//compile the fragment shader
		glCompileShader(m_fragmentShader);

		//check for any compiling errors
		glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);

		if(!success)
		{
			LOG(LogLevel::ERR,"Error compiling fragment shader: %s", filename);
			LOG(LogLevel::ERR, "Shader info log: \n%s", shaderInfoLog(m_fragmentShader).c_str());
			
			return false;
		}
        
		else
		{
			//create the program
			m_programObject = glCreateProgram();

			//attach the vertex and fragment shaders
			glAttachShader(m_programObject, m_vertexShader);
			glAttachShader(m_programObject, m_fragmentShader);

			//link it all together
			glLinkProgram(m_programObject);

			//check for any errors with the shader program
			glGetProgramiv(m_programObject, GL_LINK_STATUS, &success);
        
			if(!success)
			{
				LOG(LogLevel::ERR, "Error linking shader %s", filename);
				LOG(LogLevel::ERR, "Program info log: \n%s", programInfoLog(m_programObject).c_str());
				
				return false;
			}
		}
    }

	LOG(LogLevel::INFO, "Loaded GLSL program: %s", m_name.c_str());

	return true;
}

void Shader::use() const
{
	glUseProgram(this->m_programObject);
}

//reads the shader from a file and defines the shader source
GLuint Shader::loadShader(const char* filename, GLenum type) const
{
	std::ifstream file(filename, std::ios::binary);
	
	if(!file.is_open())
		return -1;

	std::stringstream buffer;
	buffer << "#version 320 es\n";
	switch (type)
	{
		case GL_VERTEX_SHADER:
		{
			buffer << "#define vert_main main\n";
		} break;
		case GL_FRAGMENT_SHADER:
		{
			buffer << "#define frag_main main\n";
		} break;
	}
	std::string shaderSrc(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));

	buffer << shaderSrc;

	std::string buffStr = buffer.str();

	//LOG(LogLevel::INFO, buffer.str().c_str());

	const char *src_str = buffStr.c_str();

	GLuint shader = glCreateShader(type);

	LOG(LogLevel::INFO, src_str);

	glShaderSource(shader, 1, &src_str, 0);

	return shader;
}

//Errors and other info for the shaders and the program are obtained
//with the following functions.
std::string Shader::shaderInfoLog(const GLuint shader) const
{
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	std::vector<char> log(length);
 
	glGetShaderInfoLog(shader, length, &length, log.data());

	std::string retLog(begin(log), end(log));
	
	return retLog;
}


std::string Shader::programInfoLog(const GLuint program) const
{
	GLsizei infoLogSize = 0;
	std::string infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);
	infoLog.resize(infoLogSize);
	glGetProgramInfoLog(program, infoLogSize, &infoLogSize, &infoLog[0]);

	return infoLog;
}