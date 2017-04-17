#include "Communicator.h"
using namespace std;

Communicator::Communicator(ISensorManager* sensorManager, IHardwareInterface* hardwareInterface) : Thread() {
    this->sensorManager = sensorManager;
    this->hardwareInterface = hardwareInterface;

    this->messageQueue = new list<Message*>;
}

Communicator::~Communicator() {
    delete this->sensorManager;
    delete this->hardwareInterface;
    delete this->messageQueue;
}

void Communicator::queueMessage(Message *msg) {
    this->messageQueue->push_back(msg);
}

void Communicator::queueMessage(list<Message*>* messages) {
    for(Message* msg: *messages) {
        this->queueMessage(msg);
    }
}

void Communicator::write() {
    while (!this->messageQueue->empty())
    {
        this->hardwareInterface->write(this->messageQueue->front());
        this->messageQueue->pop_front();
    }
}

void Communicator::read() {
    this->sensorManager->updateSensors(this->hardwareInterface->read());
}

void* Communicator::run() {
    while(!m_signal) {

        // Write any data
        this->write();

        // Read any data
        this->read();

    }
    return NULL;
}

