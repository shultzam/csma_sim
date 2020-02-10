/*
 * Implementation of the Message class. A class used to represent a message.
 */

#include "message.h"

// Message class constructor with args. Most of this information is symbolic.
Message::Message(int sendingAddress, int receiveAddress, int messageSize, int timeOfCreation) {
   // Update the message's contents.
   bool success = setSendAddr(sendingAddress) 
                & setReceiveAddr(receiveAddress) 
                & setMessageSize(messageSize)
                & setMessageTimeOfCreation(timeOfCreation);
   if (!success) {
      std::cout << "ERROR - message failed to initialize" << std::endl;
      exit(-1);  
   }
}

// Setter for theSendAddr.
bool Message::setSendAddr(int address) {
   // Validate the address.
   if (address < 0) {
      std::cout << "ERROR - illegal address: " << address << std::endl;
      return false;   
   }
   
   theSendAddr = address;
   return true;
}

// Setter for theReceiveAddr.
bool Message::setReceiveAddr(int address) {
   // Validate the address.
   if (address < 0) {
      std::cout << "ERROR - illegal address: " << address << std::endl;
      return false;   
   }
   
   theReceiveAddr = address;
   return true;
}
      
// Setter for theMessageSize.
bool Message::setMessageSize(int size) {
   // Validate the size.
   if (size < 0) {
      std::cout << "ERROR - illegal size: " << size << std::endl;
      return false;   
   }
   
   theMessageSize = size;
   return true;
}

// Setter for theMessageTimeOfCreation.
bool Message::setMessageTimeOfCreation(int time) {
   // Validate the size.
   if (time < 0) {
      std::cout << "ERROR - illegal time: " << time << std::endl;
      return false;   
   }
   
   theMessageTimeOfCreation = time;
   return true;
}

// Getter for theSendAddr.
int Message::getSendAddr() {
   return theSendAddr;
}

// Getter for theReceiveAddr.
int Message::getReceiveAddr() {
   return theReceiveAddr;
}
      
// Setter for theMessageSize.
int Message::getMessageSize() {
   return theMessageSize;
}

// Setter for theMessageTimeOfCreation.
int Message::getMessageTimeOfCreation() {
   return theMessageTimeOfCreation;
}

