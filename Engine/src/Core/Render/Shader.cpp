#include "spch.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Six {
    static void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
	SShader::SShader(std::string vert, std::string frag) {
        std::string vertexShaderSource;
        std::string fragmentShaderSource;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vert);
            fShaderFile.open(frag);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexShaderSource = vShaderStream.str();
            fragmentShaderSource = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vss = vertexShaderSource.c_str();
        const char* fss = fragmentShaderSource.c_str();
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vss, nullptr);
        glCompileShader(vs);
        checkCompileErrors(vs, "VERTEX");

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fss, nullptr);
        glCompileShader(fs);
        checkCompileErrors(fs, "FRAGMENT");

        m_id = glCreateProgram();
        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);
        checkCompileErrors(m_id, "PROGRAM");
        glDeleteShader(vs);
        glDeleteShader(fs);
	}
    void SShader::Bind() const{
        glUseProgram(m_id);
    }
    void SShader::Unbind() const{
        glUseProgram(0);
    }

    void SShader::SetUniform(const std::string& name, int v) {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()),v);
    }

    void SShader::SetUniform(const std::string& name, float v) {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), v);
    }

    void SShader::SetUniform(const std::string& name, bool v) {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), v);
    }

    void SShader::SetUniform(const std::string& name, float v1,float v2) {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), v1,v2);
    }

    void SShader::SetUniform(const std::string& name, float v1,float v2,float v3) {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), v1,v2,v3);
    }

    void SShader::SetUniform(const std::string& name, float v1, float v2, float v3,float v4) {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), v1, v2, v3,v4);
    }

    void SShader::SetUniform(const std::string& name, glm::vec2 v) {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), v.x, v.y);
    }
    void SShader::SetUniform(const std::string& name, glm::vec3 v) {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), v.x, v.y,v.z);
    }
    void SShader::SetUniform(const std::string& name, glm::vec4 v) {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), v.x, v.y,v.z,v.w);
    }
    void SShader::SetUniform(const std::string& name, glm::mat4 m) {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    }
	void SShader::SetUniform(const std::string& name, STexture* texture, int slot) {
		texture->Bind(slot);
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), slot);
	}
}