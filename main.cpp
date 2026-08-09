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
#include "source/PhysicsWorld.h"

#include <chrono>
#include <cmath>

using namespace std::chrono_literals;

//x position of model
float x_mod = 0;

//y position of model
float y_mod = 0;

//z position of model
float z_mod = 0;

void Key_Callback(GLFWwindow* window, //pointer to window
	                int key, // key pressed
	                int scancode, //physical key code
                    int action, //press or release
	                int mods) //which modifier keys are pressed
{
	//user presses escape key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        //quit program
		glfwSetWindowShouldClose(window, true);
	}

    //when user presses D
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//move model right
		x_mod += 0.1f;
	}

	//when user presses A
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//move model left
		x_mod -= 0.1f;
	}

	//when user presses W
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//move model up
		y_mod += 0.1f;
	}

	//when user presses S
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//move model down
		y_mod -= 0.1f;
	}
}

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

	//set key callback function
	glfwSetKeyCallback(window, Key_Callback);

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

    // Second render object, reusing the same loaded mesh data
    OpenGLObject obj2(attributes, shapes[0]);
    obj2.setDefaults();
    obj2.setOrthographic(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);

    const float leftBound = -400.0f + obj.scale.x * 0.5f;
    const float rightBound = 400.0f - obj.scale.x * 0.5f;

    // Start both particles at the left edge, like the slides' p1/p2 demo,
    // so you can actually see them travel before p1 gets destroyed at the center
	P6::P6Particle particle = P6::P6Particle();
    particle.position = glm::vec3(leftBound, 200, 0);
    particle.velocity = glm::vec3(100, 0, 0);

    P6::P6Particle particle2 = P6::P6Particle();
    particle2.position = glm::vec3(leftBound, 0, 0);
    particle2.velocity = glm::vec3(100, 0, 0);

    RenderParticle renderParticle(&particle, &obj, glm::vec3(1.0f, 0.0f, 0.0f));
    RenderParticle renderParticle2(&particle2, &obj2, glm::vec3(0.0f, 0.4f, 1.0f));

    // Create our Physics World and register both particles with it
    // instead of updating each particle by hand
    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    pWorld.AddParticle(&particle);
    pWorld.AddParticle(&particle2);

	using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
	auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);


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

            //Update the physics world -- this updates every particle
            //registered with it (and clears out any destroyed ones)
            //instead of us calling particle.update() by hand for each one
            pWorld.Update(timestep_sec);

            obj.position = particle.position;
            obj2.position = particle2.position;

            if (obj.position.x <= leftBound) {
                obj.position.x = leftBound;        
				particle.velocity.x *= -1.0f;      //go to the right
            }
            else if (obj.position.x >= rightBound) {
                obj.position.x = rightBound;       
                particle.velocity.x *= -1.0f;      //go to the left
            }

            if (obj2.position.x <= leftBound) {
                obj2.position.x = leftBound;
                particle2.velocity.x *= -1.0f;
            }
            else if (obj2.position.x >= rightBound) {
                obj2.position.x = rightBound;
                particle2.velocity.x *= -1.0f;
            }

            //When particle 1 reaches the center, destroy it
            //(it will stop updating and stop rendering)
            if (!particle.IsDestroyed() && std::abs(obj.position.x) < 1.0f) {
                particle.Destroy();
            }
		}

		std::cout << "Normal Update" << std::endl;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw particle 1 (skips itself once destroyed)
        shader.setMat4("projection", glm::value_ptr(obj.projection));
        shader.setMat4("transform", glm::value_ptr(obj.getTransform()));
        shader.setVec3("color", renderParticle.color);
        renderParticle.draw();

        // Draw particle 2
        shader.setMat4("projection", glm::value_ptr(obj2.projection));
        shader.setMat4("transform", glm::value_ptr(obj2.getTransform()));
        shader.setVec3("color", renderParticle2.color);
        renderParticle2.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    obj.cleanup();
    obj2.cleanup();

    glfwTerminate();
    return 0;
}