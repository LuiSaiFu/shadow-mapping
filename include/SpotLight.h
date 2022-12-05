#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct SpotLight {
	const unsigned int WIDTH = 1024, HEIGHT = 1024;
	static unsigned int depthMapFBO;


	glm::vec4 position = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 direction = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

	float light_angle = 30.0f;
	float zNear = 0.01f;
	float zFar  = 100.0f;

	unsigned int depthMap;
	glm::mat4 view;
	glm::mat4 proj;

	static void staticInit( void );
	void init( void );
	void renderDepth( void );
};

#endif 
