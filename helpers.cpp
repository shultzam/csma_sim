/*
 * Implementation of helper functions that belong to no class.
 */

#include <algorithm>    // random_shuffle

#include "helpers.h"

// Helper function that loops through each node and determines the state of each node based on the current configuration.
// A decision was made to not verify that the objects are not null to speed up the application. This is risky and not 
// suitable for any sort of commercial product.
void determineNodeStates(std::vector<Node*> nodeVector, unsigned int currentTime, Configuration* configObj) {
   /* 
    * Shuffle the order of the node vector so that the nodes are being serviced in a "random" order. 
    * This will help prevent service biases.
    */
   std::random_shuffle(nodeVector.begin(), nodeVector.end());
   
   // Loop through each node to check if any transmits concluded.
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      // Check if current node is transmitting and has completed its transmission.
      if (TRANSMITTING == (*it)->getNodeState() && currentTime == (*it)->getTimeOfTransmitCompletion()) {
         if (!(*it)->completeMessageTransmit(currentTime)) {
               std::cout << "WARNING - failed to complete message transmit for node " 
                         << (*it)->getInternalAddress() 
                         << std::endl;
         }
      }
   }
   
   // Loop through each node to check if any nodes will generate a message.
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      // Determine if node should try to transmit - based on the porbability of frame generation.
      if (generateRandomFloatZeroToOne() > (1 - configObj->getProbFrameGeneration())) {
         // Initialize the message.
         // TODO: give destination address a real value if it gets implemented
         CLog::write(CLog::VERBOSE, "node %d generating a message\n", (*it)->getInternalAddress());
         Message* message = new Message((*it)->getInternalAddress(),    // sender's address
                                     0,                                 // destination's address
                                     configObj->getFrameLength(),       // size
                                     currentTime);                      // time of message creation
         
         // Load the message.
         if (!(*it)->addMessage(message)) {
            std::cout << "ERROR - failed to add message" << std::endl;  
         }
      }
   }
               
   // Determine the type of backoff.
   CSMA_TYPE csmaType = configObj->getCsmaType();
   
   // Save off a list of nodes that intend to tranmist this time slot.
   std::vector<Node*> transmittingNodes;
   
   // Loop through each node to check if any new transmissions will begin.
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      // Check if current node is transmitting.
      if (TRANSMITTING == (*it)->getNodeState()) {
         // No need to consider transmitting again if already transmitting.
         CLog::write(CLog::VERBOSE, "node %d is transmitting\n", (*it)->getInternalAddress());
                  
         // Update the metric.
         (*it)->theNodeMetric->incrementClockCyclesTransmitting();
         
         continue;
      }
      // Check if current node is transmitting.
      else if (BACKED_OFF == (*it)->getNodeState()) {
         CLog::write(CLog::VERBOSE, "node %d is backed-off\n", (*it)->getInternalAddress());
         if (currentTime == (*it)->getNextAttemptedTransmitTime()) {
            CLog::write(CLog::VERBOSE, "now is node %d's next attempted transmit time\n", (*it)->getInternalAddress());
            // Check if the medium is idle for a transmission.
            if ((*it)->isMediumIdle(nodeVector)) {
               CLog::write(CLog::VERBOSE, "medium is idle for retransmit attempt\n");
               // The node transmits here if the medium is idle and (with probability p if a p-persistence CSMA is used).
               if (P_PERSISTENT == csmaType) {
                  if (generateRandomFloatZeroToOne() > (1 - configObj->getProbOfPersistance())) {
                     // p-Persistence node transmitting with probability p.
                     transmittingNodes.push_back(*it);
                  }
                  else {
                     // Backed-off with probability (1 - p) until next cycle and check again.
                     if (!(*it)->backoffFromTransmit(currentTime + 1)) {
                        std::cout << "ERROR - failed to back-off from transmit of message" << std::endl;
                     }
         
                     // Update the metric.
                     (*it)->theNodeMetric->incrementClockCyclesIdle();
                  }
               }
               // Medium is idle and a CSMA protocol other than p-Persistent was chosen so transmit immediately.
               else {
                  transmittingNodes.push_back(*it);
               }
            }
            else {
               // Medium is not idle. Continue the back-off.
               CLog::write(CLog::VERBOSE, "medium is NOT idle for retransmit attempt\n");
               int nextAttemptedTransmitTime = (*it)->determineBackoffEndTime(currentTime, configObj);
               if (!(*it)->backoffFromTransmit(nextAttemptedTransmitTime)) {
                  std::cout << "ERROR - failed to continue back-off from transmit of message" << std::endl;
               }
               
               // Update the metric.
               (*it)->theNodeMetric->incrementClockCyclesIdle();
               (*it)->theNodeMetric->incrementCountOfTransmissionAttempts();
            }
         }
         else {         
            // Update the metric.
            (*it)->theNodeMetric->incrementClockCyclesIdle();
         }
         
         // Check next node.
         continue;
      }
         
      // Check if the idle node has a message to determine if it should start a transmission.
      if ((*it)->hasMessage()) {
         if ((*it)->isMediumIdle(nodeVector)) {
            // If this is a p-persistent system another check is required.
            if (P_PERSISTENT == csmaType) {
               if (generateRandomFloatZeroToOne() > (1 - configObj->getProbOfPersistance())) {
                  CLog::write(CLog::VERBOSE, 
                              "medium is idle for retransmit attempt and p-persistent node %d will attempt to transmit\n", 
                                    (*it)->getInternalAddress());
                  transmittingNodes.push_back(*it);
               }
               else {
                  // Backoff until next timeslot and do it again.
                  CLog::write(CLog::VERBOSE, 
                              "p-persistance node %d will wait until next time cycle and try again\n", 
                              (*it)->getInternalAddress());
                  if (!(*it)->backoffFromTransmit(currentTime + 1)) {
                     std::cout << "ERROR - failed to continue back-off from transmit of message" << std::endl;
                  }
                  
                  // Update the metric.
                  (*it)->theNodeMetric->incrementClockCyclesIdle();
               }
            }
            // Every other CSMA type will transmit immediately given an idle system.
            else {
               // Transmit the message.
               CLog::write(CLog::VERBOSE, "medium is idle for transmit so node will transmit\n");
               transmittingNodes.push_back(*it);
            }
         }
         else {
            // Medium is NOT idle. Determine the duration of the back-off.
            CLog::write(CLog::VERBOSE, "medium is NOT idle for retransmit attempt\n");
            int nextAttemptedTransmitTime = (*it)->determineBackoffEndTime(currentTime, configObj);

            // Execute the back-off.
            if (!(*it)->backoffFromTransmit(nextAttemptedTransmitTime)) {
               std::cout << "ERROR - failed to back-off from transmit of message" << std::endl;
            }
            
            // Update the metric.
            (*it)->theNodeMetric->incrementCountOfTransmissionAttempts();
            (*it)->theNodeMetric->incrementClockCyclesIdle();
         }
      }
      else {
         // Node is idle so metrics are only needed to update the metric.
         (*it)->theNodeMetric->incrementClockCyclesIdle();
      }
   }
   
   // Determine if a node can transmit or if a collision occurred.
   if (0 == transmittingNodes.size()) {
      // Nothing to do.
   }
   else if (1 == transmittingNodes.size()) {
      if (!transmittingNodes[0]->startMessageTransmit(currentTime)) {
         std::cout << "ERROR - failed to start transmit of message" << std::endl;  
      }
      else {
         // Update the metric.
         transmittingNodes[0]->theNodeMetric->incrementClockCyclesTransmitting();
         transmittingNodes[0]->theNodeMetric->incrementCountOfTransmissionAttempts();
      }
   }
   else {
      // Collision occurred for each node that tried to transmit.
      for (std::vector<Node*>::iterator it = transmittingNodes.begin(); it != transmittingNodes.end(); it++) {
         int nextAttemptedTransmitTime = (*it)->determineEndOfBinaryExpBackoff(currentTime, configObj);
         
         // Execute the back-off.
         if (!(*it)->backoffFromTransmit(nextAttemptedTransmitTime)) {
            std::cout << "ERROR - failed to back-off from transmit of message" << std::endl;
         }
         
         CLog::write(CLog::VERBOSE, 
                     "collision occurred for node %d, next transmit at time %d\n", 
                     (*it)->getInternalAddress(), 
                     (*it)->getNextAttemptedTransmitTime());
         
         // Update the metrics.
         (*it)->theNodeMetric->incrementCountOfCollisions();
         (*it)->theNodeMetric->incrementClockCyclesIdle();
         (*it)->theNodeMetric->incrementCountOfTransmissionAttempts();
      }
   }
}

// Helper function used to generate a random float [0.0, 1.0].
float generateRandomFloatZeroToOne() {
   return static_cast<float>(rand())/static_cast<float>(RAND_MAX);  
}

// Helper function used to generate a random integer between minimum and maximum.
// Note that this function is not perfect and can be biased towards a non-uniform distrution.
int generateRandomIntegerMinToMax(unsigned int min, unsigned int max) {
   return (rand() % static_cast<unsigned int>(max)) + min;
}
