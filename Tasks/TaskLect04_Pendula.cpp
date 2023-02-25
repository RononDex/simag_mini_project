#include "TaskLect04_Pendula.h"

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

    void createScene(ParticleSystem& ps, float length, int noOfPendulas)
    {
        ps.clear();

        // todo students
    }

}

void TaskLect04_Pendula::setForces()
{
    auto& ps = particleSystem(0);
    int s = (int)ps.size();
    if (ps.size() <= 1) return; // nothing to to

    // Add force & length constraint between particles.
    // All particles should have the distance m_pendulaLength to their neighbors
    
    // todo students
}

void TaskLect04_Pendula::doWork() 
{
}

void TaskLect04_Pendula::draw() const
{
    auto& ps = particleSystem(0);
    int s = (int)ps.size();
    if (ps.size() <= 1) return; // nothing to to

    // Draw line between particles
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(m_lineWidth);
    for (int i = 0; i < s-1; i++)
    {
        Helper::drawLine2d(ps.positions()[i].x, ps.positions()[i].y, ps.positions()[i+1].x, ps.positions()[i + 1].y);
    }
    glLineWidth(1.0f);
}

void TaskLect04_Pendula::imGui()
{
    if (ImGui::Button(paramName("Create Scene"))) { createScene(particleSystem(0), m_pendulaLength, m_noOfPendulas); }
    ImGui::SliderFloat(paramName("Pendula Length"), &m_pendulaLength, 0.5f, 3.0f);
    ImGui::SliderInt(paramName("No of Pendulas"), &m_noOfPendulas, 2, 5);
}

const char* TaskLect04_Pendula::toString() const
{
    std::stringstream ss;

    ss << "Description:" << "\n";
    ss << "Simple Pendula in PS0\n";

    m_string = ss.str();
    return m_string.c_str();
}


