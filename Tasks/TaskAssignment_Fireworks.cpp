#include "TaskAssignment_Fireworks.h"

#include <sstream>
#include <iostream>
#include <glad/gl.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../Context/GlobalEnvironment.h"
#include "../ParticleSystem/ParticleSystem.h"
#include "../Helper/HelperDraw.h"

namespace
{

    glm::vec3 vecRand(float min, float max)
    {
        float r0 = (float)rand() / (float)RAND_MAX * (max - min) + min;
        float r1 = (float)rand() / (float)RAND_MAX * (max - min) + min;
        float r2 = (float)rand() / (float)RAND_MAX * (max - min) + min;
        return glm::vec3(r0, r1, r2);
    }

}

void TaskAssignment_Fireworks::setForces()
{

}

void TaskAssignment_Fireworks::doWork() 
{
    auto& ps = particleSystem(0);
    if (ps.size() == 0) return;
    if (!gEnv->stateSim->isRunning) return;

    if (m_lifetimeCounter > 0)
    {
        m_lifetimeCounter--;
    }

    // todo students

}

void TaskAssignment_Fireworks::draw() const
{
    auto& ps = particleSystem(0);
    if (ps.size() == 0) return;

    const glm::vec4 col0(1, 0, 0, 1);
    const glm::vec4 col1(1, 0, 0, 0);

    const float velScale = 0.1f;
    glm::vec3 posEnd;

    glBegin(GL_LINES);
    for (int i = 0; i < (int)ps.size(); i++)
    {
        glColor4fv(&col0.r);
        glVertex3fv(glm::value_ptr(ps.particle(i).pos()));
        glColor4fv(&col1.r);
        posEnd = ps.particle(i).pos() - ps.particle(i).vel() * velScale;
        glVertex3fv(glm::value_ptr(posEnd));
    }
    glEnd();

}

void TaskAssignment_Fireworks::imGui()
{
    if (ImGui::Button(paramName("Init Fireworks")))
    {
        auto& ps = particleSystem(0);
        ps.clear();

        m_createNParticles = (m_createNParticles / 2) * 2; // ensure an even number of particles
        m_lifetimeCounter = m_lifetime;

        // Add particle

        // todo students
    }
    ImGui::SliderFloat(paramName("Init Speed Y-Direction"), &m_initSpeedY, 1.0f, 30.0f);
    ImGui::SliderInt(paramName("Lifetime (frames)"), &m_lifetime, 10, 100);
    ImGui::SliderInt(paramName("Create n particles"), &m_createNParticles, 2, 100);
}

const char* TaskAssignment_Fireworks::toString() const
{
    std::stringstream ss;

    ss << "Description:" << "\n";
    ss << "A Base for your own tasks..." << "\n";
    ss << "" << "\n";

    m_string = ss.str();
    return m_string.c_str();
}


