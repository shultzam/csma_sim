/*
 * Implementation of the Node class. A class used to represent a slot or station.
 */

#include <math.h>       // pow
#include <algorithm>    // std::min

#include "node.h"

// Node class constructor with args.
Node::Node(int address) {
   if (!setInternalAddress(address) 
    || !setNodeState(IDLE) 
    || !setNextAttemptedTransmitTime(-1)
    || !setTimeOfTransmitCompletion(-1)) {
      std::cout << "ERROR - node failed to initialize" << std::endl;
      exit(-1);
   }
   
   // Initialize the retransmit counter.
   resetRetransmitAttempts();
   
   // Initialize the node's Message object to NULL.
   theMessage = NULL;
   
   // Initialize the node's Metric object.
   theNodeMetric = new Metric();
}

// Destructor declared in order to free up the stored message object if needed.
Node::~Node() {
   clearMessage();
   
   // Delete the Metric object.
   Metric* storedMetric = getNodeMetric();
   if (storedMetric != NULL) {
      theNodeMetric = NULL;
      delete storedMetric;
   }
}

// Helper function to determine if the medium is idle.
bool Node::isMediumIdle(std::vector<Node*> nodeVector) {
   // Loop through each node.
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      if (TRANSMITTING == (*it)->getNodeState()) {
         return false;  
      }
   }
   
   // Return true, representing no nodes are transmitting.
   return true;
}

// Starts the transmit of a message to a node (other than itself).
bool Node::startMessageTransmit(unsigned int currentTime) {
   // Ensure the node has a message.
   if (!hasMessage()) {
      std::cout << "ERROR - cannot start message transmit on node " 
                << getInternalAddress()
                << " as it has no message" 
                << std::endl;
      return false;
   }
   
   // Update time of completed transmit.
   int completionTime = currentTime + theMessage->getMessageSize();
   if (!setTimeOfTransmitCompletion(completionTime)) {
      std::cout << "ERROR - failed to update node's time of transmit completion" << std::endl;
      return false;
   }
   
   // Update node state.
   if (!setNodeState(TRANSMITTING)) {
      std::cout << "ERROR - failed to set node state to TRANSMITTING" << std::endl;
      return false;
   }
   
   // Reset the next attempted transmit time.
   if (!setNextAttemptedTransmitTime(-1)) {
      std::cout << "WARNING - failed to reset the next attempted transmit time" << std::endl;  
   }
   
   // Reset the retransmit counter.
   resetRetransmitAttempts();
   
   
   CLog::write(CLog::VERBOSE, 
               "node %d starting transmit with completion time set to: %d\n", 
               getInternalAddress(),
               getTimeOfTransmitCompletion());
   
   // Update the message object and return.
   return true;
}

// Completes the node's current transmit of a message.
bool Node::completeMessageTransmit() {
   // Verify that this node is actually in a transmitting state.
   if (getNodeState() != TRANSMITTING) {
      std::cout << "WARNING - node " 
                << getInternalAddress() 
                << " was not transmitting yet completeMessageTransmit() was called." 
                << std::endl;
      return false;
   }
   
   // Remove the node's message that it was sending. This is more symbolic than anything.
   clearMessage();
   
   // Reset the node's state and return.
   if (!setNodeState(IDLE) || !setTimeOfTransmitCompletion(-1)) {
      std::cout << "WARNING - node " 
                << getInternalAddress() 
                << " failed to reset its state and time of transmit completion." 
                << std::endl;
   }
   
   // Update the metric.
   theNodeMetric->incrementCountOfMessagesTransmitted();
   
   CLog::write(CLog::VERBOSE, "node %d completed message transmission\n", getInternalAddress());
   return true;
}

// Backs off from attempting to transmit.
bool Node::backoffFromTransmit(unsigned int timeOfNextTransmitAttempt) {
   // Ensure the node has a message.
   if (!hasMessage()) {
      std::cout << "ERROR - cannot back-off on node " 
                << getInternalAddress()
                << " as it has no message" 
                << std::endl;
       return false;
   }
   
   // Update the node's state.
   if (!setNodeState(BACKED_OFF)) {
      std::cout << "ERROR - failed to set node state to BACKED_OFF" << std::endl;
      return false;
   }
   
   // Updated time of next attempted transmit.
   if (!(setNextAttemptedTransmitTime(timeOfNextTransmitAttempt))) {
      std::cout << "ERROR - failed to update next transmit time" << std::endl;
      return false;
   }
   
   // Increase the retransmit counter.
   incrementRetransmitAttempts();
   CLog::write(CLog::VERBOSE, 
               "node %d will back-off, reattempting transmission at time %d, new retransmit attempt counter: %d\n",
               getInternalAddress(),
               getNextAttemptedTransmitTime(),
               getRetransmitAttempts());
   return true;
}

// Clears theMessage.
void Node::clearMessage() {
   Message* storedMessage = getMessage();
   if (storedMessage != NULL) {
      theMessage = NULL;
      delete storedMessage;
   }
}

// Returns if node has a message.
bool Node::hasMessage() {
   Message* storedMessage = getMessage();
   if (storedMessage != NULL) {
      return true;  
   }
   
   return false;
}

// Reset the node's counter of consecutively experience.
void Node::resetRetransmitAttempts() {
   theRetransmitAttempts = 0;
}

// Increments the node's counter of consecutively experience.
void Node::incrementRetransmitAttempts() {
   theRetransmitAttempts++;
   
   // Update the metric.
   if (theNodeMetric->getMaximumRetransmissionAttempts() < theRetransmitAttempts) {
      theNodeMetric->setMaximumRetransmissionAttempts(theRetransmitAttempts);
   }
}

// Determines the end of the back-off.
int Node::determineBackoffEndTime(unsigned int currentTime, Configuration* configObj) {
   // Medium is NOT idle. Determine the duration of the back-off.
   unsigned int nextAttemptedTransmitTime = -1;
   CSMA_TYPE csmaType = configObj->getCsmaType();
   if (P_PERSISTENT == csmaType || ONE_PERSISTENT == csmaType) {
      nextAttemptedTransmitTime = currentTime + 1;
   }
   else if (NON_PERSISTENT == csmaType) {
      unsigned int power = std::min(configObj->getMaxBackoffRetransmitCount(), getRetransmitAttempts());
      nextAttemptedTransmitTime = currentTime + generateRandomIntegerMinToMax(1, pow(2, power));
   }
   else {
      // Intentionally blank for readability.
   }
   
   return nextAttemptedTransmitTime;
}

// Determine the end of the binary exponential backoff.
int Node::determineEndOfBinaryExpBackoff(unsigned int currentTime, Configuration* configObj) {
   unsigned int power = std::min(configObj->getMaxBackoffRetransmitCount(), getRetransmitAttempts());
   return currentTime + generateRandomIntegerMinToMax(1, pow(2, power));
}

// Setter for nodeInternalAddress.
bool Node::setInternalAddress(int address) {
   // Validate the address
   if (address < 0) {
      std::cout << "ERROR - illegal address: " << address << std::endl;
      return false;   
   }
   
   theNodeInternalAddress = address;
   return true;
}

// Setter for nodeState.
bool Node::setNodeState(NODE_STATE state) {
   if (state != IDLE && state != PENDING && state != TRANSMITTING && state != BACKED_OFF) {
      std::cout << "ERROR - unrecognized NODE_STATE: " << state << std::endl;
      return false;
   }
   
   theNodeState = state;
   return true;
}

// Setter for theTimeOfTransmitCompletion.
bool Node::setTimeOfTransmitCompletion(int time) {
   // Validate the time
   if (time < -1) {
      std::cout << "ERROR - illegal time: " << time << std::endl;
      return false;   
   }
   
   theTimeOfTransmitCompletion = time;
   return true;
}
   
// Setter for nextAttemptedTransmitTime.
bool Node::setNextAttemptedTransmitTime(int time) {
   // Validate the time.
   if (time < -1) {
      std::cout << "ERROR - illegal time: " << time << std::endl;
      return false;   
   }
   
   theNextAttemptedTransmitTime = time;
   return true;
}

// Setter for theRetransmitAttempts.
bool Node::setRetransmitAttempts(int attempts) {
   // Validate the time.
   if (attempts < 0) {
      std::cout << "ERROR - illegal attempts: " << attempts << std::endl;
      return false;   
   }
   
   theRetransmitAttempts = attempts;
   return true;
}

// Setter for theMessage.
bool Node::setMessage(Message* messageObj) {
   // Validate the object.
   if (!messageObj) {
      std::cout << "ERROR - message object is null" << std::endl;
      return false;   
   }
   
   // Update the metric.
   theNodeMetric->incrementCountOfMessagesGenerated();
   
   theMessage = messageObj;
   return true;
}

// Getter for nodeInternalAddress.
int Node::getInternalAddress() {
   return theNodeInternalAddress;  
}

// Getter for nodeState.
NODE_STATE Node::getNodeState() {
   return theNodeState;
}
   
// Getter for theTimeOfTransmitCompletion.
int Node::getTimeOfTransmitCompletion() {
   return theTimeOfTransmitCompletion;
}

// Getter for nextAttemptedTransmitTime.
int Node::getNextAttemptedTransmitTime() {
   return theNextAttemptedTransmitTime;
}
   
// Getter for theRetransmitAttempts.
int Node::getRetransmitAttempts() {
   return theRetransmitAttempts;
}

// Getter for theMessage.
Message* Node::getMessage() {
   return theMessage;  
}

// Getter for theMetric.
Metric* Node::getNodeMetric() {
   return theNodeMetric;  
}
