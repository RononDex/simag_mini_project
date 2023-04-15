#include "CollisionSphere.h"

#include <imgui/imgui.h>

#include "../Helper/HelperDraw.h"
#include "CollisionPlane.h"
#include "ParticleSystem.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

CollisionSphere::CollisionSphere() : m_p(glm::vec3(0)), m_r(1.0f) {}

CollisionSphere::~CollisionSphere() {}

void CollisionSphere::handleCollisionByVelocity(ParticleSystem &ps,
                                                float kn_normalFriction,
                                                float kt_tangentialFriction) {
    int s = (int)ps.size();
    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();
    auto &states = ps.states();

    auto &sphereP = m_p;
    auto &sphereR = m_r;

    const float eps = 0.0001f;

    glm::vec3 vecToCenter(0);
    glm::vec3 sphereNormal(0);

    auto collisionPlane = new CollisionPlane();

    for (int i = 0; i < s; i++) {
        if (states[i].isStatic())
            continue; // Do not collide static particles

        glm::vec3 &p = pos[i];
        glm::vec3 &v = vel[i];
        glm::vec3 &f = forces[i];

        // todo students
        glm::vec3 delta = p - sphereP;
        glm::vec3 planeN = glm::normalize(delta);
        glm::vec3 planeP = sphereP + (sphereR * planeN);

        collisionPlane->handleCollisionByVelocity(
            ps, i, planeP, planeN, kn_normalFriction, kt_tangentialFriction);
    }

    delete collisionPlane;
}

void CollisionSphere::handleCollisionByForce(ParticleSystem &ps,
                                             float forceStrength,
                                             float kn_normalFriction,
                                             float kt_tangentialFriction) {
    int s = (int)ps.size();
    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();
    auto &states = ps.states();

    auto &sphereP = m_p;
    auto &sphereR = m_r;

    const float eps = 0.0001f;

    glm::vec3 vecToCenter(0);
    glm::vec3 sphereNormal(0);

    auto collisionPlane = new CollisionPlane();

    for (int i = 0; i < s; i++) {
        if (states[i].isStatic())
            continue; // Do not collide static particles

        glm::vec3 &p = pos[i];
        glm::vec3 &v = vel[i];
        glm::vec3 &f = forces[i];

        // todo students
        glm::vec3 delta = p - sphereP;
        glm::vec3 planeN = glm::normalize(delta);
        glm::vec3 planeP = sphereP + (sphereR * planeN);

        // todo students
        auto d = glm::dot((p - planeP), planeN);

        // If d is >= 0, we have no collision
        if (d >= 0) {
            continue;
        }

        const glm::vec3 eps = 0.0001f * planeN;

        forces[i] -= forceStrength * d * planeN;

        // add tangential and normal friction
        handleFriction(v, f, planeN, kn_normalFriction, kt_tangentialFriction);
        float length = glm::dot(planeN, f);
        if (length < 0) {
            f -= length * planeN;
        }
    }

    delete collisionPlane;
}

void CollisionSphere::draw() const { Helper::drawSphere(m_p, m_r); }

void CollisionSphere::imGui(std::string const &pre) {
    ImGui::Text("Sphere Parameter");
    ImGui::SliderFloat3((pre + "Position").c_str(), &m_p[0], -5.0f, 5.0f);
    ImGui::SliderFloat((pre + "Radius").c_str(), &m_r, 0.1f, 2.0f);
}

const char *CollisionSphere::toString() const {
    return "Collision with Sphere";
}
