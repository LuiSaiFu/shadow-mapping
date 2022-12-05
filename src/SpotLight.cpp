#include "SpotLight.h"

unsigned int SpotLight::depthMapFBO = 0;

void SpotLight::staticInit(void) {
	glGenFramebuffers(1, &SpotLight::depthMapFBO);
}

void SpotLight::init(void) {
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Cez = glm::normalize(glm::vec3(direction));
	glm::vec3 Cey = glm::normalize(up - glm::dot(Cez, up) * Cez);
	glm::vec3 Cex = glm::cross(Cey, Cez);
	glm::mat4 C;
	C[0] = glm::vec4(Cex, 0.0f);
	C[1] = glm::vec4(Cey, 0.0f);
	C[2] = glm::vec4(Cez, 0.0f);
	C[3] = glm::vec4(glm::vec3(position), 1.0f);
	view = glm::inverse(C);

	proj = glm::mat4(1.0f);
	float fovy_rad = light_angle * M_PI / 180.0f;
	float tan = glm::tan(fovy_rad / 2.0f);
	proj = glm::mat4(1.0f / tan, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / tan, 0.0f, 0.0f,
		0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f,
		0.0f, 0.0f, -2.0f * zFar * zNear / (zFar - zNear), 0.0f);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SpotLight::renderDepth(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
}