#include "TaskLect05_Springs.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include <glm/glm.hpp>

#include "../Helper/HelperDraw.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
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
    // Adds a box with the following indices:
    //
    //
    //     7 .+------+ 6 (p1)
    //     .' |    .'|
    //  4 +---+--+' 5|
    //    |   |  |   |
    //    | 3,+--+---+  2
    //    |.'    | .'
    //    +------+'
    //  0(p0)   1
    //
    //
    // todo students

    auto &ps = particleSystem(m_workOnPsIdx);

    auto center = (p0 + p1) / 2.0f;
    float height = std::abs(p0.y - p1.y);
    float width = std::abs(p0.z - p1.z);
    float length = std::abs(p0.x - p1.x);

    int startIndex = ps.positions().size();

    // Create particles
    ps.add(glm::vec3(p0.x, p0.y, p0.z));
    ps.add(glm::vec3(p0.x + length, p0.y, p0.z));
    ps.add(glm::vec3(p0.x + length, p0.y, p0.z + width));
    ps.add(glm::vec3(p0.x, p0.y, p0.z + width));
    ps.add(glm::vec3(p1.x - length, p1.y, p1.z - width));
    ps.add(glm::vec3(p1.x, p1.y, p1.z - width));
    ps.add(glm::vec3(p1.x, p1.y, p1.z));
    ps.add(glm::vec3(p1.x - length, p1.y, p1.z));

    // Add springs along the edges
    addNeighbor(startIndex, startIndex + 1);
    addNeighbor(startIndex + 1, startIndex + 2);
    addNeighbor(startIndex + 2, startIndex + 3);
    addNeighbor(startIndex + 3, startIndex);
    addNeighbor(startIndex, startIndex + 4);
    addNeighbor(startIndex + 1, startIndex + 5);
    addNeighbor(startIndex + 2, startIndex + 6);
    addNeighbor(startIndex + 3, startIndex + 7);
    addNeighbor(startIndex + 4, startIndex + 5);
    addNeighbor(startIndex + 5, startIndex + 6);
    addNeighbor(startIndex + 6, startIndex + 7);
    addNeighbor(startIndex + 7, startIndex + 4);

    // Add diagonal springs along the surfaces
    addNeighbor(startIndex, startIndex + 2);
    addNeighbor(startIndex + 1, startIndex + 3);
    addNeighbor(startIndex, startIndex + 5);
    addNeighbor(startIndex + 1, startIndex + 4);
    addNeighbor(startIndex + 1, startIndex + 6);
    addNeighbor(startIndex + 2, startIndex + 5);
    addNeighbor(startIndex + 2, startIndex + 7);
    addNeighbor(startIndex + 3, startIndex + 6);
    addNeighbor(startIndex + 3, startIndex + 4);
    addNeighbor(startIndex, startIndex + 7);
    addNeighbor(startIndex + 4, startIndex + 6);
    addNeighbor(startIndex + 5, startIndex + 7);

    // Add diagonal springs through the box
    addNeighbor(startIndex, startIndex + 6);
    addNeighbor(startIndex + 1, startIndex + 7);
    addNeighbor(startIndex + 2, startIndex + 4);
    addNeighbor(startIndex + 3, startIndex + 5);
}

void TaskLect05_Springs::generateScene2_ragdoll() {

    m_kSpring = 1500.0f;
    m_dampingSpring = 50.0f;
    gEnv->stateSim->dtFixed = 0.002f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 10;

    auto &ps = particleSystem(m_workOnPsIdx);

    int startIndex = ps.positions().size();

    // todo students
    // Create boxes with storing start indices in this array:
    int startIndices[] = {
        startIndex,      // Head
        startIndex + 8,  // Body
        startIndex + 16, // Left Arm upper
        startIndex + 24, // Left Arm lower
        startIndex + 32, // Right Arm upper
        startIndex + 40, // Right Arm lower
        startIndex + 48, // Left Leg upper
        startIndex + 56, // Left Leg lower
        startIndex + 64, // Right Leg upper
        startIndex + 72, // Right Leg lower
    };

    float width = 2.0f;
    float height = 2.0f;
    float depth = 0.4f;

    glm::vec3 centerPos =
        glm::vec3(width / 2.0f, height / 2.0f + 0.2, depth / 2.0f);

    // Setup boxes:
    // -------------------
    // Head
    addBox(glm::vec3(centerPos.x - width * 0.07f, centerPos.y + height * 0.32f,
                     centerPos.z - depth * 0.3f),
           glm::vec3(centerPos.x + width * 0.07f, centerPos.y + height * 0.5f,
                     centerPos.z + depth * 0.3f));

    // Body
    addBox(glm::vec3(centerPos.x - width * 0.15f, centerPos.y - height * 0.08f,
                     centerPos.z - depth * 0.5f),
           glm::vec3(centerPos.x + width * 0.15f, centerPos.y + height * 0.28f,
                     centerPos.z + depth * 0.5f));

    // Left Arm Upper
    addBox(glm::vec3(centerPos.x - width * 0.32f, centerPos.y + height * 0.2f,
                     centerPos.z - depth * 0.2f),
           glm::vec3(centerPos.x - width * 0.16f, centerPos.y + height * 0.25f,
                     centerPos.z + depth * 0.2f));

    // Left Arm lower
    addBox(glm::vec3(centerPos.x - width * 0.5f, centerPos.y + height * 0.22f,
                     centerPos.z - depth * 0.1f),
           glm::vec3(centerPos.x - width * 0.33f, centerPos.y + height * 0.25f,
                     centerPos.z + depth * 0.1f));

    // Right Arm Upper
    addBox(glm::vec3(centerPos.x + width * 0.16f, centerPos.y + height * 0.2f,
                     centerPos.z - depth * 0.2f),
           glm::vec3(centerPos.x + width * 0.32f, centerPos.y + height * 0.25f,
                     centerPos.z + depth * 0.2f));

    // Right Arm lower
    addBox(glm::vec3(centerPos.x + width * 0.33f, centerPos.y + height * 0.22f,
                     centerPos.z - depth * 0.1f),
           glm::vec3(centerPos.x + width * 0.5f, centerPos.y + height * 0.25f,
                     centerPos.z + depth * 0.1f));

    // Left Leg upper
    addBox(glm::vec3(centerPos.x - width * 0.25f, centerPos.y - height * 0.3f,
                     centerPos.z - depth * 0.2f),
           glm::vec3(centerPos.x - width * 0.15f, centerPos.y - height * 0.12f,
                     centerPos.z + depth * 0.2f));

    // Left Leg lower
    addBox(glm::vec3(centerPos.x - width * 0.23f, centerPos.y - height * 0.5f,
                     centerPos.z - depth * 0.1f),
           glm::vec3(centerPos.x - width * 0.17f, centerPos.y - height * 0.32f,
                     centerPos.z + depth * 0.1f));

    // Right Leg upper
    addBox(glm::vec3(centerPos.x + width * 0.15f, centerPos.y - height * 0.3f,
                     centerPos.z - depth * 0.2f),
           glm::vec3(centerPos.x + width * 0.25f, centerPos.y - height * 0.12f,
                     centerPos.z + depth * 0.2f));

    // Right Leg lower
    addBox(glm::vec3(centerPos.x + width * 0.17f, centerPos.y - height * 0.5f,
                     centerPos.z - depth * 0.1f),
           glm::vec3(centerPos.x + width * 0.23f, centerPos.y - height * 0.32f,
                     centerPos.z + depth * 0.1f));


    // Add connections between the boxes:
    // ------------------------------------
    // Connect Head to Body
    addNeighbor(startIndices[0], startIndices[1] + 4);
    addNeighbor(startIndices[0] + 4, startIndices[1] + 4);
    addNeighbor(startIndices[0] + 3, startIndices[1] + 7);
    addNeighbor(startIndices[0] + 7, startIndices[1] + 7);
    addNeighbor(startIndices[0] + 1, startIndices[1] + 5);
    addNeighbor(startIndices[0] + 5, startIndices[1] + 5);
    addNeighbor(startIndices[0] + 2, startIndices[1] + 6);
    addNeighbor(startIndices[0] + 6, startIndices[1] + 6);

    // Connect left Arm uppper to Body
    addNeighbor(startIndices[2] + 5, startIndices[1] + 4);
    addNeighbor(startIndices[2] + 1, startIndices[1] + 4);
    addNeighbor(startIndices[2] + 6, startIndices[1] + 7);
    addNeighbor(startIndices[2] + 2, startIndices[1] + 7);

    // Connect left Arm lower to left Arm upper
    addNeighbor(startIndices[3] + 1, startIndices[2] + 0);
    addNeighbor(startIndices[3] + 2, startIndices[2] + 3);
    addNeighbor(startIndices[3] + 5, startIndices[2] + 4);
    addNeighbor(startIndices[3] + 6, startIndices[2] + 7);

    // Connect right Arm uppper to Body
    addNeighbor(startIndices[4] + 0, startIndices[1] + 5);
    addNeighbor(startIndices[4] + 3, startIndices[1] + 6);
    addNeighbor(startIndices[4] + 4, startIndices[1] + 5);
    addNeighbor(startIndices[4] + 7, startIndices[1] + 6);

    // Connect right Arm lower to right arm upper
    addNeighbor(startIndices[5] + 0, startIndices[4] + 1);
    addNeighbor(startIndices[5] + 4, startIndices[4] + 5);
    addNeighbor(startIndices[5] + 7, startIndices[4] + 6);
    addNeighbor(startIndices[5] + 3, startIndices[4] + 2);

    // Connect left leg upper to Body
    addNeighbor(startIndices[6] + 4, startIndices[1] + 0);
    addNeighbor(startIndices[6] + 7, startIndices[1] + 3);
    addNeighbor(startIndices[6] + 5, startIndices[1] + 0);
    addNeighbor(startIndices[6] + 6, startIndices[1] + 3);

    // Connect left lower to left leg upper
    addNeighbor(startIndices[7] + 4, startIndices[6] + 0);
    addNeighbor(startIndices[7] + 7, startIndices[6] + 3);
    addNeighbor(startIndices[7] + 5, startIndices[6] + 1);
    addNeighbor(startIndices[7] + 6, startIndices[6] + 2);
    
    // Connect right leg upper to Body
    addNeighbor(startIndices[8] + 4, startIndices[1] + 1);
    addNeighbor(startIndices[8] + 7, startIndices[1] + 2);
    addNeighbor(startIndices[8] + 5, startIndices[1] + 1);
    addNeighbor(startIndices[8] + 6, startIndices[1] + 2);

    // Connect right lower to left leg upper
    addNeighbor(startIndices[9] + 4, startIndices[8] + 0);
    addNeighbor(startIndices[9] + 7, startIndices[8] + 3);
    addNeighbor(startIndices[9] + 5, startIndices[8] + 1);
    addNeighbor(startIndices[9] + 6, startIndices[8] + 2);
}

void TaskLect05_Springs::generateScene3_cloth() {
    // Cloth
    m_kSpring = 20.0f;
    m_dampingSpring = 5.0f;
    gEnv->stateSim->dtFixed = 0.004f;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 5;

    auto &ps = particleSystem(m_workOnPsIdx);
    ps.clear();

    const int nX = 35;
    const int nY = 36;
    const float height = 3.0f;
    const float scale = 0.1f;

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
    for (int i = 0; i < nX; i++) {
        for (int j = 0; j < nY; j++) {
            float xPos = i;
            float yPos = j;

            ps.add(glm::vec3(xPos * scale, height, yPos * scale));

            if (yPos == 0) {
                ps.particle(i * nY + j).setStatic(true);
            }
        }
    }
    for (int i = 0; i < nX; i++) {     // rows
        for (int j = 0; j < nY; j++) { // columns
            int idx = i * nY + j;
            // Add left spring
            if (j > 0) {
                addNeighbor(idx, idx - 1);
            }

            // Add right spring
            if (j != nY - 1) {
                addNeighbor(idx, idx + 1);
            }

            // Add up spring
            if (i > 0) {
                addNeighbor(idx, idx - nY);
            }

            // Add below spring
            if (i != nX - 1) {
                addNeighbor(idx, idx + nY);
            }

            // Add top left diagonal
            if (j > 0 && i > 0) {
                addNeighbor(idx, idx - nY - 1);
            }

            // Add second neighbors 
            if (j > 1) {
                addNeighbor(idx, idx - 2);
            }
            if (i > 1) {
                addNeighbor(idx, idx - 2 * nY);
            }
            if (j < nY - 2) {
                addNeighbor(idx, idx + 2);
            }
            if (i < nX - 2) {
                addNeighbor(idx, idx + 2 * nY);
            }
        }
    }
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
