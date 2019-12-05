/*
 * Declaration of the Metric class. A class used to store a node's metrics.
 */

#ifndef __METRIC_H__
#define __METRIC_H__

#include "helpers.h"

class Metric {
	public:
      // Overwrite the default constructor.
      Metric();
      
      // Destructor not declared since the default will suffice.
      
      /*
       * INCREMENTERS/SETTERS
       */
      // Incrementer for theCountOfClockCyclesIdle.
      void incrementClockCyclesIdle();
      
      // Incrementer for theCountOfClockCyclesBackedOff.
      void incrementClockCyclesBackedOff();
      
      // Incrementer for theCountOfCollisions.
      void incrementCountOfCollisions();
      
      // Incrementer for theCountOfTransmissionAttempts.
      void incrementCountOfTransmissionAttempts();
      
      // Incrementer for theCountOfMessagesGenerated.
      void incrementCountOfMessagesGenerated();
      
      // Incrementer for theCountOfMessagesTransmitted.
      void incrementCountOfMessagesTransmitted();
      
      // Setter for theMaximumRetransmissionAttempts.
      void setMaximumRetransmissionAttempts(unsigned int count);
      
      /*
       * GETTERS
       */
      // Getter for theCountOfClockCyclesIdle.
      unsigned int getClockCyclesIdle();
      
      // Getter for theCountOfClockCyclesBackedOff.
      unsigned int getClockCyclesBackedOff();
      
      // Getter for theCountOfCollisions.
      unsigned int getCountOfCollisions();
      
      // Getter for theCountOfTransmissionAttempts.
      unsigned int getCountOfTransmissionAttempts();
      
      // Getter for theCountOfMessagesGenerated.
      unsigned int getCountOfMessagesGenerated();
      
      // Getter for theCountOfMessagesTransmitted.
      unsigned int getCountOfMessagesTransmitted();
      
      // Getter for theMaximumRetransmissionAttempts.
      unsigned int getMaximumRetransmissionAttempts();
      
	private:
      // Used to track the number of cycles in an idle stae.
      unsigned int theCountOfClockCyclesIdle;
      
      // Used to track the number of cycles in a backed-off state.
      unsigned int theCountOfClockCyclesBackedOff;
      
      // Used to track how many collisions a channel experienced. 
      unsigned int theCountOfCollisions;
      
      // Used to track how many transmission attempts occurred. 
      unsigned int theCountOfTransmissionAttempts;
      
      // Used to track how many packets were generated.
      unsigned int theCountOfMessagesGenerated;
      
      // Used to track how many packets were transmitted.
      unsigned int theCountOfMessagesTransmitted;
      
      // Used to track the maximum amount of retransmissions occurred before sending any one particular packet.
      unsigned int theMaximumRetransmissionAttempts;
};

#endif	// __METRIC_H__
