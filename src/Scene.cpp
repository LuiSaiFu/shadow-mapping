/**************************************************
Scene.cpp contains the implementation of the draw command
*****************************************************/
#include "Scene.h"
#include "Cube.h"
#include "Obj.h"

// The scene init definition 
#include "Scene.inl"


using namespace glm;

void Scene::render(void) {
	std::stack < Node* > dfs_stack;
	std::stack < mat4 >  matrix_stack;

	Node* cur = node["world"];
	mat4 cur_VM = camera->view;

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
			// Prepare to draw the geometry. Assign the modelview and the material.
			shader->modelview = cur_VM * cur->modeltransforms[i]; // TODO: HW3: Without updating cur_VM, modelview would just be camera's view matrix.
			shader->material = (cur->models[i])->material;

			// The draw command
			shader->setUniforms();
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
    int count = 0;
    for (std::pair<std::string, SpotLight*> entry : light){
		shader->view = entry.second->view;
		shader->projection = entry.second->proj;
		entry.second->renderDepth();

        shader -> lightpositions[ count ] = (entry.second) -> position;
        shader -> lightcolors[ count ] = (entry.second) -> color;
        count++;
    }

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	camera->computeMatrices();
	shader->view = camera->view;
	shader->projection = camera->proj;
	this->render();
}



