/*
 * Declaration of the Node class. A class used to represent a slot or station.
 */

#ifndef __NODE_H__
#define __NODE_H__

#include <deque>

#include "helpers.h"

// Forward declarations. Resolves circular dependency issues.
class Message;
class Configuration;
class Metric;

// Enum representing a node's current transmit state.
// IN_QUEUE means that the node is currently backed off and is waiting for a reattempt.
typedef enum NODE_STATE {
   IDLE = 0,
   PENDING,
   TRANSMITTING,
   BACKED_OFF
} NODE_STATE;

class Node {
   public:
      // Constructor with args.
      Node(int address);
      
      // Destructor declared in order to free up the stored message object if needed.
      ~Node();
      
      // Helper function to determine if the medium is idle.
      bool isMediumIdle(std::vector<Node*> nodeVector);
   
      // Transmits a message to a node other than itself.
      bool startMessageTransmit(unsigned int currentTime);
   
      // Completes the node's current transmit of a message.
      bool completeMessageTransmit();
   
      // Backs off from attempting to transmit based on the current configuration.
      bool backoffFromTransmit(unsigned int timeOfNextTransmitAttempt);
   
      // Reset the node's counter of consecutively experience.
      void resetRetransmitAttempts();
   
      // Increments the node's counter of consecutively experience.
      void incrementRetransmitAttempts();
   
      // Determines the end of the back-off.
      int determineBackoffEndTime(unsigned int currentTime, Configuration* configObj);
      
      // Determine the end of the binary exponential backoff.
      int determineEndOfBinaryExpBackoff(unsigned int currentTime, Configuration* configObj);

      // Returns if node has a message.
      bool hasMessage();

      // Adds a message to theMessageVector.
      bool addMessage(Message* messageObj);

      // Clears front most message object.
      void clearCurrentMessage();

      // Pops a message off the back of theMessageVector due to a simulated buffer overflow.
      void messagesOverflowed();
   
      /*
       * SETTERS
       */
      // Setter for nodeInternalAddress.
      bool setInternalAddress(int address);
   
      // Setter for theNodeState.
      bool setNodeState(NODE_STATE state);
   
      // Setter for theTimeOfTransmitCompletion.
      bool setTimeOfTransmitCompletion(int time);
   
      // Setter for nextAttemptedTransmitTime.
      bool setNextAttemptedTransmitTime(int time);
   
      // Setter for theRetransmitAttempts.
      bool setRetransmitAttempts(int attempts);
   
      // Setter for theMessage.
      bool setMessage(Message* messageObj);
      
      /*
       * GETTERS
       */
      // Returns the count of messages in theMessageVector.
      int getMessageCount();
      
      // Getter for theNodeInternalAddress.
      int getInternalAddress();
   
      // Getter for theNodeState.
      NODE_STATE getNodeState();
   
      // Getter for theTimeOfTransmitCompletion.
      int getTimeOfTransmitCompletion();
   
      // Getter for theNextAttemptedTransmitTime.
      int getNextAttemptedTransmitTime();
   
      // Getter for theRetransmitAttempts.
      int getRetransmitAttempts();
      
      // Getter for theMetric.
      Metric* getNodeMetric();
      
      // Metric object.
      // Contains non-null Metric object.
      Metric* theNodeMetric;
   
   private:
      // Clears all message objects in theMessageVector. 
      void clearAllMessages();
   
      // Internal address for this node.
      int theNodeInternalAddress;
   
      // Enum representing current state.
      NODE_STATE theNodeState;
   
      // Integer representing the time of completion for the current transmit. 
      // Values:
      //    -1: default, no transmit in progress
      //    non-negative integer: time of completion for the current transmitted message
      // This value will be ignored if theNodeState is not set to BACKED_OFF
      int theTimeOfTransmitCompletion;
   
      // Integer representing the next attempted transmit time. Behavior will depend upon the configured protocol.
      // Values:
      //    -1: default, no scheduled attempted transmit time
      //    non-negative integer: time of next scheduled transmission attempt
      // This value will be ignored if theNodeState is not set to BACKED_OFF
      int theNextAttemptedTransmitTime;
      
      // Integer representing the number of retransmission attempts this node has currently consecutively experienced.
      int theRetransmitAttempts;
      
      // Message deque.
      // Contains non-null Message objects if currently transmitting or in a back-off state. Maximum count of 10.
      std::deque<Message*> theMessageDeque;
};

#endif	// __NODE_H__
