#include "TaskSolarSystem_PickParticle.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../Context/GlobalEnvironment.h"
#include "../ParticleSystem/ParticleSystemContainer.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"

bool TaskSolarSystem_PickParticle::isValidParticleIdx(int idx) const {
    if (idx >= 0 && idx < (int)particleSystem(m_workOnPsIdx).size()) {
        return true;
    }
    return false;
}

void TaskSolarSystem_PickParticle::setForces() {}

void TaskSolarSystem_PickParticle::findClosestParticle() {
    // Search for closest particle according to mouse position, returns idx

    auto *cam = gEnv->camera;
    const int width = cam->sizeWindow().x;
    const int height = cam->sizeWindow().y;
    glm::vec3 posMouse(gEnv->stateGui->mousePos, 0.0f);
    posMouse.x = posMouse.x / (float)width * 2.0f - 1.0f;
    posMouse.y = ((float)height - posMouse.y) / (float)height * 2.0f - 1.0f;

    glm::mat4 m = cam->getProjection() * cam->getView();

    ParticleSystem &ps = particleSystem(m_workOnPsIdx);

    float minDistSqr = FLT_MAX;
    int idx = 0;
    for (auto const &it : ps.positions()) {
        glm::vec4 temp4 = m * glm::vec4(it, 1.0f);
        glm::vec3 temp = glm::vec3(temp4) / temp4.w;
        temp.z = 0;
        temp = temp - posMouse;
        float distSqr = glm::dot(temp, temp);
        if (distSqr < minDistSqr) {
            m_closestParticleIdx = idx;
            minDistSqr = distSqr;
        }
        idx++;
    }
}

void TaskSolarSystem_PickParticle::doWork() {
    // only work on first step in case of several steps per frame
    if (gEnv->stateSim->dtActualStepPerFrame != 0)
        return;

    ParticleSystem &ps = particleSystem(m_workOnPsIdx);
    m_closestParticleIdx = -1;

    findClosestParticle();

    const bool is2d = gEnv->camera->is2d();
    const bool is3d = gEnv->camera->is3d();

    if (gEnv->stateGui->mouseButtonLeftClick) {
        m_clickPos = gEnv->stateGui->mousePos;

        switch (m_onClickWhatToDo) {
        case 0: // select
            m_selectedParticleIdx = m_closestParticleIdx;
            break;
        default:
            break;
        }
    }
}

void TaskSolarSystem_PickParticle::imGui() {

    // Show changeble states of selected particle
    if (isValidParticleIdx(m_selectedParticleIdx)) {
        auto &ps = particleSystem(m_workOnPsIdx);
        const int idx = m_selectedParticleIdx;
        ImGui::SliderFloat3(paramName("Pos"), &ps.positions()[idx][0], -10.0f,
                            10.0f);
        ImGui::SliderFloat3(paramName("Vel"), &ps.velocities()[idx][0], -10.0f,
                            10.0f);
        ImGui::Text("%s %s", "Force", glm::to_string(ps.forces()[idx]).c_str());
        ImGui::ColorEdit4(paramName("Color"), &ps.colors()[idx][0]);

        ImGui::SliderFloat(paramName("Mass"), &ps.mass()[idx], -10.0f, 10.0f);

        ImGui::Separator();
        ImGui::Text("Selected Particle");
        ImGui::Text(
            "%s",
            gEnv->solarSystemPS.get(m_selectedParticleIdx).toString().c_str());
    }
}

const char *TaskSolarSystem_PickParticle::toString() const {
    std::stringstream ss;
    ss << "Select closest particle to mouse cursor\n";
    ss << "Finds the corresponding particle in the SolarSystem \n";
    ss << "Particle System and displays information \n";
    ss << "--------------------------------------- \n";

    ParticleSystem const &ps = particleSystem(m_workOnPsIdx);
    if (ps.isValidIdx(m_closestParticleIdx)) {
        const int &idx = m_closestParticleIdx;

        auto &sp = gEnv->solarSystemPS.get(idx);
        ss << sp.toString() << "\n";
    }

    m_string = ss.str();
    return m_string.c_str();
}

void TaskSolarSystem_PickParticle::draw() const {
    if (isValidParticleIdx(m_closestParticleIdx)) {
        glm::vec3 posParticle =
            particleSystem(m_workOnPsIdx).positions()[m_closestParticleIdx];

        glPointSize(14.0f);
        glColor4f(1, 0, 0, 1);
        glBegin(GL_POINTS);
        glVertex3fv(glm::value_ptr(posParticle));
        glEnd();
        glPointSize(12.0f);
    }

    if (isValidParticleIdx(m_selectedParticleIdx)) {
        glm::vec3 posParticle =
            particleSystem(m_workOnPsIdx).positions()[m_selectedParticleIdx];

        glPointSize(14.0f);
        glColor4f(0, 1, 0, 1);
        glBegin(GL_POINTS);
        glVertex3fv(glm::value_ptr(posParticle));
        glEnd();
        glPointSize(12.0f);
    }
}
