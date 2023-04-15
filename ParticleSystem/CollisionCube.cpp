#include "CollisionCube.h"

#include <cstdlib>
#include <imgui/imgui.h>

#include "../Helper/HelperDraw.h"
#include "CollisionPlane.h"
#include "ParticleSystem.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

namespace {
void ensureMinMax(glm::vec3 &p0, glm::vec3 &p1) {
    glm::vec3 a = p0;
    glm::vec3 b = p1;
    for (int i = 0; i < 3; i++) {
        p0[i] = std::min<float>(a[i], b[i]);
        p1[i] = std::max<float>(a[i], b[i]);
    }
}

} // namespace

CollisionCube::CollisionCube()
    : m_p0(glm::vec3(0, 0, -1)), m_p1(glm::vec3(3, 1, 1)) {
    ensureMinMax(m_p0, m_p1);
}

CollisionCube::~CollisionCube() {}

void CollisionCube::updatePlanes() {
    glm::vec3 center = (m_p0 + m_p1) * 0.5f;
    float length = std::abs((m_p0 - m_p1).x);
    float width = std::abs((m_p0 - m_p1).y);
    float height = std::abs((m_p0 - m_p1).z);

    m_plane_positions[0] =
        glm::vec3(center.x, center.y, center.z + (height / 2.0f));
    m_plane_positions[1] =
        glm::vec3(center.x, center.y - (width / 2.0f), center.z);
    m_plane_positions[2] =
        glm::vec3(center.x + (length / 2.0f), center.y, center.z);
    m_plane_positions[3] =
        glm::vec3(center.x, center.y + (width / 2.0f), center.z);
    m_plane_positions[4] =
        glm::vec3(center.x - (length / 2.0f), center.y, center.z);
    m_plane_positions[5] =
        glm::vec3(center.x, center.y, center.z - (height / 2.0f));

    m_plane_normals[0] = glm::vec3(0, 0, 1);
    m_plane_normals[1] = glm::vec3(0, -1, 0);
    m_plane_normals[2] = glm::vec3(1, 0, 0);
    m_plane_normals[3] = glm::vec3(0, 1, 0);
    m_plane_normals[4] = glm::vec3(-1, 0, 0);
    m_plane_normals[5] = glm::vec3(0, 0, -1);
}

void CollisionCube::handleCollisionByVelocity(ParticleSystem &ps,
                                              float kn_normalFriction,
                                              float kt_tangentialFriction) {

    this->updatePlanes();

    int s = (int)ps.size();
    if (s < 1)
        return;

    auto &pos = ps.positions();
    auto &vel = ps.velocities();
    auto &forces = ps.forces();
    auto &states = ps.states();

    auto &cubeP0 = m_p0;
    auto &cubeP1 = m_p1;

    const float eps = 0.0001f;

    for (int i = 0; i < s; i++) {
        if (states[i].isStatic())
            continue; // Do not collide static particles

        glm::vec3 &p = pos[i];
        glm::vec3 &v = vel[i];
        glm::vec3 &f = forces[i];

        // todo students
        float distance_to_planes[m_number_of_planes];
        bool didCollide = isCollided(ps, i, distance_to_planes);

        if (didCollide) {
            // find smallest distance
            float smallestDistance = std::numeric_limits<float>::max();
            int closestPlaneIndex = 0;
            for (int j = 0; j < m_number_of_planes; j++) {
                if (std::abs(distance_to_planes[j]) < smallestDistance) {
                    smallestDistance = std::abs(distance_to_planes[j]);
                    closestPlaneIndex = j;
                }
            }

            // Move particle to the surface of the closest plane
            pos[i] = std::abs(smallestDistance) *
                         m_plane_normals[closestPlaneIndex] +
                     pos[i] + eps * m_plane_normals[closestPlaneIndex];
            vel[i] = glm::reflect(vel[i], m_plane_normals[closestPlaneIndex]);

            handleFriction(vel[i], forces[i],
                           m_plane_normals[closestPlaneIndex],
                           kn_normalFriction, kt_tangentialFriction);
        }
    }
}

bool CollisionCube::isCollided(ParticleSystem &ps, int idx,
                               float *distance_to_planes) {
    bool didCollide = true;
    glm::vec3 particle_position = ps.positions()[idx];
    for (int j = 0; j < m_number_of_planes; j++) {
        glm::vec3 d = particle_position - m_plane_positions[j];
        float distance = glm::dot(d, m_plane_normals[j]);
        distance_to_planes[j] = distance;
        if (distance >= 0) {
            didCollide = false;
            break;
        }
    }

    return didCollide;
}

void CollisionCube::handleCollisionByForce(ParticleSystem &ps,
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

    auto &cubeP0 = m_p0;
    auto &cubeP1 = m_p1;

    const float eps = 0.0001f;

    for (int i = 0; i < s; i++) {
        if (states[i].isStatic())
            continue; // Do not collide static particles

        glm::vec3 &p = pos[i];
        glm::vec3 &v = vel[i];
        glm::vec3 &f = forces[i];

        // todo students
        float distance_to_planes[m_number_of_planes];
        bool didCollide = isCollided(ps, i, distance_to_planes);

        if (didCollide) {
            // find smallest distance
            float smallestDistance = std::numeric_limits<float>::max();
            int closestPlaneIndex = 0;
            for (int j = 0; j < m_number_of_planes; j++) {
                if (std::abs(distance_to_planes[j]) < smallestDistance) {
                    smallestDistance = std::abs(distance_to_planes[j]);
                    closestPlaneIndex = j;
                }
            }

            // Move particle to the surface of the closest plane
            forces[i] += forceStrength * smallestDistance *
                         m_plane_normals[closestPlaneIndex];

            handleFriction(vel[i], forces[i],
                           m_plane_normals[closestPlaneIndex],
                           kn_normalFriction, kt_tangentialFriction);
        }
    }
}

void CollisionCube::draw() const { Helper::drawCube(m_p0, m_p1); }

void CollisionCube::imGui(std::string const &pre) {
    ImGui::Text("Cube Parameter");
    ImGui::SliderFloat3((pre + "P0").c_str(), &m_p0[0], -5.0f, 5.0f);
    ImGui::SliderFloat3((pre + "P1").c_str(), &m_p1[0], -5.0f, 5.0f);

    glm::vec3 center = (m_p0 + m_p1) * 0.5f;
    ImGui::SliderFloat3((pre + "Center").c_str(), &center[0], -5.0f, 5.0f);
    glm::vec3 dx = (m_p1 - m_p0) * 0.5f;
    m_p0 = center - dx;
    m_p1 = center + dx;

    ensureMinMax(m_p0, m_p1);
}

const char *CollisionCube::toString() const { return "Collision with Cube"; }
