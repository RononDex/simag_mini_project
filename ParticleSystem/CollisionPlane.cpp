#include "CollisionPlane.h"

#include <imgui/imgui.h>

#include "../Helper/HelperDraw.h"
#include "ParticleSystem.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

CollisionPlane::CollisionPlane() : m_p(glm::vec3(0)), m_n(glm::vec3(0, 1, 0)) {}

CollisionPlane::~CollisionPlane() {}

void CollisionPlane::handleCollisionByVelocity(ParticleSystem &ps,
                                               float kn_normalFriction,
                                               float kt_tangentialFriction) {
    int s = (int)ps.size();
    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();
    auto &states = ps.states();

    auto &planeP = m_p;
    auto &planeN = m_n;

    const glm::vec3 eps = 0.0001f * planeN;

    for (int i = 0; i < s; i++) {
        handleCollisionByVelocity(ps, i, planeP, planeN, kn_normalFriction,
                                  kt_tangentialFriction);
    }
}

void CollisionPlane::handleCollisionByVelocity(ParticleSystem &ps, int idx,
                                               glm::vec3 planeP,
                                               glm::vec3 planeN,
                                               float kn_normalFriction,
                                               float kt_tangentialFriction) {
    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();
    auto &states = ps.states();

    if (states[idx].isStatic())
        return;

    glm::vec3 &p = pos[idx];
    glm::vec3 &v = vel[idx];
    glm::vec3 &f = forces[idx];

    // todo students
    auto d = glm::dot((p - planeP), planeN);
    if (d >= 0) {
        return;
    }
    const glm::vec3 eps = 0.0001f * planeN;
    p = (std::abs(d) * planeN) + p + eps * planeN;
    v = glm::reflect(v, planeN);

    // add tangential and normal friction
    handleFriction(v, f, planeN, kn_normalFriction, kt_tangentialFriction);
    float length = glm::dot(planeN, f);
    if (length < 0) {
        f -= length * planeN;
    }
}

void CollisionPlane::handleCollisionByForce(ParticleSystem &ps,
                                            float forceStrength,
                                            float kn_normalFriction,
                                            float kt_tangentialFriction) {
    // todo students
}

void CollisionPlane::draw() const { Helper::drawPlane(m_p, m_n); }

void CollisionPlane::imGui(std::string const &pre) {
    ImGui::Text("Plane Parameter");
    ImGui::SliderFloat3((pre + "Position").c_str(), &m_p[0], -5.0f, 5.0f);
    ImGui::SliderFloat3((pre + "Normal").c_str(), &m_n[0], -1.0f, 1.0f);
    if (glm::dot(m_n, m_n) == 0) {
        m_n = glm::vec3(0, 1, 0);
    }
    m_n = glm::normalize(m_n);
}

const char *CollisionPlane::toString() const { return "Collision with Plane"; }
