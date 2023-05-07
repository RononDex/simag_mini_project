#pragma once

#include <string>

#include "ITask.h"

class TaskMiniProject_Integrator : public CTask {
  public:
    TaskMiniProject_Integrator() : CTask() {}

    // ITask
    virtual const char *name() const override { return "TaskMiniProject_Integrator"; }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
};
