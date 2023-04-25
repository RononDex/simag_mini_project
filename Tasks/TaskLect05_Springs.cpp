#include "TaskLect05_Springs.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include <glm/glm.hpp>

#include "../Helper/HelperDraw.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

namespace {}

bool TaskLect05_Springs::isValidParticleIdx(int idx) const {
    if (idx >= 0 && idx < (int)particleSystem(m_workOnPsIdx).size()) {
        return true;
    }
    return false;
}

void TaskLect05_Springs::addNeighbor(int idx0, int idx1) {
    int idxMin = std::min<int>(idx0, idx1);
    int idxMax = std::max<int>(idx0, idx1);
    if (!isValidParticleIdx(idxMin))
        return;
    if (!isValidParticleIdx(idxMax))
        return;
    auto &ps = particleSystem(m_workOnPsIdx);
    ps.states()[idxMin].neighborList().push(Neighbor(
        idxMax, glm::length(ps.positions()[idxMin] -
                            ps.positions()[idxMax]))); // only add neighbor from
                                                       // small idx -> large idx
}

void TaskLect05_Springs::addParticle(glm::vec3 const &pos, bool isStatic) {
    const glm::vec4 colDefault(1, 1, 1, 1);
    const glm::vec4 colStatic(1, 0, 0, 1);
    auto &ps = particleSystem(m_workOnPsIdx);
    ps.add(pos, isStatic ? colStatic : colDefault);
    ps.particleLast().setStatic(isStatic);
}

void TaskLect05_Springs::generateScene0_pendula() {
    // Pendula
    m_kSpring = 100.0f;
    m_dampingSpring = 5.0f;
    gEnv->stateSim->dtFixed = 0.006f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 30;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    addParticle(glm::vec3(0, 1, 0), true); // attach first particle
    addParticle(glm::vec3(1, 1, 0), false);

    addNeighbor(0, 1);
}

void TaskLect05_Springs::generateScene1_rope() {
    // Rope
    m_kSpring = 20.0f;
    m_dampingSpring = 5.0f;
    gEnv->stateSim->dtFixed = 0.006f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 30;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    const int nMax = 5;

    // Add partcicles
    // Set first and last particle as static
    for (int i = -nMax; i <= nMax; i++) {
        // todo students
        ps.add(glm::vec3(i, 0, 0), glm::vec3(0, 0, 0));
    }

    ps.particle(0).setStatic(true);
    ps.particle(ps.size() - 1).setStatic(true);

    // Add neighbors
    for (int i = -nMax; i < nMax; i++) {
        // todo students
        int idx = i + nMax;

        if (idx > 0) {
            addNeighbor(idx - 1, idx);
        }
        if (idx < nMax * 2) {
            addNeighbor(idx, idx + 1);
        }
    }
}

void TaskLect05_Springs::addBox(glm::vec3 const &p0, glm::vec3 const &p1) {

    // todo students
}

void TaskLect05_Springs::generateScene2_ragdoll() {

    m_kSpring = 2000.0f;
    m_dampingSpring = 50.0f;
    gEnv->stateSim->dtFixed = 0.002f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 10;

    auto &ps = particleSystem(m_workOnPsIdx);

    // todo students
}

void TaskLect05_Springs::generateScene3_cloth() {
    // Cloth
    m_kSpring = 20.0f;
    m_dampingSpring = 5.0f;
    gEnv->stateSim->dtFixed = 0.006f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 30;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    const int nX = 15;
    const int nY = 16;
    const float height = 3.0f;
    const float scale = 0.3f;

    // 1. Create grid of particles with following coordinates:
    //    (float)x* scale, height, (float)y* scale) with x=0..nX-1 and y=0..nY-1
    //    set all particles with y==0 static!

    // 2. add 3 types of springs
    //    - stretch
    //    - shear
    //    - bend
    // use helper method getIdx to receive neighbor ids within your grid - e.g.,
    // int idx = getIdx(x, y, nX, nY); add neighbor is safe, so you can try to
    // add springs to non valid particle ids (e.g., -1) - then a spring would
    // not be created
    // ==> thus, you don't have to take care of array-borders
    //

    // todo students
}

void TaskLect05_Springs::generateScene4() {
    // Bunny
}

void TaskLect05_Springs::generateSpringsAll() {
    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clearAllNeighbors();

    int s = (int)ps.size();
    if (s < 1)
        return;

    for (int y = 0; y < s - 1; y++) {
        glm::vec3 p0 = ps.positions()[y];
        for (int x = y + 1; x < s; x++) {
            glm::vec3 p1 = ps.positions()[x];
            p1 = p1 - p0;
            float dist = sqrtf(glm::dot(p1, p1));
            ps.states()[y].neighborList().push(Neighbor(
                x, dist)); // only add neighbor from small idx -> large idx
        }
    }
}

namespace {}

void TaskLect05_Springs::setForces() {

    auto &ps = particleSystem(m_workOnPsIdx);
    int s = (int)ps.size();
    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();

    const float kSpring = m_kSpring;       // spring constant
    const float bSpring = m_dampingSpring; // damping constant
    const float eps = 0.0001f;

    glm::vec3 distVec;
    glm::vec3 forceVec;
    glm::vec3 relVel;

    for (int i = 0; i < s; i++) {
        glm::vec3 &p0 = pos[i];
        glm::vec3 &v0 = vel[i];
        glm::vec3 &force0 = forces[i];
        auto &neighborList = ps.states()[i].neighborList().get();

        for (auto &n : neighborList) {
            // todo students
            glm::vec3 &force1 = forces[n.idx];
            auto distance_vector = -(p0 - pos[n.idx]);
            auto distance_normed = glm::normalize(distance_vector);
            auto spring_force =
                kSpring * (distance_vector - distance_normed * n.distance);
            auto dampening_force =
                glm::dot((vel[n.idx] - v0), distance_normed) * distance_normed *
                (bSpring);

            force0 += spring_force + dampening_force;
            force1 -= spring_force + dampening_force;
        }
    }
}

void TaskLect05_Springs::doWork() {}

void TaskLect05_Springs::imGui() {
    ImGui::SliderInt(paramName("Work on PS Idx"), &m_workOnPsIdx, 0,
                     noOfPs() - 1);
    ImGui::Separator();
    ImGui::SliderFloat(paramName("kSpring"), &m_kSpring, 0.1f, 10.0f);
    ImGui::Separator();
    ImGui::Checkbox(paramName("UseDampingSpring"), &m_bUseDampingSpring);
    ImGui::SliderFloat(paramName("dampingSpring"), &m_dampingSpring, 0.01f,
                       1.0f);
    ImGui::Separator();
    if (ImGui::Button(paramName("Generate Springs All"))) {
        generateSpringsAll();
    }
    ImGui::Separator();
    if (ImGui::Button(paramName("Generate Scene0 - Pendula"))) {
        generateScene0_pendula();
    }
    if (ImGui::Button(paramName("Generate Scene1 - Rope"))) {
        generateScene1_rope();
    }
    if (ImGui::Button(paramName("Generate Scene2 - Ragdoll"))) {
        generateScene2_ragdoll();
    }
    if (ImGui::Button(paramName("Generate Scene3 - Cloth"))) {
        generateScene3_cloth();
    }
    // if (ImGui::Button(paramName("Generate Scene4"))) { generateScene4(); }
}

const char *TaskLect05_Springs::toString() const {
    std::stringstream ss;
    ss << "Simulate springs."
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}

void TaskLect05_Springs::draw() const {}
