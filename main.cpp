#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
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

struct ParticleInfo {
    RenderParticle* rp;
    std::string name;
    bool finished = false;
};

int main(void)
{
	//frame 1 -> frame 2 takes 16ms
	constexpr std::chrono::nanoseconds timestep(16ms);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	float width = 700.0f;
	float height = 700.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "PC01 Julian Baladjay", NULL, NULL);
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

    std::list<RenderParticle*> renderParticles;

	P6::PhysicsWorld pWorld = P6::PhysicsWorld();

    //red sphere
    auto* redParticle = new P6::P6Particle();
    redParticle->position = glm::vec3(0.0f, 350.0f, 0.0f); // top-left corner
    redParticle->velocity = glm::vec3(0.0f, 0.0f, 0.0f);     // toward center
    redParticle->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);       // constant accel

    auto* redObj = new OpenGLObject(attributes, shapes[0]);
    redObj->setDefaults();
    redObj->setOrthographic(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
	redObj->scale = glm::vec3(10.0f);

    auto* rpRed = new RenderParticle(redParticle, redObj, glm::vec3(1.0f, 0.0f, 0.0f), "Red");
    renderParticles.push_back(rpRed);

    //green sphere
    auto* greenParticle = new P6::P6Particle();
    greenParticle->position = glm::vec3(0.0f, 0.0f, 0.0f);
    greenParticle->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    greenParticle->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    auto* greenObj = new OpenGLObject(attributes, shapes[0]);
    greenObj->setDefaults();
    greenObj->setOrthographic(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
    greenObj->scale = glm::vec3(10.0f);

    auto* rpGreen = new RenderParticle(greenParticle, greenObj, glm::vec3(0.0f, 1.0f, 0.0f), "Green");
    renderParticles.push_back(rpGreen);

    //blue sphere
    auto* blueParticle = new P6::P6Particle();
    blueParticle->position = glm::vec3(0.0f, -350.0f, 0.0f);
    blueParticle->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    blueParticle->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    auto* blueObj = new OpenGLObject(attributes, shapes[0]);
    blueObj->setDefaults();
    blueObj->setOrthographic(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
    blueObj->scale = glm::vec3(10.0f);

    auto* rpBlue = new RenderParticle(blueParticle, blueObj, glm::vec3(0.0f, 0.0f, 1.0f), "Blue");
    renderParticles.push_back(rpBlue);
    
	using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();
    auto curr_time = start_time;
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        glEnable(GL_DEPTH_TEST);

        // get current time
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;
        if (curr_ns >= timestep)
        {
            constexpr float timestep_sec = timestep.count() / (float)(1E09);
            curr_ns -= timestep;

            for (auto& rp : renderParticles) {
                glm::vec3 toCenter = glm::vec3(0.0f) - rp->physicsParticle->position;
                glm::vec3 dir = glm::normalize(toCenter);

                float accelMag = glm::length(rp->physicsParticle->acceleration);
                rp->physicsParticle->acceleration = dir * accelMag;

                rp->physicsParticle->update(timestep_sec);

                // optional: stop condition if you still want them to freeze at center
                if (glm::length(toCenter) <= 1.0f ||
                    glm::dot(rp->physicsParticle->velocity, toCenter) <= 0.0f)
                {
                    rp->physicsParticle->position = glm::vec3(0.0f);
                    rp->physicsParticle->velocity = glm::vec3(0.0f);
                    rp->physicsParticle->acceleration = glm::vec3(0.0f);
                }

                rp->RenderObject->setPosition(rp->physicsParticle->position);
            }
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& rp : renderParticles) {
            rp->draw(shader);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }


    obj.cleanup();

    glfwTerminate();
    return 0;
}