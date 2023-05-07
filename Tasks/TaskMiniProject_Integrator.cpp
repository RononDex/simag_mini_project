#include "TaskMiniProject_Integrator.h"
#include <iostream>
#include <sstream>

void TaskMiniProject_Integrator::setForces() {}
void TaskMiniProject_Integrator::doWork() {}
void TaskMiniProject_Integrator::imGui() {}
const char *TaskMiniProject_Integrator::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Integrator for the high precision SolarSystemPS"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskMiniProject_Integrator::draw() const {}
