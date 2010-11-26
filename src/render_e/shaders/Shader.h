/* 
 * File:   Shader.h
 * Author: morten
 *
 * Created on November 14, 2010, 10:07 PM
 */

#ifndef SHADER_H
#define	SHADER_H

namespace render_e {

enum ShaderLoadStatus {
    SHADER_OK,
    SHADER_CANNOT_ALLOCATE,
    SHADER_FILE_NOT_FOUND,
    SHADER_COMPILE_ERROR_VERTEX_SHADER,
    SHADER_COMPILE_ERROR_FRAGMENT_SHADER,
    SHADER_LINK_ERROR    
};

class Shader {
public:
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
    virtual ~Shader();
    ShaderLoadStatus Compile();
    ShaderLoadStatus Link();
    void Bind();
    void SetTexture(unsigned int index, unsigned int textureId);
    void SetVector3(unsigned int index, float *vector);
    void SetVector4(unsigned int index, float *vector);
    void SetMatrix44(unsigned int index, float *mat);
private:
    Shader(const Shader& orig); // disallow copy constructor
    Shader& operator = (const Shader&); // disallow copy constructor
    
    unsigned int shaderProgramId;
    unsigned int vertexShaderId;
    unsigned int fragmentShaderId;
    const char *vertexShaderSource;
    const char *fragmentShaderSource;
};
}
#endif	/* SHADER_H */
