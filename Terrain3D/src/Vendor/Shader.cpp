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
        std::cout << "Error: File is not opened" << std::endl;
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


 unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
     unsigned int id  = glCreateShader(type);
     const char* src = &source[0];
     GLCall(glShaderSource(id, 1, &src, nullptr));
     GLCall(glCompileShader(id));

     // To do: Some error handling
     int result;
     GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
     if (result == GL_FALSE) {
         int length;
         GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
         char* message = (char*)alloca(length * sizeof(char)); // allocate on stack dynamically

         GLCall(glGetShaderInfoLog(id, length, &length, message));
         std::cout << "Fail to Compile Shader: " << message << std::endl;
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
 int Shader::GetUniformLocation(const std::string name) {

     if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
         return m_UniformLocationCache[name];
     unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
     if (location == -1) {
         std::cout << "Warning:Uniform " << name << " Doesn't exist." << std::endl;
     }
     m_UniformLocationCache[name] = location;
     return location;
 }

 Shader::~Shader(){
     GLCall(glDeleteProgram(m_RendererID));
 };

 void Shader::Bind() const{
     GLCall(glUseProgram(m_RendererID));
 };
 void Shader::Unbind() const{
    GLCall(glUseProgram(0));
 };
 
 void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
     GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
 }

 void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
     GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
 }
 void Shader::SetUniform1f(const std::string& name, float value) {
     GLCall(glUniform1f(GetUniformLocation(name), value));
 };
 void Shader::SetUniform1i(const std::string& name, int value) {
     GLCall(glUniform1i(GetUniformLocation(name), value));

 };
 void Shader::setUniformMat4f(const std::string& name, glm::mat4& matrix) {
     GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
 }

 void Shader::setUniformMatrix4fv(const std::string& name, int count, bool transpose, glm::mat4 matrix) {
     glUniformMatrix4fv(GetUniformLocation(name), count, transpose, glm::value_ptr(matrix));
 };
