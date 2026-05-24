#version 330 core //version

//gets data at attrib index 0
//converts it and stores into vec3
layout(location = 0) in vec3 aPos;

//transform variable
uniform mat4 transform;

//projection variable
uniform mat4 projection;

void main()
{
	//gl_Position is predefined
	//denotes final position of vertex
	gl_Position = projection * //multiply projection with transform
					transform *  //multiply matrix with vertex position
					vec4(aPos, 1.0); //turns 3x1 matrix into 4x1
}