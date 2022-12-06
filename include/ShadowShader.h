#include <iostream>

#include "SurfaceShader.h"
#ifndef __SHADOWSHADER_H__
#define __SHADOWSHADER_H__

struct ShadowShader : SurfaceShader {
	std::vector<glm::mat4> lightVP;
	GLuint lightVP_loc;

	GLuint shadows_loc;

	void initUniforms() override {
		SurfaceShader::initUniforms();
		lightVP_loc = glGetUniformLocation(program, "lightVP");
		shadows_loc = glGetUniformLocation(program, "shadows");
	}
	void setUniforms() override {
		SurfaceShader::setUniforms();
		glUniformMatrix4fv(lightVP_loc, GLsizei(nlights), GL_FALSE, &lightVP[0][0][0]);
		glUniform1i(shadows_loc, 0);
	}
};

#endif