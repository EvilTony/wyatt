#include "Main.h"
#include <iostream>
int main (void)
{

    /* Create sensor manager */
    ISensorManager* sensorManager = new SensorManager();

    /* Create communicator and attach serial port */
    Communicator* comm = new Communicator(sensorManager, 9600);
    comm->attachHardware("/dev/tty.wchusbserial1410", H_LEFT_MOTOR);

    comm->start();

    /* Create test command */
    char commands[] = {H_LEFT_MOTOR.address, 0};
    Message* msg1 = new Message(H_LEFT_MOTOR, commands);

    /* Queue and send command */
    comm->queueMessage(msg1);
//    comm->sendNextMsg(H_LEFT_MOTOR);

    comm->join();

    return 0 ;
}
