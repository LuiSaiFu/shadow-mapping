#include "SpotLight.h"
#include <iostream>

unsigned int SpotLight::depthMapFBO = 0;
unsigned int SpotLight::depthMap = 0;

void SpotLight::staticInit(int count) {
	glGenFramebuffers(1, &SpotLight::depthMapFBO);

	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, SpotLight::WIDTH, SpotLight::HEIGHT, count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	
}

void SpotLight::init(void) {
	if (position.w == 0.0f)
		direction = -position;
	glm::vec3 Cez = glm::normalize(glm::vec3(-direction));
	glm::vec3 up = glm::vec3(Cez.y, Cez.z, Cez.x);
	glm::vec3 Cey = glm::normalize(up - glm::dot(Cez, up) * Cez);
	glm::vec3 Cex = glm::cross(Cey, Cez);
	glm::mat4 C;
	C[0] = glm::vec4(Cex, 0.0f);
	C[1] = glm::vec4(Cey, 0.0f);
	C[2] = glm::vec4(Cez, 0.0f);
	C[3] = glm::vec4(glm::vec3(position), 1.0f);
	view = glm::inverse(C);

	if (position.w == 0.0f)
	{
		proj = glm::mat4(2.0f / size, 0.0f, 0.0f, 0.0f, 
			0.0f, 2.0f/size, 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f/depth, -1.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		proj = glm::mat4(1.0f);
		float fovy_rad = light_angle * M_PI / 180.0f;
		float tan = glm::tan(fovy_rad / 2.0f);
		proj = glm::mat4(1.0f / tan, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / tan, 0.0f, 0.0f,
			0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f,
			0.0f, 0.0f, -2.0f * zFar * zNear / (zFar - zNear), 0.0f);
	}
}

void SpotLight::prepareRenderDepth(int layer) {
	glViewport(0, 0, WIDTH, HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0, layer);

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
}