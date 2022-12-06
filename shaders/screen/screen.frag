#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
//uniform sampler2DArray shadows;
//uniform sampler2D shadow;

float near = 0.01;
float far = 100f;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    FragColor = texture(screenTexture, TexCoords);

    //float depth = texture(shadows, vec3(TexCoords, 0)).r;
    //FragColor = vec4(vec3(LinearizeDepth(depth) / far), 1.0f);

    //float depth = texture(shadow, TexCoords).r;
	//FragColor = vec4(vec3(LinearizeDepth(depth) / far), 1.0f);
}