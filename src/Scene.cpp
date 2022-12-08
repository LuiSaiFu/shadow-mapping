/**************************************************
Scene.cpp contains the implementation of the draw command
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"

// The scene init definition

//Choose Scene (1,2,3)
#define SCENE 3

#if SCENE == 1
#include "Scene.inl"
#elif SCENE == 2
#include "Scene2.inl"
#elif SCENE == 3
#include "Scene3.inl"
#endif


using namespace glm;

void Scene::render(SurfaceShader* sha, std::set<Model*>* ignore) {
	std::stack < Node* > dfs_stack;
	std::stack < mat4 >  matrix_stack;

	Node* cur = node["world"];
	mat4 cur_VM = sha->view;

	dfs_stack.push(cur);
	matrix_stack.push(cur_VM);

	int total_number_of_edges = 0;
	for (const auto& n : node) total_number_of_edges += n.second->childnodes.size();

	while (!dfs_stack.empty()) {
		if (dfs_stack.size() > total_number_of_edges) {
			std::cerr << "Error: The scene graph has a closed loop." << std::endl;
			exit(-1);
		}

		cur = dfs_stack.top();  dfs_stack.pop();
		cur_VM = matrix_stack.top(); matrix_stack.pop();

		// draw all the models at the current node
		for (size_t i = 0; i < cur->models.size(); i++) {
			if (ignore != nullptr && ignore->find(cur->models[i]) != ignore->end())
				continue;
			// Prepare to draw the geometry. Assign the modelview and the material.
			sha->modelview = cur_VM * cur->modeltransforms[i];
			sha->material = (cur->models[i])->material;

			// The draw command
			sha->setUniforms();
			(cur->models[i])->geometry->draw();
		}

		// Continue the DFS: put all the child nodes of the current node in the stack
		for (size_t i = 0; i < cur->childnodes.size(); i++) {
			dfs_stack.push(cur->childnodes[i]);
			matrix_stack.push(cur_VM * cur->childtransforms[i]);
		}

	}
	
}
void Scene::draw(void){
    shader -> nlights = light.size();
    shader -> lightpositions.resize( shader -> nlights );
    shader -> lightcolors.resize( shader -> nlights );
	shader -> lightVP.resize(shader->nlights);

	//First pass, depth buffer
	glUseProgram(depthShader->program);
	SpotLight* currLight = nullptr;
    int count = 0;
    for (std::pair<std::string, SpotLight*> entry : light){
		if (count == lightInd)
			currLight = entry.second;
		depthShader->view = entry.second->view;
		depthShader->projection = entry.second->proj;
		entry.second->prepareRenderDepth(count);
		this->render(depthShader, &(entry.second->ignore));

		shader->lightVP[count] = entry.second->proj * entry.second->view;

        shader -> lightpositions[ count ] = (entry.second) -> position;
        shader -> lightcolors[ count ] = (entry.second) -> color;
        count++;
    }
	
	
	//Second pass, draw to internal framebuffer
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, SpotLight::depthMap);

	camera->computeMatrices();
	shader->view = camera->view;
	shader->projection = camera->proj;
	this->render(shader, nullptr);
	


	//Third pass, draw to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (renderDepth)
	{
		depthScreenShader = depthScreenShader;
		depthScreenShader->ind = lightInd;
		depthScreenShader->far_dist = currLight->zFar;
		depthScreenShader->near_dist = currLight->zNear;

		glUseProgram(depthScreenShader->program);
		depthScreenShader->setUniforms();
	}
	else
	{
		glUseProgram(screenShader->program);
		screenShader->setUniforms();
	}

	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, SpotLight::depthMap);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
	

#if SCENE == 2 || SCENE == 3
	t += 1.0f;
	node["world"]->modeltransforms.pop_back();
	float a = t / 80;
	#if SCENE == 2
	node["world"]->modeltransforms.push_back(translate((-1.5f * vec3(glm::sin(a), 0.0f, glm::cos(a)-0.7f))) * rotate(a, vec3(0.0f, 1.0f, 0.0f)) * translate(vec3(0.0f, 1.0f, 1.5f)));
	#else
	node["world"]->modeltransforms.push_back(scale(vec3(0.5f, 0.5f, 0.5f)) * translate(vec3(0.0f, 0.0f, 2.0f)) * rotate(float(M_PI) / 2 + a, vec3(0.0f, 1.0f, 0.0f)));
	#endif
	glutPostRedisplay();
#endif
}



