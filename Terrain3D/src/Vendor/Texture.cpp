#include "GL/glew.h"
#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include "Window.h"
Texture::Texture(const char* path): filepath(path),m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0) {
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(stbi_set_flip_vertically_on_load(1));
    // set the texture wrapping/filtering options (on the currently bound texture object)
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // load and generate the texture
    data = stbi_load(path, &m_Width, &m_Height, &m_BPP, 0);
    if (data)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cout << "Failed to load texture : " << filepath << std::endl;
    }
    stbi_image_free(data);

};
Texture::~Texture(){
    GLCall(glDeleteTextures(1, &m_RendererID));

};
void Texture::Bind(unsigned int slot) const{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot))
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
};
void Texture::Unbind() const{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
};


