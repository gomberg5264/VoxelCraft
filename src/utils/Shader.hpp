#pragma once

/**
 * Simple wrapper around a shader to compile and display errors
 */
class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    
    void Use();

private:
    void checkCompileErrors(unsigned int shader, std::string type);

    unsigned int m_ID;
};