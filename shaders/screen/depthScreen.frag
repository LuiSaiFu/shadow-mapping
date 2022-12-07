#version 330 core
out vec4 fragColor;
  
in vec2 TexCoords;

uniform sampler2DArray shadows;

uniform float near;
uniform float far;
uniform int ind;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = texture(shadows, vec3(TexCoords, ind)).r;
	fragColor = vec4(vec3(LinearizeDepth(depth) / far), 1.0f);
}