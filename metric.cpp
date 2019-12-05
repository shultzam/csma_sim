/*
 * Implementation of the Metric class. A class used to store a node's metrics.
 */

#include "metric.h"

// Message class constructor with args. Most of this information is symbolic.
Metric::Metric() {
   // Initialize all values to 0.
   theCountOfClockCyclesIdle = 0;
   theCountOfClockCyclesBackedOff = 0;
   theCountOfCollisions = 0;
   theCountOfTransmissionAttempts = 0;
   theCountOfMessagesGenerated = 0;
   theCountOfMessagesTransmitted = 0;
   theMaximumRetransmissionAttempts = 0;
}

// Incrementer for theCountOfClockCyclesIdle.
void Metric::incrementClockCyclesIdle() {
   theCountOfClockCyclesIdle++;
}

// Incrementer for theCountOfClockCyclesBackedOff.
void Metric::incrementClockCyclesBackedOff() {
   theCountOfClockCyclesBackedOff++;
}

// Incrementer for theCountOfCollisions.
void Metric::incrementCountOfCollisions() {
   theCountOfCollisions++;
}

// Incrementer for theCountOfTransmissionAttempts.
void Metric::incrementCountOfTransmissionAttempts() {
   theCountOfTransmissionAttempts++;
}

// Incrementer for theCountOfMessagesGenerated.
void Metric::incrementCountOfMessagesGenerated() {
   theCountOfMessagesGenerated++;
}

// Incrementer for theCountOfMessagesTransmitted.
void Metric::incrementCountOfMessagesTransmitted() {
   theCountOfMessagesTransmitted++;
}

// Setter for theMaximumRetransmissionAttempts.
void Metric::setMaximumRetransmissionAttempts(unsigned int count) {
   theMaximumRetransmissionAttempts = count;
}

// Getter for theCountOfClockCyclesIdle.
unsigned int Metric::getClockCyclesIdle() {
   return theCountOfClockCyclesIdle;
}

// Getter for theCountOfClockCyclesBackedOff.
unsigned int Metric::getClockCyclesBackedOff() {
   return theCountOfClockCyclesBackedOff;
}

// Getter for theCountOfCollisions.
unsigned int Metric::getCountOfCollisions() {
   return theCountOfCollisions;
}

// Getter for theCountOfTransmissionAttempts.
unsigned int Metric::getCountOfTransmissionAttempts() {
   return theCountOfTransmissionAttempts;
}

// Getter for theCountOfMessagesGenerated.
unsigned int Metric::getCountOfMessagesGenerated() {
   return theCountOfMessagesGenerated;
}

// Getter for theCountOfMessagesTransmitted.
unsigned int Metric::getCountOfMessagesTransmitted() {
   return theCountOfMessagesTransmitted;
}

// Getter for theMaximumRetransmissionAttempts.
unsigned int Metric::getMaximumRetransmissionAttempts() {
   return theMaximumRetransmissionAttempts;
}
