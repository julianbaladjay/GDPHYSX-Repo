#include <string>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//tiny_obj_loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "source/OpenGLObject.h"
#include "source/OpenGLShader.h"
#include "source/P6Particle.h"
#include "source/RenderParticle.h"

#include <chrono>
#include <cmath>

using namespace std::chrono_literals;

int main(void)
{
	//frame 1 -> frame 2 takes 16ms
	constexpr std::chrono::nanoseconds timestep(16ms);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	float width = 1000.0f;
	float height = 1000.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Julian Baladjay", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	gladLoadGL();
    
    glViewport(0, 0, width, height);

    //use shaders
    OpenGLShader shader("Shaders/sample.vert", "Shaders/sample.frag");
	shader.use();

    // Load OBJ
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, "models/sphere.obj")) {
        std::cerr << "Failed to load OBJ: " << error << "\n";
        return -1;
    }

    if (shapes.empty()) {
        std::cerr << "No shapes found in OBJ\n";
        return -1;
    }

	OpenGLObject obj(attributes, shapes[0]);
    obj.setDefaults();

    obj.setOrthographic(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
    // or obj.setPerspective(60.0f, width/height, 0.1f, 100.0f);

	P6::P6Particle particle = P6::P6Particle();
    particle.velocity = glm::vec3(100, 0, 0);

    std::list<RenderParticle*> renderParticles;
	RenderParticle rp1 = RenderParticle(&particle, &obj, glm::vec3(1.0f, 0.0f, 0.0f));
	renderParticles.push_back(&rp1);

	using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
	auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    const float leftBound = -400.0f + obj.scale.x * 0.5f;
    const float rightBound = 400.0f - obj.scale.x * 0.5f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        glEnable(GL_DEPTH_TEST);

		//get current time
        curr_time = clock::now();
        //iteration time 0 -> 1
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
		prev_time = curr_time;

		curr_ns += dur;
		if(curr_ns >= timestep) 
        {
            //call physics engine
			//convert ns to seconds
            constexpr float timestep_sec = timestep.count() / (float)(1E09); //1 with 9 zeroes
            curr_ns -= timestep;

            std::cout << "P6 Update" << std::endl;

			particle.update(timestep_sec);
            obj.position = particle.position;

            if (obj.position.x <= leftBound) {
                obj.position.x = leftBound;        
				particle.velocity.x *= -1.0f;      //go to the right
            }
            else if (obj.position.x >= rightBound) {
                obj.position.x = rightBound;       
                particle.velocity.x *= -1.0f;      //go to the left
            }
		}

		std::cout << "Normal Update" << std::endl;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (std::list<RenderParticle*>::iterator i = renderParticles.begin(); i != renderParticles.end(); i++) 
        {
			(*i)->draw(shader);
		}

        // Let RenderParticle sync position/color and draw
        //renderParticle.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    obj.cleanup();

    glfwTerminate();
    return 0;
}