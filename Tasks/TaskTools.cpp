#include "TaskTools.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Context/GlobalEnvironment.h"
#include "../ParticleSystem/ParticleSystemContainer.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

namespace {
void setVelocitiesRadial(ParticleSystem &ps) {
  for (auto &it : ps.states()) {
    glm::vec3 velocity = it.vel();
    glm::vec3 pos = it.pos();
    velocity.x = pos.x;
    velocity.y = pos.y;
    velocity.z = pos.z;
    it.vel() = velocity;
  }
}

void setVelocitiesRadialNormalized(ParticleSystem &ps) {
  setVelocitiesRadial(ps);

  for (auto &it : ps.states()) {
    // todo students
    glm::vec3 normedVelocity = glm::normalize(it.vel());
    it.vel() = normedVelocity;
  }
}

void setVelocitiesPerpendicular(ParticleSystem &ps) {
  glm::vec3 zAxis = glm::vec3(0, 0, 1);
  for (auto &it : ps.states()) {
    // todo students
    glm::vec3 pos = it.pos();
    glm::vec3 radialVelocity = glm::cross(pos, zAxis);
    it.vel() = radialVelocity;
  }
}

void setVelocitiesPerpendicularNormalized(ParticleSystem &ps) {
  setVelocitiesPerpendicular(ps);

  for (auto &it : ps.states()) {
    // todo students
    glm::vec3 normedVelocity = glm::normalize(it.vel());
    it.vel() = normedVelocity;
  }
}

void invertVelocities(ParticleSystem &ps) {
  for (auto &it : ps.states()) {
    glm::vec3 velocity = it.vel();
    velocity.x = -velocity.x;
    velocity.y = -velocity.y;
    velocity.z = -velocity.z;

    it.vel() = velocity;
  }
}

} // namespace

void TaskTools::clearPs() { particleSystem(m_workOnPsIdx).clear(); }

void TaskTools::generateCircle(int n) {
  auto &ps = particleSystem(m_workOnPsIdx);

  float radius = 2.0f;
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
  float dALpha = 2 * glm::pi<float>() / n;

  // Generate n particles on a circle around the root.
  for (int i = 0; i < n; i++) {
    float angle = i * dALpha;
    glm::vec3 pos = glm::vec3(radius * sin(angle), radius * cos(angle), 0);
    particleSystem(0).add(pos);
  }
}

void TaskTools::generateQuad() {
  auto &ps = particleSystem(m_workOnPsIdx);

  float dx = 0.1f;
  glm::vec3 pMin(-1.0f, -1.0f, 0);
  glm::vec3 pMax(1.0f, 1.0f, 0);

  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

  // Generate a quad of particles with distance dx within pMin and pMax.
  float x = pMin.x;
  float y = pMin.y;

  while (y < pMax.y) {
    while (x < pMax.x) {

      glm::vec3 pos = glm::vec3(x, y, 0);

      particleSystem(0).add(pos, color);

      x += dx;
    }
    x = pMin.x;
    y += dx;
  }
}

void TaskTools::setForces() {}

void TaskTools::doWork() {}

void TaskTools::imGui() {
  ImGui::SliderInt(paramName("Work on PS Idx"), &m_workOnPsIdx, 0,
                   noOfPs() - 1);

  ImGui::Separator();
  if (ImGui::Button(paramName("Clear PS"))) {
    clearPs();
  }

  ImGui::Separator();
  if (ImGui::Button(paramName("Generate Circle Hi-Res"))) {
    generateCircle(100);
  }
  if (ImGui::Button(paramName("Generate Circle Low-Res"))) {
    generateCircle(10);
  }
  if (ImGui::Button(paramName("Generate Quad"))) {
    generateQuad();
  }

  ImGui::Separator();
  if (ImGui::Button(paramName("Set Velocities Radial"))) {
    setVelocitiesRadial(particleSystem(m_workOnPsIdx));
  }
  if (ImGui::Button(paramName("Set Velocities Radial Normalized"))) {
    setVelocitiesRadialNormalized(particleSystem(m_workOnPsIdx));
  }
  if (ImGui::Button(paramName("Set Velocities Perpendicular"))) {
    setVelocitiesPerpendicular(particleSystem(m_workOnPsIdx));
  }
  if (ImGui::Button(paramName("Set Velocities Perpendicular Normalized"))) {
    setVelocitiesPerpendicularNormalized(particleSystem(m_workOnPsIdx));
  }
  if (ImGui::Button(paramName("Invert Velocities"))) {
    invertVelocities(particleSystem(m_workOnPsIdx));
  }
}

const char *TaskTools::toString() const {
  std::stringstream ss;
  ss << "Tools"
     << "\n";

  m_string = ss.str();
  return m_string.c_str();
}

void TaskTools::draw() const {}
