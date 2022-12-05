#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {
		mat4 invView = inverse(view);
		vec3 eye = vec3(invView[3][0], invView[3][1], invView[3][2]);
		mat4 model = invView*modelview;
        vec3 N = normalize(transpose(inverse(mat3(model)))*normal);
		vec3 P = vec3(model*position);
		vec4 R = vec4(emision);
		for(int i = 0; i < nlights; i++)
		{
			vec3 l = normalize(lightpositions[i].xyz - P * lightpositions[i].w);
			vec3 h = normalize(normalize(eye-P)+l);
			R += (ambient + diffuse*max(dot(N,l),0) + specular*pow(max(dot(N,h),0),shininess)) * lightcolors[i];
		}
		fragColor = R;
    }
}