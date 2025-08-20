#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

struct TessellationShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
    std::string TessControlSource;
    std::string TessEvaluationSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    // Regular shader constructor (vertex + fragment)
    Shader(const std::string& filepath) {
        ShaderProgramSource source = ParseShader(filepath);
        m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    }

    // Tessellation shader constructor
    Shader(const std::string& vertexPath, const std::string& fragmentPath,
        const std::string& tcsPath, const std::string& tesPath) {
        m_RendererID = CreateTessellationShader(vertexPath, fragmentPath, tcsPath, tesPath);
    }

    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, glm::mat4& matrix);
    void setUniformMatrix4fv(const std::string& name, int count, bool transpose, glm::mat4 matrix);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    // MVP convenience method
    void setMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
        glm::mat4 mvp = projection * view * model;
        setUniformMat4f("u_MVP", mvp);
    }

private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    std::string LoadShaderFromFile(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CreateTessellationShader(const std::string& vertexPath, const std::string& fragmentPath,
        const std::string& tcsPath, const std::string& tesPath);
    int GetUniformLocation(const std::string name);
};