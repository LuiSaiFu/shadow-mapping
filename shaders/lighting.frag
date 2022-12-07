#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

in vec4 gl_FragCoord;

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


uniform mat4 lightVP[ maximal_allowed_lights ];
uniform sampler2DArray shadows;


// Output the frag color
out vec4 fragColor;

///DEBUG ONLY
float near = 0.01f;
float far = 1.0f;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}
vec4 DepthCompute(int ind, vec4 position)
{
	vec4 projP = lightVP[ind] * position;
	projP = projP / projP.w;
	projP = projP * 0.5 + 0.5;
	float sDepth = texture(shadows, vec3(projP.xy, ind)).r;
	vec4 c = vec4(vec3(LinearizeDepth(sDepth) / far), 1.0f);
	return c;
}
///DEBUG ONLY

float ShadowComputation(int ind, vec4 position, vec3 lightDir)
{
	vec4 projP = lightVP[ind] * position;
	projP = projP / projP.w;
	projP = projP * 0.5 + 0.5;
	float bias = max(0.002 * (1.0 - dot(normalize(normal), -lightDir)), 0.0002);
	/*
	float sDepth = texture(shadows, vec3(projP.xy, ind)).r;
	float shadow = sDepth  ? 1.0f : 0.0f;
	*/
	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(shadows, 0));
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadows, vec3(projP.xy + vec2(x, y) * texelSize, ind)).r; 
			shadow += pcfDepth < projP.z - bias ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	return shadow;
}

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
			vec3 d = lightpositions[i].xyz - P * lightpositions[i].w;
			vec3 l = normalize(d);
			vec3 h = normalize(normalize(eye-P)+l);
			float shadow = ShadowComputation(i, model * position, l);
			R += (ambient + (diffuse*max(dot(N,l),0) + specular*pow(max(dot(N,h),0),shininess)) * (1.0f-shadow)) * lightcolors[i] / sqrt(d.x*d.x+d.y*d.y+d.z*d.z);
			//fragColor = vec4(vec3(shadow), 1.0f);
		}
		fragColor = R;
    }
}
