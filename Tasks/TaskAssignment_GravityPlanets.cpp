#include "TaskAssignment_GravityPlanets.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include <glm/glm.hpp>

#include "../Helper/HelperDraw.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

namespace {

inline void determineGravityNormalized(glm::vec3 &outGravity,
                                       glm::vec3 const &p0, float const &mass0,
                                       glm::vec3 const &p1, float const &mass1,
                                       glm::vec3 &temp) {

    // todo students
}

} // namespace

void TaskAssignment_GravityPlanets::setForces() {
    auto &ps = particleSystem(m_workOnPsIdx);

    int s = (int)ps.size();

    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &mass = ps.mass();
    auto &forces = ps.forces();

    glm::vec3 temp;
    glm::vec3 gravity;

    const float intensity = m_gravityIntensity;

    // todo students
    for (int i = 0; i < pos.size(); i++) {
        auto &cur_pos = pos[i];
        auto &cur_mass = mass[i];
        auto &cur_force = forces[i];

        for (int j = 0; j < pos.size(); j++) {
            if (i == j)
                continue; // Ignore gravitational forces with one self
            auto &other_pos = pos[j];
            auto &other_mass = mass[j];

            glm::vec3 delta_Vector = other_pos - cur_pos;
            float distance = glm::length(delta_Vector);

            glm::vec3 F_G = delta_Vector / distance * m_gravityIntensity *
                            cur_mass * other_mass * m_gravityConstant /
                            ((float)pow(distance, 2));

            cur_force += F_G;
        }
    }
}

void TaskAssignment_GravityPlanets::doWork() {}

void TaskAssignment_GravityPlanets::generateScene0() {
    gEnv->stateSim->dtFixed = 0.001f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 10;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    // todo students
    ps.add(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 100);
    ps.add(glm::vec3(2, 0, 0), glm::vec3(0, 1, 0), 1);
}

void TaskAssignment_GravityPlanets::generateScene1() {
    gEnv->stateSim->dtFixed = 0.001f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 10;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    // todo students
    ps.add(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 100);
    ps.add(glm::vec3(2, 0, 0), glm::vec3(0, 1, 0), 3);
    ps.add(glm::vec3(2.1, 0, 0), glm::vec3(0, 2.5, 0), 0.3);
    ps.add(glm::vec3(3, 0, 0), glm::vec3(0, 1, 0), 2);
    ps.add(glm::vec3(3.1, 0, 0), glm::vec3(0, 2.5, 0), 0.3);
    ps.add(glm::vec3(4, 0, 0), glm::vec3(0, 1.5, 0), 1);
    ps.add(glm::vec3(4.1, 0, 0), glm::vec3(0, 2.5, 0), 0.3);
}

void TaskAssignment_GravityPlanets::generateScene2() {
    gEnv->stateSim->dtFixed = 0.001f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 10;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    // Have fun here, if you like.
    // todo students
}

void TaskAssignment_GravityPlanets::imGui() {
    ImGui::SliderInt(paramName("Work on PS Idx"), &m_workOnPsIdx, 0,
                     noOfPs() - 1);

    ImGui::Separator();

    ImGui::SliderFloat(paramName("GravityIntensity"), &m_gravityIntensity,
                       0.01f, 10.0f);

    ImGui::Separator();
    if (ImGui::Button(paramName("Generate Scene0"))) {
        generateScene0();
    }
    if (ImGui::Button(paramName("Generate Scene1"))) {
        generateScene1();
    }
    if (ImGui::Button(paramName("Generate Scene2"))) {
        generateScene2();
    }
}

const char *TaskAssignment_GravityPlanets::toString() const {
    std::stringstream ss;
    ss << "Gravity: Handle particles as planets"
       << "\n";
    ss << "Following Newton's theory of gravitation"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}

void TaskAssignment_GravityPlanets::draw() const {}
