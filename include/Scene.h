/**************************************************

*****************************************************/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <math.h>

#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <stack>

#include "Camera.h"
#include "shaders/SurfaceShader.h"
#include "shaders/ShadowShader.h"
#include "shaders/ScreenShader.h"
#include "Light.h"
#include "SpotLight.h"
#include "Geometry.h"
#include "Material.h"
#include "Model.h"

#ifndef __SCENE_H__
#define __SCENE_H__

class Node {
public:
    std::vector< Node* > childnodes;
    std::vector< glm::mat4 > childtransforms;
    std::vector< Model* > models;
    std::vector< glm::mat4 > modeltransforms;
};

class Scene {
public:
	float t;
	bool renderDepth = false;
	int lightInd = 0;

	int width, height;
    Camera* camera;
    ShadowShader* shader;
	SurfaceShader* depthShader;
	
	ScreenShader* screenShader;
	ScreenShader* depthScreenShader;
	unsigned int quadVAO;

    // The following are containers of objects serving as the object palettes.
    // The containers store pointers so that they can also store derived class objects.
    std::map< std::string, Geometry* > geometry;
    std::map< std::string, Material* > material;
    std::map< std::string, Model* > model;
    std::map< std::string, SpotLight* > light;
    
    // The container of nodes will be the scene graph after we connect the nodes by setting the child_nodes.
    std::map< std::string, Node* > node;

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;
    
    Scene(){
        // the default scene graph already has one node named "world."
        node["world"] = new Node;
    }

    void init( int w, int h );
    void draw( void );
	void render(SurfaceShader* shader, std::set<Model*>* ignore);
    
    // destructor
    ~Scene(){
        // The containers of pointers own the object pointed to by the pointers.
        // All the objects should be deleted when the object palette is destructed.
        // light
        for(std::pair<std::string,SpotLight*> entry : light ){
            delete entry.second;
        }
        // geometry
        for(std::pair<std::string,Geometry*> entry : geometry ){
            delete entry.second;
        }
        // material
        for(std::pair<std::string,Material*> entry : material ){
            delete entry.second;
        }
        // model
        for(std::pair<std::string,Model*> entry : model ){
            delete entry.second;
        }
        // model
        for(std::pair<std::string,Node*> entry : node ){
            delete entry.second;
        }
        delete camera;
        delete shader;
		delete depthShader;
    }
};

#endif 
