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
   theCountOfMessagesDropped = 0;
}

// Setter for theCountOfClockCyclesIdle.
void Metric::setClockCyclesIdle(unsigned int value) {
   theCountOfClockCyclesIdle = value;
}

// Setter for theCountOfClockCyclesBackedOff.
void Metric::setClockCyclesBackedOff(unsigned int value) {
   theCountOfClockCyclesBackedOff = value;
}

// Setter for theCountOfCollisions.
void Metric::setCountOfCollisions(unsigned int value) {
   theCountOfCollisions = value;
}

// Setter for theCountOfTransmissionAttempts.
void Metric::setCountOfTransmissionAttempts(unsigned int value) {
   theCountOfTransmissionAttempts = value;
}

// Setter for theCountOfMessagesGenerated.
void Metric::setCountOfMessagesGenerated(unsigned int value) {
   theCountOfMessagesGenerated = value;
}

// Setter for theCountOfMessagesTransmitted.
void Metric::setCountOfMessagesTransmitted(unsigned int value) {
   theCountOfMessagesTransmitted = value;
}

// Setter for theCountOfMessagesDropped.
void Metric::setCountOfMessagesDropped(unsigned int value) {
   theCountOfMessagesDropped = value;
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

// Incrementer for theCountOfMessagesDropped.
void Metric::incrementCountOfMessagesDropped() {
   theCountOfMessagesDropped++;
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

// Getter for theCountOfMessagesDropped.
unsigned int Metric::getCountOfMessagesDropped() {
   return theCountOfMessagesDropped;
}

// Getter for theMaximumRetransmissionAttempts.
unsigned int Metric::getMaximumRetransmissionAttempts() {
   return theMaximumRetransmissionAttempts;
}
