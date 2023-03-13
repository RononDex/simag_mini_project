#include "TaskAssignment_Morph.h"

#include <glad/gl.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include "../Helper/HelperDraw.h"
#include "../Helper/Image.h"
#include "../ParticleSystem/ParticleSystem.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

namespace {

void createPsFromImage(std::string const &filename, ParticleSystem &ps) {
    std::string filepath = std::string(Globals::PATH_DATA) + filename;
    std::cout << "Loading image: " << filepath << "...\n";

    Image image(filepath);

    if (image.size() == 0) {
        std::cout << "Error loading image: " << filepath << "\n";
        return;
    }

    const float size = 3.0f;
    const float scaleTo1 =
        1.0f / (float)std::max(image.width(), image.height());
    glm::vec3 offs(-0.5f, -0.5f, 0.0f); // center images

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            glm::vec3 const &c = image.at(x, y);
            if (c.r + c.g + c.b > 0.01f) // not black?
            {
                glm::vec3 p((float)(x - image.width() / 2) * scaleTo1,
                            (float)(y - image.height() / 2) * scaleTo1, 0.0f);
                p = p * size;
                ps.add(p, glm::vec4(c, 1.0f));
            }
        }
    }
}

} // anonymous namespace

void TaskAssignment_Morph::setForces() {}

glm::vec3 moveVectorTowards(glm::vec3 source, glm::vec3 target,
                            float amountPercentage) {
    glm::vec3 delta = glm::vec3(target.x - source.x, target.y - source.y,
                                target.z - source.z);

    return glm::vec3(source.x + (delta.x * amountPercentage),
                     source.y + (delta.y * amountPercentage),
                     source.z + (delta.z * amountPercentage));
}

glm::vec4 moveVectorTowards(glm::vec4 source, glm::vec4 target,
                            float amountPercentage) {
    glm::vec4 delta = glm::vec4(target.x - source.x, target.y - source.y,
                                target.z - source.z, target.w - source.w);

    return glm::vec4(source.x + (delta.x * amountPercentage),
                     source.y + (delta.y * amountPercentage),
                     source.z + (delta.z * amountPercentage),
                     source.w + (delta.w * amountPercentage));
}

void TaskAssignment_Morph::doWork() {
    // Do nothing, if simulation is not running
    if (!gEnv->stateSim->isRunning) {
        return;
    }

    // Get Timestep
    const float dt = gEnv->stateSim->dt;

    auto &ps0 = particleSystem(0); // get particle system 0 -> ps0
    int s0 = (int)ps0.size();      // number of particles in ps0
    auto &ps1 = particleSystem(1); // get particle system 1 -> ps1
    int s1 = (int)ps1.size();      // number of particles in ps1

    const int s = std::min<int>(s0, s1);

    for (int i = 0; i < s; i++) {
        glm::vec3 pos = ps0.particle(i).pos();
        glm::vec4 color = ps0.particle(i).color();

        glm::vec3 targetPos = ps1.particle(i).pos();
        glm::vec4 targetColor = ps1.particle(i).color();

        ps0.particle(i).pos() =
            moveVectorTowards(pos, targetPos, m_morphSpeed * dt);
        ps0.particle(i).color() =
            moveVectorTowards(color, targetColor, m_morphSpeed * dt);
    }
}

void TaskAssignment_Morph::draw() const {}

void TaskAssignment_Morph::imGui() {
    if (ImGui::Button(paramName("Load ParticleSystems - b/w"))) {
        particleSystem(0).clear();
        particleSystem(1).clear();
        createPsFromImage("imageMorph0.png", particleSystem(0));
        createPsFromImage("imageMorph1.png", particleSystem(1));
    }

    if (ImGui::Button(paramName("Load ParticleSystems - Color"))) {
        particleSystem(0).clear();
        particleSystem(1).clear();
        createPsFromImage("imageMorph0_col.png", particleSystem(0));
        createPsFromImage("imageMorph1_col.png", particleSystem(1));
    }

    ImGui::SliderFloat(paramName("Morph Speed"), &m_morphSpeed, 0.01f, 0.4f);
}

const char *TaskAssignment_Morph::toString() const {
    std::stringstream ss;

    ss << "Description:"
       << "\n";
    ss << "1. Load imageMorph0.png and imageMorph1.png on button"
       << "\n";
    ss << "   - images are loaded into particle systems 0 and 1"
       << "\n";
    ss << "2. Morph/Move particle system 0 to positions of particle system 1 "
       << "\n";
    ss << "   - simulation must be enabled for morphing"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
