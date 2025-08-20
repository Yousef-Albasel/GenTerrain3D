#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "Window.h"
#include <glm/gtc/type_ptr.hpp>

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    enum class ShaderType { // enum to determine the index in the string stream array
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::fstream stream;
    stream.open(filepath);
    if (!stream.is_open()) {
        std::cout << "Error: File is not opened: " << filepath << std::endl;
        return{ "","" };
    }

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                //set mode to vertex
                type = ShaderType::VERTEX;

            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    stream.close();
    return { ss[0].str(),ss[1].str() };
}

std::string Shader::LoadShaderFromFile(const std::string& filepath) {
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cout << "Error: Cannot open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();

    return buffer.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // allocate on stack dynamically

        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::string shaderType;
        switch (type) {
        case GL_VERTEX_SHADER: shaderType = "Vertex"; break;
        case GL_FRAGMENT_SHADER: shaderType = "Fragment"; break;
        case GL_TESS_CONTROL_SHADER: shaderType = "Tessellation Control"; break;
        case GL_TESS_EVALUATION_SHADER: shaderType = "Tessellation Evaluation"; break;
        default: shaderType = "Unknown"; break;
        }

        std::cout << "Failed to compile " << shaderType << " shader: " << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // These strings are the source code for each shader
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    int linkStatus;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

    if (linkStatus == GL_FALSE) {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, message));

        std::cerr << "Failed to link shader program: " << message << std::endl;
    }

    glValidateProgram(program);

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
// Replace your CreateTessellationShader method with this fixed version:

unsigned int Shader::CreateTessellationShader(const std::string& vertexPath, const std::string& fragmentPath,
    const std::string& tcsPath, const std::string& tesPath) {

    // Load shader sources from files - use ParseShader for combined vertex/fragment file
    ShaderProgramSource vertexFragmentSource = ParseShader(vertexPath);
    std::string tcsSource = LoadShaderFromFile(tcsPath);
    std::string tesSource = LoadShaderFromFile(tesPath);

    // Validate that all sources were loaded
    if (vertexFragmentSource.VertexSource.empty()) {
        std::cout << "Failed to load vertex shader from: " << vertexPath << std::endl;
        return 0;
    }
    if (vertexFragmentSource.FragmentSource.empty()) {
        std::cout << "Failed to load fragment shader from: " << vertexPath << std::endl;
        return 0;
    }
    if (tcsSource.empty()) {
        std::cout << "Failed to load tessellation control shader from: " << tcsPath << std::endl;
        return 0;
    }
    if (tesSource.empty()) {
        std::cout << "Failed to load tessellation evaluation shader from: " << tesPath << std::endl;
        return 0;
    }

    // Create program
    unsigned int program = glCreateProgram();

    // Compile all shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexFragmentSource.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, vertexFragmentSource.FragmentSource);
    unsigned int tcs = CompileShader(GL_TESS_CONTROL_SHADER, tcsSource);
    unsigned int tes = CompileShader(GL_TESS_EVALUATION_SHADER, tesSource);

    // Check if all shaders compiled successfully
    if (vs == 0 || fs == 0 || tcs == 0 || tes == 0) {
        std::cout << "Failed to compile one or more tessellation shaders!" << std::endl;
        if (vs == 0) std::cout << "  - Vertex shader compilation failed" << std::endl;
        if (fs == 0) std::cout << "  - Fragment shader compilation failed" << std::endl;
        if (tcs == 0) std::cout << "  - Tessellation control shader compilation failed" << std::endl;
        if (tes == 0) std::cout << "  - Tessellation evaluation shader compilation failed" << std::endl;

        GLCall(glDeleteProgram(program));
        return 0;
    }

    // Attach all shaders
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glAttachShader(program, tcs));
    GLCall(glAttachShader(program, tes));

    // Link program
    GLCall(glLinkProgram(program));

    // Check link status
    int linkStatus;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

    if (linkStatus == GL_FALSE) {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, message));

        std::cerr << "Failed to link tessellation shader program: " << message << std::endl;

        // Clean up and return 0
        GLCall(glDeleteProgram(program));
        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));
        GLCall(glDeleteShader(tcs));
        GLCall(glDeleteShader(tes));
        return 0;
    }

    // Validate program
    glValidateProgram(program);

    // Clean up individual shaders
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    GLCall(glDeleteShader(tcs));
    GLCall(glDeleteShader(tes));

    std::cout << "Tessellation shader program created successfully!" << std::endl;
    return program;
}
int Shader::GetUniformLocation(const std::string name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "Warning: Uniform " << name << " doesn't exist." << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::setUniformMat4f(const std::string& name, glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniformMatrix4fv(const std::string& name, int count, bool transpose, glm::mat4 matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), count, transpose, glm::value_ptr(matrix));
}