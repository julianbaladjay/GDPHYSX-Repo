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

float yaw = 0.0f; // left/right rotation around Z
float pitch = 0.3f; // up/down tilt
float cameraDistance = 600.0f;

int main(void)
{
	//frame 1 -> frame 2 takes 16ms
	constexpr std::chrono::nanoseconds timestep(16ms);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	float width = 800.0f;
	float height = 800.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Group1/Yay P6", NULL, NULL);
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

	auto velocity = particle.velocity = glm::vec3(100.0f, 0.0f, 0.0f);

	auto acceleration = particle.acceleration = glm::vec3(10.0f, 0.0f, 0.0f);

	auto scale = obj.scale = glm::vec3(20.0f);

	using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();
    auto curr_time = start_time;
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    // Setup
    int sparks;
    std::cout << "Enter number of sparks: ";
    std::cin >> sparks;

    bool paused = false;
    bool usePerspective = true; // start in perspective

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Toggle play/pause
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            paused = !paused;
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) usePerspective = false;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) usePerspective = true;

        // Camera rotation (WASD)
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw -= 0.02f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw += 0.02f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += 0.02f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= 0.02f;

        // Timing
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;
        curr_ns += dur;

        if (curr_ns >= timestep) {
            constexpr float timestep_sec = timestep.count() / (float)(1E09);
            curr_ns -= timestep;

            if (!paused) {
                // Spawn continuously
                int spawnRate = 5; // sparks per frame
                for (int i = 0; i < spawnRate && (int)renderParticles.size() < sparks; ++i) {
                    auto* p = new P6::P6Particle();
                    pWorld.AddParticle(p);

                    auto* obj = new OpenGLObject(attributes, shapes[0]);
                    obj->setDefaults();
                    if (usePerspective) {
                        obj->setPerspective(60.0f, 1.0f, 0.1f, 1000.0f);
                    }
                    else {
                        obj->setOrthographic(-400, 400, -400, 400, -400, 400);
                    }

                    auto* rp = new RenderParticle(p, obj);
                    renderParticles.push_back(rp);
                }

                // Physics update
                pWorld.Update(timestep_sec);

                // Cleanup dead particles
                for (auto it = renderParticles.begin(); it != renderParticles.end();) {
                    if ((*it)->physicsParticle->IsDestroyed()) {
                        delete (*it)->physicsParticle;
                        delete (*it)->RenderObject;
                        delete* it;
                        it = renderParticles.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }


        glm::vec3 pivot = glm::vec3(0.0f, -400.0f, 0.0f); // fountain base

        // Compute camera position from angles
        glm::vec3 cameraPos = pivot + glm::vec3(
            cameraDistance * cos(pitch) * cos(yaw),
            cameraDistance * sin(pitch),
            cameraDistance * cos(pitch) * sin(yaw)
        );

        // Build view matrix
        glm::mat4 view = glm::lookAt(
            cameraPos,
            pivot, // look at fountain center
            glm::vec3(0.0f, 1.0f, 0.0f)  // up vector
        );

        glm::mat4 projection;
        if (usePerspective) {
            projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
        }
        else {
            projection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);
        }

        shader.use();
        shader.setMat4("view", glm::value_ptr(view));

        shader.setMat4("projection", glm::value_ptr(projection));

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& rp : renderParticles) {
            rp->draw(shader);
        }

        glfwSwapBuffers(window);
    }


    obj.cleanup();

    glfwTerminate();
    return 0;
}