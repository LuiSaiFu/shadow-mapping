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
    geometry["teapot"] = new Obj;
    geometry["bunny"] = new Obj;
    geometry["cube"]->init();
    geometry["teapot"]->init("models/teapot.obj");
    geometry["bunny"]->init("models/bunny.obj");
    
    // Create a material palette
    material["wood"] = new Material;
    material["wood"]->ambient = vec4(0.1f,0.1f,0.1f,1.0f);
    material["wood"]->diffuse = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"]->specular = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"]->shininess = 100.0f;

	material["altwood"] = new Material;
	material["altwood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	material["altwood"]->diffuse = vec4(0.1f, 0.15f, 0.1f, 1.0f);
	material["altwood"]->specular = vec4(0.1f, 0.15f, 0.1f, 1.0f);
	material["altwood"]->shininess = 10.0f;
    
    material["ceramic"] = new Material;
    material["ceramic"]->ambient = vec4(0.02f, 0.07f, 0.2f, 1.0f);
    material["ceramic"]->diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
    material["ceramic"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["ceramic"]->shininess = 150.0f;
 
    material["silver"] = new Material;
    material["silver"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["silver"]->diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    material["silver"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["silver"]->shininess = 50.0f;
    
    material["turquoise"] = new Material;
    material["turquoise"]->ambient = vec4(0.1f, 0.2f, 0.17f, 1.0f);
    material["turquoise"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["turquoise"]->specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["turquoise"]->shininess = 100.0f;
    
    material["bulb"] = new Material;
    material["bulb"]->ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"]->diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"]->specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material["bulb"]->emision = vec4(1.0f,0.2f,0.1f,1.0f);
    material["bulb"]->shininess = 200.0f;
    
    // Create a model palette
    model["teapot1"] = new Model;
    model["teapot1"]->geometry = geometry["teapot"];
    model["teapot1"]->material = material["silver"];
    model["teapot2"] = new Model;
    model["teapot2"]->geometry = geometry["teapot"];
    model["teapot2"]->material = material["ceramic"];
    model["table piece"] = new Model;
    model["table piece"]->geometry = geometry["cube"];
    model["table piece"]->material = material["wood"];
	model["chair piece"] = new Model;
	model["chair piece"]->geometry = geometry["cube"];
	model["chair piece"]->material = material["altwood"];
    model["bunny"] = new Model;
    model["bunny"]->geometry = geometry["bunny"];
    model["bunny"]->material = material["turquoise"];
    model["bulb"] = new Model;
    model["bulb"]->geometry = geometry["cube"];
    model["bulb"]->material = material["bulb"];
    
    // Create a light palette
	/*
    light["sun"] = new SpotLight;
    light["sun"]->position = vec4(3.0f,2.0f,1.0f,0.0f);
    light["sun"]->color = 1.0f*vec4(1.0f,1.0f,1.0f,1.0f);
	light["sun"]->direction = vec4(1.0f, -1.0f, 0.0f, 0.0f);
	*/
    
    light["bulb"] = new SpotLight;
    light["bulb"]->position = vec4(0.0f,2.0f,0.0f,1.0f);
    light["bulb"]->color = 1.5f * vec4(1.0f,0.2f,0.1f,1.0f);
	light["bulb"]->direction = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    
    // Build the scene graph
    node["table"] = new Node;
    node["table top"] = new Node;
    node["table leg"] = new Node;
    node["teapot1"] = new Node;
    node["teapot2"] = new Node;
    node["bunny"] = new Node;
	node["chair"] = new Node;
	node["chair surface"] = new Node;
	node["chair leg"] = new Node;
	node["chair back"] = new Node;
    
    
    node["table"]->childnodes.push_back( node["table top"] );
    node["table"]->childtransforms.push_back( translate(vec3(0.0f,1.2f,0.0f)) );
    node["table"]->childnodes.push_back( node["table leg"] );
    node["table"]->childtransforms.push_back( translate(vec3(-0.9f,0.0f,-0.4f)) );
    node["table"]->childnodes.push_back( node["table leg"] );
    node["table"]->childtransforms.push_back( translate(vec3(-0.9f,0.0f,0.4f)) );
    node["table"]->childnodes.push_back( node["table leg"] );
    node["table"]->childtransforms.push_back( translate(vec3(0.9f,0.0f,0.4f)) );
    node["table"]->childnodes.push_back( node["table leg"] );
    node["table"]->childtransforms.push_back( translate(vec3(0.9f,0.0f,-0.4f)) );
    
    node["table leg"]->models.push_back( model["table piece"] );
    node["table leg"]->modeltransforms.push_back( translate(vec3(0.0f,0.5f,0.0f)) * scale(vec3(0.2f,1.0f,0.2f)) );
    
    node["table top"]->models.push_back( model["table piece"] );
    node["table top"]->modeltransforms.push_back( translate(vec3(0.0f,-0.1f,0.0f)) * scale(vec3(2.0f,0.2f,1.0f)) );
    node["table top"]->childnodes.push_back( node["teapot1"] );
    node["table top"]->childtransforms.push_back( translate(vec3(-0.5f,0.0f,0.0f)) );
    node["table top"]->childnodes.push_back( node["teapot2"] );
    node["table top"]->childtransforms.push_back( translate(vec3( 0.5f,0.0f,0.0f)) * rotate( -120.0f*float(M_PI)/180.0f, vec3(0.0f, 1.0f, 0.0f) ) );
    
    node["teapot1"]->models.push_back( model["teapot1"] );
    node["teapot1"]->modeltransforms.push_back( scale(vec3(0.5f)) );
    node["teapot2"]->models.push_back( model["teapot2"] );
    node["teapot2"]->modeltransforms.push_back( scale(vec3(1.0f,1.5f,1.0f)) * scale(vec3(0.5f)) );
    
    node["bunny"]->models.push_back( model["bunny"] );
    node["bunny"]->modeltransforms.push_back( translate(vec3(0.0f,0.8f,0.0f)) * scale(vec3(0.8f)) * translate(vec3(0.0f,1.0f,0.0f)) );
    
	node["chair"]->childnodes.push_back(node["chair surface"]);
	node["chair"]->childtransforms.push_back(translate(vec3(0.0f, 0.0f, 0.0f)));
	node["chair"]->childnodes.push_back(node["chair leg"]);
	node["chair"]->childtransforms.push_back(translate(vec3(-0.55f, -0.35f, -0.55f)));
	node["chair"]->childnodes.push_back(node["chair leg"]);
	node["chair"]->childtransforms.push_back(translate(vec3(0.55f, -0.35f, -0.55f)));
	node["chair"]->childnodes.push_back(node["chair leg"]);
	node["chair"]->childtransforms.push_back(translate(vec3(-0.55f, -0.35f, 0.55f)));
	node["chair"]->childnodes.push_back(node["chair leg"]);
	node["chair"]->childtransforms.push_back(translate(vec3(0.55f, -0.35f, 0.55f)));
	node["chair"]->childnodes.push_back(node["chair back"]);
	node["chair"]->childtransforms.push_back(translate(vec3(-0.55f, 0.75f, 0.0f)));

	node["chair leg"]->models.push_back(model["chair piece"]);
	node["chair leg"]->modeltransforms.push_back(scale(vec3(0.1f, 0.7f, 0.1f)));

	node["chair surface"]->models.push_back(model["chair piece"]);
	node["chair surface"]->modeltransforms.push_back(scale(vec3(1.2f, 0.1f, 1.2f)));

	node["chair back"]->models.push_back(model["chair piece"]);
	node["chair back"]->modeltransforms.push_back(scale(vec3(0.1f, 1.5f, 1.2f)));

    node["world"]->childnodes.push_back( node["table"] );
    node["world"]->childtransforms.push_back( mat4(1.0f) );
	node["world"]->childnodes.push_back(node["chair"]);
	node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.75f, 0.0f)));
    node["world"]->childnodes.push_back( node["bunny"] );
    node["world"]->childtransforms.push_back( translate(vec3(-1.8f,0.0f,0.0f)) * rotate( 90.0f*float(M_PI)/180.0f, vec3(0.0f, 1.0f, 0.0f) ));
   
	//node["world"]->models.push_back( model["bulb"] );
    //node["world"]->modeltransforms.push_back( translate(vec3(0.0f,2.0f,0.0f))*scale(vec3(0.1f)) );
    
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

	screenShader = new Shader;
	screenShader->read_source("shaders/screen/screen.vert", "shaders/screen/screen.frag");
	screenShader->compile();

    shader = new ShadowShader;
    shader->read_source( "shaders/projective.vert", "shaders/lighting.frag" );
    shader->compile();
    glUseProgram(shader->program);
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
