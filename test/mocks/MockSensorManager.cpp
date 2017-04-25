//
// Created by Tucker Haydon on 4/9/17.
//

#include "include/MockSensorManager.h"

MockSensorManager::MockSensorManager() : ISensorManager() {
    this->updateMessages = new std::list<Message*>;
};

MockSensorManager::~MockSensorManager() {

};

void MockSensorManager::addSensor(Hardware hardware, ISensor* sensor) {

};

void MockSensorManager::updateSensors(std::list<Message*>* messages) {

    while(!messages->empty()) {
        Message* msg = messages->front();
        this->updateMessages->push_back(msg);
        messages->pop_front();
    }
};