#pragma once
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glm/glm.hpp"
#include <unordered_map>
#include <iostream>


struct ShaderProgramSource {
    std::string   VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    const std::string path;
    unsigned int CompileShader(unsigned int type, const std::string& source);
    std::unordered_map<std::string, int> m_UniformLocationCache;


public:
    unsigned int m_RendererID;

     Shader(const std::string& filepath) :path(filepath),m_RendererID(0) {
         ShaderProgramSource source = ParseShader(filepath);
         m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
         std::cout << "Used Program : " << m_RendererID << std::endl;
     };
     ShaderProgramSource ParseShader(const std::string& filepath);
     unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
     ~Shader();

     void Bind() const;
     void Unbind() const;

     int GetUniformLocation(const std::string name);
     void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
     void SetUniform1f(const std::string& name, float value);
     void SetUniform1i(const std::string& name, int value);
     void setUniformMat4f(const std::string& name, glm::mat4& matrix);
     void setUniformMatrix4fv(const std::string& name ,int count,bool transpose, glm::mat4 matrix);
     void SetUniform3f(const std::string& name, float v0, float v1, float v2);
     void PrintShaderInfo(){
         std::cout << path << std::endl;
     }
};

#endif