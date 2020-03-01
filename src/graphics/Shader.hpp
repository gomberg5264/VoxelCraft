#pragma once

/**
 * Simple wrapper around a shader to compile and display errors
 */
class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    
    void Use() const;
    void SetInt(const char* name, int val) const;
    void SetFloat(const char* name, float val) const;
    void SetBool(const char* name, bool val) const;
    void SetMatrix(const char* name, const float* val) const;

private:
    unsigned int m_ID;
};