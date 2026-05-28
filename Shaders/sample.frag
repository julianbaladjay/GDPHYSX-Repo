#version 330 core //version

uniform vec3 color;

out vec4 FragColor; //output color

//simple shader with color
void main()
{
	//FragColor = vec4(0.6f, 0.0f, 0.2f, 1.0f); //set color in RGBA format

	FragColor = vec4(color.x, color.y, color.z, 1.0f); //set color in RGBA format
}