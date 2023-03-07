#include "TaskLect03_Billiard.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <iostream>
#include <sstream>

#include "../Helper/HelperDraw.h"

// ----------------------------------------------------------------------------

namespace {
// Helper: Random number from -1 to 1
float rand_m11() { return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f; }
// Helper: Random number from 0 to 1
float rand_01() { return (float)rand() / (float)RAND_MAX; }
} // namespace

void TaskLect03_Billiard::addBall(glm::vec3 const &pos, glm::vec3 const &vel,
                                  float radius, float mass) {
  particleSystem(0).add(pos);
  m_balls.emplace_back(Ball(particleSystem(0),
                            (int)particleSystem(0).size() - 1, pos, vel, radius,
                            mass));
}

void TaskLect03_Billiard::createScene() {
  particleSystem(0).clear();
  m_balls.clear();

  const float r = 0.49f;
  const float mass = 1.0f;
  const glm::vec3 vel0(0, 0, 0);
  const glm::vec3 velStart(-8.0f, 0.0f, 0.0f);

  // Create simple billiard scene:
  // 6 balls in a triangle on the left (velocities=[vel0])
  // 1 ball on the right (velocities=[velStart])

  // Example to add a ball:
  addBall(glm::vec3(-3, 0, 0), vel0, r, mass);
  addBall(glm::vec3(-3, 1, 0), vel0, r, mass);
  addBall(glm::vec3(-3, -1, 0), vel0, r, mass);

  addBall(glm::vec3(-2, 0.5, 0), vel0, r, mass);
  addBall(glm::vec3(-2, -0.5, 0), vel0, r, mass);

  addBall(glm::vec3(-1, 0, 0), vel0, r, mass);

  addBall(glm::vec3(3, 0.5, 0), velStart, r, mass);
}

void TaskLect03_Billiard::setRandomVelocities() {
  float maxSpeed = 10.0f;

  // Set random velocities for all balls
  // See helper methods above:
  // rand_m11() => return random number from -1..1
  // rand_01()  => return random number from 0..1

  // todo students
}

void TaskLect03_Billiard::setRandomRadius() {
  float maxRadius = 0.8f;
  float minRadius = 0.2f;

  // Set random radius for all balls

  // todo students
}

void TaskLect03_Billiard::handleBallBallCollision(int i, int j) {
  float restitution = m_restitution;

  auto &ball1 = m_balls[i];
  auto &ball2 = m_balls[j];

  auto AB_n = glm::normalize(ball2.pos() - ball1.pos());
  auto BA_n = glm::normalize(ball1.pos() - ball2.pos());

  auto v_A = ball1.vel();
  auto v_B = ball2.vel();

  // todo Fill in
  if (glm::distance(ball1.pos(), ball2.pos()) <
      ball1.radius() + ball2.radius()) {

    auto v_A_y = glm::vec3(0);
    auto v_B_y = glm::vec3(0);

    auto v_A_x = glm::vec3(0);
    auto v_B_x = glm::vec3(0);

    v_A_x = AB_n * glm::dot(AB_n, v_A);
    v_B_x = BA_n * glm::dot(BA_n, v_B);

    v_A_y = v_A - v_A_x;
    v_B_y = v_B - v_B_x;

    auto v_A_x_new = glm::vec3(0);
    auto v_B_x_new = glm::vec3(0);

    auto m_A = ball1.mass();
    auto m_B = ball2.mass();

    float e = 1.0;
    v_A_x_new = ((m_A * v_A_x) + (m_B * v_B_x) - (e * m_B * (v_A_x - v_B_x))) /
                (m_A + m_B);
    v_B_x_new = ((m_A * v_A_x) + (m_B * v_B_x) - (e * m_A * (v_B_x - v_A_x))) /
                (m_A + m_B);

    auto v_A_new = glm::vec3(0);
    auto v_B_new = glm::vec3(0);

    v_A_new = v_A_x_new + v_A_y;
    v_B_new = v_B_x_new + v_B_y;

    ball1.vel() = v_A_new;
    ball2.vel() = v_B_new;
  }
  // todo students
}

void TaskLect03_Billiard::handleBallWallCollision(int i) {
  Ball &ball = m_balls[i];
  glm::vec3 &pos = ball.pos();
  glm::vec3 &vel = ball.vel();
  float r = ball.radius();

  glm::vec3 const &wallMin = m_wallMin;
  glm::vec3 const &wallMax = m_wallMax;

  if (pos.x - r <= wallMin.x) {
    pos.x = wallMin.x + r;
    vel.x = -vel.x;
  }
  if (pos.y - r <= wallMin.y) {
    pos.y = wallMin.y + r;
    vel.y = -vel.y;
  }
  if (pos.x + r >= wallMax.x) {
    pos.x = wallMax.x - r;
    vel.x = -vel.x;
  }
  if (pos.y + r >= wallMax.y) {
    pos.y = wallMax.y - r;
    vel.y = -vel.y;
  }
}

// ----------------------------------------------------------------------------

void TaskLect03_Billiard::setForces() {
  int nBalls = (int)particleSystem(0).size();
  if (nBalls != (int)m_balls.size())
    return;

  // Ball-Ball Collision
  if (nBalls > 1) {
    for (int i = 0; i < nBalls - 1; i++) {
      for (int j = i + 1; j < nBalls; j++) {
        handleBallBallCollision(i, j);
      }
    }
  }

  // Ball-Wall Collision
  for (int i = 0; i < nBalls; i++) {
    handleBallWallCollision(i);
  }

  // Simple Damping by reducing velocity per frame
  auto &vel = particleSystem(0).velocities();
  for (int i = 0; i < nBalls; i++) {
    vel[i] *= m_friction;
  }
}

void TaskLect03_Billiard::doWork() {}

void TaskLect03_Billiard::draw() const {
  int nBalls = (int)particleSystem(0).size();
  if (nBalls != (int)m_balls.size())
    return;

  // Draw Balls
  glColor3f(1, 1, 1);
  glLineWidth(3.0f);
  for (int i = 0; i < nBalls; i++) {
    Helper::drawCircle2d(particleSystem(0).positions()[i], m_balls[i].radius(),
                         20);
  }
  glLineWidth(1.0f);

  // Draw Wall
  glColor3f(0, 1, 0);
  glLineWidth(3.0f);
  Helper::drawBox2d(m_wallMin.x, m_wallMin.y, m_wallMax.x, m_wallMax.y);
  glLineWidth(1.0f);
  glColor3f(1, 1, 1);
}

void TaskLect03_Billiard::imGui() {
  if (ImGui::Button("Create Scene##Billiard")) {
    createScene();
  }
  if (ImGui::Button("Set Random Velocities##Billiard")) {
    setRandomVelocities();
  }
  if (ImGui::Button("Set Random Radius##Billiard")) {
    setRandomRadius();
  }
  ImGui::SliderFloat("Restitution##Billiard", &m_restitution, 0.1f, 1.0f);
  ImGui::SliderFloat("Friction##Billiard", &m_friction, 0.99f, 1.0f);
}

const char *TaskLect03_Billiard::toString() const {
  std::stringstream ss;
  ss << "Billiard simulation in 2D\n";
  ss << "Using conservation of momentum\n";
  m_string = ss.str();
  return m_string.c_str();
}
