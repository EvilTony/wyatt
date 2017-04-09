//
// Created by Arthur Lockman on 4/5/17.
//

#include <commands/SimpleIteratorCommand.h>
#include "../include/CommandManager.h"
#include "../catch/catch.hpp"
#include "../include/ISensor.h"
#include "../include/IRangeFinderSensor.h"
#include "../include/RawSensorData.h"
#include "../include/Hardware.h"
#include "../include/ISensorManager.h"
#include "../include/SensorManager.h"
#include "../include/ISensorManagerExceptions.h"

/* MOCKS */
#include "MockIRRangeFinderSensor.h"

TEST_CASE("Command subsystem tests", "[CommandManager]") {

    SECTION("Command manager can be killed.") {
        CommandManager *commandManager = new CommandManager();
        REQUIRE(commandManager->kill() == 0);
    }

    SECTION("Simple commands run to completion.") {
        CommandManager *commandManager1 = new CommandManager();
        SimpleIteratorCommand *sc1 = new SimpleIteratorCommand(10);
        SimpleIteratorCommand *sc2 = new SimpleIteratorCommand(20);
        commandManager1->runCommand(sc1);
        commandManager1->runCommand(sc2);
        REQUIRE(commandManager1->inFlight() == 2);
        while (!sc1->isFinished() && !sc2->isFinished()) {}
        REQUIRE(sc1->getCount() == 10);
        REQUIRE(sc2->getCount() == 20);
        REQUIRE(commandManager1->inFlight() == 0);
        commandManager1->kill();
    }

    SECTION("Commands can be canceled.") {
        CommandManager *commandManager2 = new CommandManager();
        SimpleIteratorCommand *sc3 = new SimpleIteratorCommand(1000);
        commandManager2->runCommand(sc3);
        commandManager2->cancel(sc3);
        while (!sc3->isFinished()) {}
        REQUIRE(sc3->getCount() != 1000);
        SimpleIteratorCommand *sc4 = new SimpleIteratorCommand(1000);
        SimpleIteratorCommand *sc5 = new SimpleIteratorCommand(2000);
        SimpleIteratorCommand *sc6 = new SimpleIteratorCommand(3000);
        commandManager2->runCommand(sc4);
        commandManager2->runCommand(sc5);
        commandManager2->runCommand(sc6);
        commandManager2->cancelAll();
        while (!sc4->isFinished() && !sc5->isFinished() && !sc6->isFinished()) {}
        REQUIRE(sc4->getCount() != 1000);
        REQUIRE(sc5->getCount() != 2000);
        REQUIRE(sc6->getCount() != 3000);
        commandManager2->kill();
    }
}


TEST_CASE("Sensor interface tests", "[ISensor]") {

    RawSensorData* mockRawSensorData_IR = new RawSensorData("12.2");

    SECTION("ISensor polymorphically updates") {
        ISensor* sensor = new MockIRRangeFinderSensor();
        sensor->updateSensor(mockRawSensorData_IR);
        MockIRRangeFinderSensor* IRSensor = (MockIRRangeFinderSensor*) sensor;
         
        REQUIRE( IRSensor->getDistanceCM() == 12.2 );
    }

}

TEST_CASE("RawSensorData tests", "[RawSensorData]") {

    std::string testString = "testString";

    SECTION("Data is correctly encapsulated and returned") {
        RawSensorData* data = new RawSensorData(testString);

        REQUIRE(data->getData() == testString);
    }
}

TEST_CASE("ISensorManager tests", "[ISensorManager]") {

    ISensorManager* sensorManager = new SensorManager();

    SECTION("Adding hardware twice results in exception.") {
        Hardware mockHardware = H_LEFT_MOTOR;
        ISensor* mockSensor = new MockIRRangeFinderSensor();
        
        sensorManager->addSensor(mockHardware, mockSensor);
        REQUIRE_THROWS(
                sensorManager->addSensor(mockHardware, mockSensor)
        );
    }

    SECTION("Updating non-existent hardware results in exception") {
        Message* msg = new Message(H_LEFT_MOTOR, (char *) "");
        std::list<Message*>* messages = new std::list<Message*>;
        messages->push_back(msg);
        REQUIRE_THROWS(
                sensorManager->updateSensors(messages)
        );
    }

    SECTION("Updates sensors correctly") {
        Hardware mockHardware = H_LEFT_MOTOR;
        MockIRRangeFinderSensor* mockSensor = new MockIRRangeFinderSensor();
        sensorManager->addSensor(mockHardware, mockSensor);

        Message* msg = new Message(H_LEFT_MOTOR, (char *) "12.2");
        std::list<Message*>* messages = new std::list<Message*>;
        messages->push_back(msg);

        sensorManager->updateSensors(messages);

        REQUIRE(mockSensor->getDistanceCM() == 12.2);
    }


}

TEST_CASE("Message tests", "[Message]") {
    char* data = (char *) "testData";
    Hardware hardware = H_LEFT_MOTOR;
    Message* msg = new Message(hardware, data);
    REQUIRE(msg->getHardware() == hardware);
    REQUIRE(msg->getMessage() == data);
}

