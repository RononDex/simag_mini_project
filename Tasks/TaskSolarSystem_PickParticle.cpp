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
#include <math.h>
#include <string>

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

        m_selectedParticleIdx = m_closestParticleIdx;
        auto &particle = gEnv->solarSystemPS.get(m_selectedParticleIdx);
        m_Mass = particle.getMass() / 1e24L;
    }

    if (m_Mass >= 0 && isValidParticleIdx(m_selectedParticleIdx)) {
        auto &particle = gEnv->solarSystemPS.get(m_selectedParticleIdx);
        particle.setMass(m_Mass * 1e24L);
    }
}

void TaskSolarSystem_PickParticle::explodeSelectedObject() {
    auto particle = gEnv->solarSystemPS.get(m_selectedParticleIdx);
    this->deleteParticle(m_selectedParticleIdx);
    m_selectedParticleIdx = -1;

    int number_of_new_particles = 50;
    long double mass_per_particle =
        particle.getMass() / number_of_new_particles;
    long double move_by_dist_fact = 100000;
    long double escapeVelocity =
        calcEscapeVelocity(&particle, move_by_dist_fact);
    long double max_explosion_velocity = 2 * escapeVelocity;

    auto &pos = particle.getPosition();
    auto &vel = particle.getVelocity();
    auto &color = particle.getColor();

    for (int i = 0; i < number_of_new_particles; i += 2) {

        float x_vel =
            randomNumberInRange(max_explosion_velocity) + escapeVelocity;
        float y_vel =
            randomNumberInRange(max_explosion_velocity) + escapeVelocity;
        float z_vel =
            randomNumberInRange(max_explosion_velocity) + escapeVelocity;

        char new_name[100];
        std::stringstream ss_name1;
        ss_name1 << particle.getName();
        ss_name1 << " Fragment ";
        ss_name1 << std::to_string(i);
        m_string = ss_name1.str();

        char new_name2[100];
        std::snprintf(new_name2, 100, "%s fragment %d", particle.getName(),
                      i + 1);

        // Create a pair of particles in opposite directions
        gEnv->solarSystemPS.add(
            glm::vec<3, long double>(pos.x + (x_vel * move_by_dist_fact),
                                     pos.y + (y_vel * move_by_dist_fact),
                                     pos.z + (z_vel * move_by_dist_fact)),
            glm::vec<3, long double>(x_vel + vel.x, y_vel + vel.y,
                                     z_vel + vel.z),
            glm::vec<3, long double>(), mass_per_particle, color, NULL);

        gEnv->solarSystemPS.add(
            glm::vec<3, long double>(pos.x - (x_vel * move_by_dist_fact),
                                     pos.y - (y_vel * move_by_dist_fact),
                                     pos.z - (z_vel * move_by_dist_fact)),
            glm::vec<3, long double>(-x_vel + vel.x, -y_vel + vel.y,
                                     -z_vel + vel.z),
            glm::vec<3, long double>(), mass_per_particle, color, NULL);
    }
}

void TaskSolarSystem_PickParticle::deleteParticle(int idx) {
    gEnv->solarSystemPS.remove(m_selectedParticleIdx);
}

float TaskSolarSystem_PickParticle::randomNumberInRange(float max) {
    return 2 * ((float)(rand()) / (float)(RAND_MAX)*max) - max;
}

long double
TaskSolarSystem_PickParticle::calcEscapeVelocity(SolarSystemParticle *particle,
                                                 long double distance) {
    return std::sqrt(particle->getMass() * (long double)6.6743e-20 / distance);
}

void TaskSolarSystem_PickParticle::imGui() {

    // Show changeble states of selected particle
    if (isValidParticleIdx(m_selectedParticleIdx)) {

        const int idx = m_selectedParticleIdx;
        auto &solarParticle = gEnv->solarSystemPS.get(idx);

        if (ImGui::Button(paramName("Remove object"))) {
            deleteParticle(m_selectedParticleIdx);
            return;
        }

        if (ImGui::Button(paramName("Explode object"))) {
            explodeSelectedObject();
            return;
        }

        ImGui::InputScalarN(paramName("Mass in 10^24 kg"), ImGuiDataType_Float, &m_Mass, 1);

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
