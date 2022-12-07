#include "Shader.h"

#ifndef __SCREENSHADER_H__
#define __SCREENSHADER_H__

struct ScreenShader : Shader {
	float near_dist = 0.01f;
	float far_dist = 1.0f;
	int ind = 0;

	GLuint near_loc;
	GLuint far_loc;
	GLuint ind_loc;

	void initUniforms() {
		near_loc = glGetUniformLocation(program, "near");
		far_loc = glGetUniformLocation(program, "far");
		ind_loc = glGetUniformLocation(program, "ind");
	}
	void setUniforms() {
		glUniform1f(near_loc, near_dist);
		glUniform1f(far_loc, far_dist);
		glUniform1i(ind_loc, ind);
	}
};

#endif