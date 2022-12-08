/**************************************************
Scene.inl contains the definition of the scene graph
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"

using namespace glm;
void Scene::init(int w, int h) {
	width = w;
	height = h;

    // Create a geometry palette
    geometry["cube"] = new Cube;
    geometry["bunny"] = new Obj;
    geometry["cube"]->init();
    geometry["bunny"]->init("models/bunny.obj");
    
    // Create a material palette    
    material["ceramic"] = new Material;
    material["ceramic"]->ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    material["ceramic"]->diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    material["ceramic"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["ceramic"]->shininess = 70.0f;
 
    material["gold"] = new Material;
    material["gold"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["gold"]->diffuse = vec4(0.4f, 0.12f, 0.0f, 1.0f);
    material["gold"]->specular = vec4(0.9f, 0.8f, 0.05f, 1.0f);
    material["gold"]->shininess = 150.0f;

	material["bulb"] = new Material;
	material["bulb"]->ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material["bulb"]->diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material["bulb"]->specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	material["bulb"]->emision = vec4(1.0f, 0.2f, 0.1f, 1.0f);
	material["bulb"]->shininess = 200.0f;
    
    // Create a model palette
	model["brick piece"] = new Model;
	model["brick piece"]->geometry = geometry["cube"];
	model["brick piece"]->material = material["ceramic"];
    model["teapot"] = new Model;
    model["teapot"]->geometry = geometry["teapot"];
    model["teapot"]->material = material["gold"];
    model["bunny"] = new Model;
    model["bunny"]->geometry = geometry["bunny"];
    model["bunny"]->material = material["gold"];

	model["bulb"] = new Model;
	model["bulb"]->geometry = geometry["cube"];
	model["bulb"]->material = material["bulb"];
    
    // Create a light palette
    light["bulb"] = new SpotLight;
    light["bulb"]->position = vec4(1.5f,4.0f,1.5f,1.0f);
	light["bulb"]->color = 3.0f * vec4(1.0f, 1.0f, 0.8f, 1.0f);
	light["bulb"]->direction = vec4(-1.0f, -1.0f, -0.5f, 0.0f);
	light["bulb"]->zNear = 2.0f;
	light["bulb"]->zFar = 6.0f;
	light["bulb"]->light_angle = 120.0f;
	light["bulb"]->ignore.insert(model["bulb"]);

	node["world"]->models.push_back(model["bulb"]);
	node["world"]->modeltransforms.push_back( translate(vec3(light["bulb"]->position))*scale(vec3(0.1f)) );
	

	light["bulb2"] = new SpotLight;
	light["bulb2"]->position = vec4(-2.0f, 3.0f, 4.0f, 1.0f);
	light["bulb2"]->color = 4.0f * vec4(1.0f, 1.0f, 0.8f, 1.0f);
	light["bulb2"]->direction = vec4(1.0f, -1.0f, -1.0f, 0.0f);
	light["bulb2"]->zNear = 2.0f;
	light["bulb2"]->zFar = 6.0f;
	light["bulb2"]->light_angle = 120.0f;
	light["bulb2"]->ignore.insert(model["bulb"]);

	node["world"]->models.push_back(model["bulb"]);
	node["world"]->modeltransforms.push_back(translate(vec3(light["bulb2"]->position)) * scale(vec3(0.1f)));

    // Build the scene graph
    node["corner"] = new Node;
    
	node["corner"]->models.push_back(model["brick piece"]);
	node["corner"]->modeltransforms.push_back(scale(vec3(6.0f, 0.1f, 6.0f)));
	node["corner"]->models.push_back(model["brick piece"]);
	node["corner"]->modeltransforms.push_back(translate(vec3(-1.25f, 1.0f, 0.0f)) * rotate(float(M_PI) / 4, vec3(0.0f, 1.0f, 0.0f)) * rotate(float(M_PI)/2, vec3(1.0f, 0.0f, 0.0f)) * scale(vec3(4.0f, 0.1f, 4.0f)));
	node["corner"]->models.push_back(model["brick piece"]);
	node["corner"]->modeltransforms.push_back(translate(vec3(1.25f, 1.0f, 0.0f)) * rotate(float(M_PI) / 4, vec3(0.0f, 1.0f, 0.0f)) * rotate(float(M_PI) / 2, vec3(0.0f, 0.0f, 1.0f)) * scale(vec3(4.0f, 0.1f, 4.0f)));

    node["world"]->childnodes.push_back( node["corner"] );
    node["world"]->childtransforms.push_back( mat4(1.0f) );
	node["world"]->models.push_back(model["bunny"]);
	node["world"]->modeltransforms.push_back(translate(vec3(0.0f, 1.0f, 1.0f)));
   
    
    // Put a camera
    camera = new Camera;
    camera->target_default = vec3( 0.0f, 1.0f, 0.0f );
    camera->eye_default = vec3( 0.0f, 1.0f, 5.0f );
    camera->up_default = vec3( 0.0f, 1.0f, 0.0f );
    camera->reset();
    
    // Initialize shader
	depthShader = new SurfaceShader;
	depthShader->read_source("shaders/depth.vert", "shaders/depth.frag");
	depthShader->compile();
	glUseProgram(depthShader->program);
	depthShader->initUniforms();

	screenShader = new ScreenShader;
	screenShader->read_source("shaders/screen/screen.vert", "shaders/screen/screen.frag");
	screenShader->compile();
	screenShader->initUniforms();
	screenShader->setInt("screenTexture", 0);

	depthScreenShader = new ScreenShader;
	depthScreenShader->read_source("shaders/screen/screen.vert", "shaders/screen/depthScreen.frag");
	depthScreenShader->compile();
	depthScreenShader->initUniforms();
	depthScreenShader->setInt("shadows", 0);


    shader = new ShadowShader;
    shader->read_source("shaders/projective.vert", "shaders/lighting.frag");
    shader->compile();
    glUseProgram(shader->program);
	depthShader->setInt("shadows", 1);
    shader->initUniforms();


	// Initialize lights
	SpotLight::staticInit(light.size());
	for (std::pair<std::string, SpotLight*> entry : light)
		entry.second->init();

	// Initialize quadVAO
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int buffers[2];
	glGenBuffers(2, &buffers[0]);
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Initialize screenbuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
