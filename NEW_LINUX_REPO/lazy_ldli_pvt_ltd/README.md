Assignment 1: Digital Input Sampling and CAN Communication

Overview:
This assignment involves writing an Interrupt Service Routine (ISR) in C for an Electronic Control Unit (ECU) in a vehicle control architecture. The ECU measures 8 digital inputs from various switches/sensors, performs defined actions based on these inputs, and communicates with other ECUs through the CAN protocol.


Global Variables:

g_ReadDIpinSts: Holds the real-time status of digital input pins.
g_AppDIpinSts: Holds the stable status of digital input pins.
prevPinState: Stores the previous state of the digital input pins.
consistencyCounter: Counts the number of consecutive ISR calls where each pin state has been consistent.
ISR Function:

Reads the current state of each pin and compares it with the previous state.
If the state is consistent, increments the counter. If the counter reaches 10, updates g_AppDIpinSts.
If the state changes, resets the counter and updates the previous state.
Sends the stable status to other ECUs using CAN communication.

200~Assignment 2: Real-Time Operating System (RTOS) with FreeRTOS
Overview:
This assignment involves using FreeRTOS to manage multiple tasks in an embedded system. The tasks are event-driven and communicate using a queue.
