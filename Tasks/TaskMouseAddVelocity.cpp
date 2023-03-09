#include "TaskMouseAddVelocity.h"

#include <cmath>
#include <glad/gl.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include "../Helper/HelperDraw.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

glm::vec2 TaskMouseAddVelocity::circleCenterFromMouse() const {
    glm::vec2 center = gEnv->camera->SSToWorld2d(gEnv->stateGui->mousePos);
    return center;
}

void TaskMouseAddVelocity::setForces() {
    auto &ps = particleSystem(0);

    for (int i = 0; i < (int)ps.size(); i++) // Run over all particles
    {
        Particle &p = ps.particle(i); // Get Particle i

        glm::vec3 &pos = p.pos(); // Reference to position
        glm::vec3 &vel = p.vel(); // Reference to velocity

        const glm::vec2 circleCenter = circleCenterFromMouse();
        const glm::vec3 circleCenter3d(circleCenter, 0.0f);
        const float radius = m_radius;


        // todo students
        // Apply dampening
        vel.x = vel.x * m_damping;
        vel.y = vel.y * m_damping;
        vel.z = vel.z * m_damping;
        p.vel() = vel;

        glm::vec2 pos_2d = glm::vec2(pos.x, pos.y);
        float distance = glm::distance(pos_2d, circleCenter);

        // Calculate the period for cosine, so that 1 equals cos(0) and 0 =
        // cos(radius) (or so that radius = T/2)
        float cos_period = 2 * M_PI / (2 * radius);

        // Offset the cosine function by 1 (so that minima is at 0 and max at 2)
        float cos_offset = 1;

        // Stretch the resulting cos(x) value by this factor
        // This ensure that cos(0) after transformation is again 1 and cos(T/2)
        // is 0
        float cos_stretch_factor = 0.5f;

        float cos_transform =
            cos_stretch_factor * (std::cos(cos_period * distance) + cos_offset);

        if (distance < radius) {
            float vel_magnitude = m_intensity * cos_transform;

            glm::vec3 radialVel_normalized = glm::normalize(glm::vec3(
                pos.x - circleCenter.x, pos.y - circleCenter.y, 0.0f));

            glm::vec3 new_vel =
                glm::vec3(radialVel_normalized.x * vel_magnitude + vel.x,
                          radialVel_normalized.y * vel_magnitude + vel.y,
                          radialVel_normalized.z * vel_magnitude + vel.z);

            p.vel() = new_vel;
        }
    }
}

void TaskMouseAddVelocity::doWork() {}

void TaskMouseAddVelocity::draw() const {
    glm::vec2 center = circleCenterFromMouse();
    Helper::drawCircle2d(glm::vec3(center, 0), m_radius, 50);
}

void TaskMouseAddVelocity::imGui() {
    ImGui::SliderFloat(paramName("Radius"), &m_radius, 0.1f, 3.0f);
    ImGui::SliderFloat(paramName("Intensity"), &m_intensity, 0.01f, 1.0f);
    ImGui::Separator();
    ImGui::SliderFloat(paramName("Velocity Damping"), &m_damping, 0.8f, 1.0f);
}

const char *TaskMouseAddVelocity::toString() const {
    std::stringstream ss;
    ss << "Add velocity to particles in circle around mouse-pos.\n";
    ss << "Additionally, adds simple damping to all velocities of all "
          "particles.\n";
    ss << "- Therefore, velocities are reduced slightly per step (by factor < "
          "1.0).\n";
    ss << "--------------------\n";
    ss << "Works on ParticleSystem 0."
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
