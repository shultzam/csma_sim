/*
 * Declaration of the Message class. A class used to represent a message.
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "helpers.h"

class Message {
	public:
      // Constructor with args.
      Message(int sendingAddress, int receiveAddress, int messageSize);
   
      // Destructor not declared since the default will suffice.
   
      /*
       * SETTERS
       */
      // Setter for theSendAddr.
      bool setSendAddr(int address);
      
      // Setter for theReceiveAddr.
      bool setReceiveAddr(int address);
      
      // Setter for theMessageSize.
      bool setMessageSize(int size);
   
   	/*
       * GETTERS
       */
      // Getter for theSendAddr.
      int getSendAddr();
   
      // Getter for theReceiveAddr.
      int getReceiveAddr();
      
      // Setter for theMessageSize.
      int getMessageSize();
   
   private:
      // The address of the sender node.
      int theSendAddr;
   
      // The address of the receiving node.
      int theReceiveAddr;
	
	   // The size of the message (used to determine when the message will be finished sending).
      int theMessageSize;
};

#endif	// __MESSAGE_H__
