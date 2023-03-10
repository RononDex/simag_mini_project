#include "TaskAssignment_Fireworks.h"

#include <functional>
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
#include "../ParticleSystem/ParticleSystem.h"
#include "glm/ext/vector_float3.hpp"

namespace {

glm::vec3 vecRand(float min, float max) {
    float r0 = (float)rand() / (float)RAND_MAX * (max - min) + min;
    float r1 = (float)rand() / (float)RAND_MAX * (max - min) + min;
    float r2 = (float)rand() / (float)RAND_MAX * (max - min) + min;
    return glm::vec3(r0, r1, r2);
}

} // namespace

void TaskAssignment_Fireworks::setForces() {}

void TaskAssignment_Fireworks::doWork() {
    auto &ps = particleSystem(0);
    if (ps.size() == 0)
        return;
    if (!gEnv->stateSim->isRunning)
        return;

    if (m_lifetimeCounter > 0) {
        m_lifetimeCounter--;
    }

    // todo students
    // Simulate gravity
    for (uint i = 0; i < ps.positions().size(); i++) {
        auto vel = ps.particle(i).vel();
        vel.y = vel.y - 0.1;
        ps.particle(i).vel() = vel;
    }

    if (m_lifetimeCounter <= 0 && !m_exploded) {
        // Let's explode :D
        m_exploded = true;
        float speedRange = 5;
        auto vel_rocket = ps.particle(0).vel();
        auto pos_rocket = ps.particle(0).pos();
        glm::vec3 rocket_impulse =
            glm::vec3(vel_rocket.x * m_rocketMass, vel_rocket.y * m_rocketMass,
                      vel_rocket.z * m_rocketMass);

        // Every particle has the same mass
        // This ensures that the law of mass conservation is met
        float mass_particle = m_rocketMass / m_createNParticles;

        // First calculate the impulse that every particle inherits from the
        // rocket
        glm::vec3 inherited_impulse_particle =
            glm::vec3(rocket_impulse.x / m_createNParticles,
                      rocket_impulse.y / m_createNParticles,
                      rocket_impulse.z / m_createNParticles);

        // Translate the inherited impulse into a velocity, in order to follow
        // the law of Conservation of Impulse
        glm::vec3 inherited_vel_particle =
            glm::vec3(inherited_impulse_particle.x / mass_particle,
                      inherited_impulse_particle.y / mass_particle,
                      inherited_impulse_particle.z / mass_particle);

        // remove rocket
        ps.clear();

        // Create explosion particles in pairs having opposite impulse
        for (int i = 0; i < m_createNParticles / 2; i++) {
            float x_vel = randomNumberInRange(speedRange);
            float y_vel = randomNumberInRange(speedRange);
            float z_vel = randomNumberInRange(speedRange);

            ps.add(glm::vec3(pos_rocket.x, pos_rocket.y, pos_rocket.z),
                   glm::vec3(x_vel + inherited_vel_particle.x,
                             y_vel + inherited_vel_particle.y,
                             z_vel + inherited_vel_particle.z));
            ps.add(pos_rocket, glm::vec3(-x_vel + inherited_vel_particle.x,
                                         -y_vel + inherited_vel_particle.y,
                                         -z_vel + inherited_vel_particle.z));
        }
    }
}

float TaskAssignment_Fireworks::randomNumberInRange(float max) {
    return 2 * ((float)(rand()) / (float)(RAND_MAX)*max) - max;
}

void TaskAssignment_Fireworks::draw() const {
    auto &ps = particleSystem(0);
    if (ps.size() == 0)
        return;

    const glm::vec4 col0(1, 0, 0, 1);
    const glm::vec4 col1(1, 0, 0, 0);

    const float velScale = 0.1f;
    glm::vec3 posEnd;

    glBegin(GL_LINES);
    for (int i = 0; i < (int)ps.size(); i++) {
        glColor4fv(&col0.r);
        glVertex3fv(glm::value_ptr(ps.particle(i).pos()));
        glColor4fv(&col1.r);
        posEnd = ps.particle(i).pos() - ps.particle(i).vel() * velScale;
        glVertex3fv(glm::value_ptr(posEnd));
    }
    glEnd();
}

void TaskAssignment_Fireworks::imGui() {
    if (ImGui::Button(paramName("Init Fireworks"))) {
        auto &ps = particleSystem(0);
        ps.clear();

        m_createNParticles =
            (m_createNParticles / 2) * 2; // ensure an even number of particles
        m_lifetimeCounter = m_lifetime;

        // Add particle
        // todo students
        float x_vel =
            2 * ((float)(rand()) / (float)(RAND_MAX)*m_maxSpeedX) - m_maxSpeedX;
        ps.add(glm::vec3(0, 0, 0), glm::vec3(x_vel, m_initSpeedY, 0));
        m_exploded = false;
    }
    ImGui::SliderFloat(paramName("Init Speed Y-Direction"), &m_initSpeedY, 1.0f,
                       30.0f);
    ImGui::SliderInt(paramName("Lifetime (frames)"), &m_lifetime, 10, 100);
    ImGui::SliderInt(paramName("Create n particles"), &m_createNParticles, 2,
                     100);
}

const char *TaskAssignment_Fireworks::toString() const {
    std::stringstream ss;

    ss << "Description:"
       << "\n";
    ss << "Launches a rocket from (0, 0, 0) and explodes after configured "
          "lifetime is reached"
       << "\n";
    ss << ""
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
