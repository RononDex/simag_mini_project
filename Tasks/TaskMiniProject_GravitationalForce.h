#pragma once

#include <string>

#include "ITask.h"

class TaskMiniProject_GravitationalForce : public CTask {
  public:
    TaskMiniProject_GravitationalForce() : CTask() {}

    // ITask
    virtual const char *name() const override {
        return "TaskMiniProject_GravitationalForce";
    }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    long double m_gravityConstant = 6.6743015E-20L;
};
