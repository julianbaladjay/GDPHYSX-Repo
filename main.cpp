#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "source/OpenGLObject.h"
#include "source/OpenGLShader.h"
#include "source/P6Particle.h"
#include "source/RenderParticle.h"
#include "source/PhysicsWorld.h"
#include "source/Cable.h"

#include <chrono>
using namespace std::chrono_literals;

float yaw = glm::radians(90.0f);
float pitch = glm::radians(15.0f);
float cameraDistance = 800.0f;

int main() {
    constexpr std::chrono::nanoseconds timestep(16ms);

    float cableLen, particleGap, particleRadius, gravityStrength, initialForce;
    std::cout << "Cable Length: "; std::cin >> cableLen;
    std::cout << "Particle Gap: "; std::cin >> particleGap;
    std::cout << "Particle Radius: "; std::cin >> particleRadius;
    std::cout << "Gravity Strength (Y-axis): "; std::cin >> gravityStrength;
    std::cout << "Initial Force (left-most particle): "; std::cin >> initialForce;

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Newton's Cradle", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 800, 800);

    OpenGLShader shader("Shaders/sample.vert", "Shaders/sample.frag");
    OpenGLShader lineShader("Shaders/line.vert", "Shaders/line.frag");
    shader.use();

    GLuint lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, "models/sphere.obj")) {
        std::cerr << "Failed to load OBJ: " << error << "\n";
        return -1;
    }
    if (shapes.empty()) { std::cerr << "No shapes found in OBJ\n"; return -1; }

    P6::PhysicsWorld pWorld(10);
    std::list<RenderParticle*> renderParticles;

    int numParticles = 5;
    float startX = -((numParticles - 1) * particleGap) / 2.0f;
    float anchorY = 300.0f;

    for (int i = 0; i < numParticles; ++i) {
        auto* p = new P6::P6Particle();
        p->mass = 50.0f;
        p->damping = 0.98f;
        p->radius = particleRadius;
        p->restitution = 0.9f;
        p->position = glm::vec3(startX + i * particleGap, anchorY - cableLen, 0.0f);

        if (i == 0) p->position.x -= 100.0f; // offset left-most

        p->velocity = glm::vec3(0.0f);
        pWorld.AddParticle(p);

        auto* obj = new OpenGLObject(attributes, shapes[0]);
        obj->setDefaults();
        obj->scale = glm::vec3(particleRadius);
        obj->projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);

        auto* rp = new RenderParticle(p, obj);
        renderParticles.push_back(rp);

        glm::vec3 anchorPoint(p->position.x, anchorY, 0.0f); // anchor directly above
        auto* cable = new P6::Cable(anchorPoint, cableLen);
        cable->particles[0] = p;
        pWorld.AddLink(cable);

        std::cout << "Cable " << i << " anchor=(" << anchorPoint.x << "," << anchorPoint.y
            << ") particle=(" << p->position.x << "," << p->position.y << ")\n";
    }

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    bool paused = true;
    bool usePerspective = true;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && paused) {
            paused = false;
            auto* leftMost = renderParticles.front()->physicsParticle;
            leftMost->addForce(glm::vec3(initialForce, 0.0f, 0.0f));
        }

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;
        curr_ns += dur;

        if (curr_ns >= timestep) {
            constexpr float timestep_sec = timestep.count() / 1e9f;
            curr_ns -= timestep;
            if (!paused) pWorld.Update(timestep_sec);
        }

        glm::vec3 pivot(0.0f);
        glm::vec3 cameraPos = pivot + glm::vec3(
            cameraDistance * cos(pitch) * cos(yaw),
            cameraDistance * sin(pitch),
            cameraDistance * cos(pitch) * sin(yaw)
        );

        glm::mat4 view = glm::lookAt(cameraPos, pivot, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = usePerspective
            ? glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f)
            : glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -400.0f, 400.0f);

        shader.use();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& rp : renderParticles) rp->draw(shader);

        for (auto* link : pWorld.links) {
            if (auto* cable = dynamic_cast<P6::Cable*>(link)) {
                if (cable->particles[0]) {
                    RenderParticle::drawLine(cable->anchor, cable->particles[0]->position);
                }
            }
        }

        glfwSwapBuffers(window);
    }

    for (auto& rp : renderParticles) {
        delete rp->physicsParticle;
        delete rp->RenderObject;
        delete rp;
    }

    glfwTerminate();
    return 0;
}
