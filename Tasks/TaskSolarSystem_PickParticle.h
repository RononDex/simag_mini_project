#pragma once

#include "../ParticleSystem/SolarSystemParticle.h"
#include "ITask.h"

class TaskSolarSystem_PickParticle : public CTask {
  public:
    TaskSolarSystem_PickParticle() : CTask(), m_clickPos(0) {}

    // ITask
    virtual const char *name() const override {
        return "TaskSolarSystem_PickParticle";
    }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    bool isValidParticleIdx(int idx) const;
    void findClosestParticle();

    int m_workOnPsIdx = 0;
    int m_closestParticleIdx = -1;
    int m_selectedParticleIdx = -1;
    float m_Mass = -1;
    float m_accelerationX = 0;
    float m_accelerationY = 0;
    float m_accelerationZ = 0;
    int m_onClickWhatToDo = 0;

    bool m_selectedParticleStateOld = false;

    glm::vec2 m_clickPos;

    void explodeSelectedObject();
    float randomNumberInRange(float max);

    void deleteParticle(int idx);
    long double calcEscapeVelocity(SolarSystemParticle *particle,
                                   long double distance);
};
