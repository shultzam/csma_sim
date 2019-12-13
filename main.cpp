/*
 * This file is the main driver file for csma_sim, a simulation of a very, very basic CSMA protocol. Please 
 * execute java Configure to configure the system via the GUI. You can also edit csma_config.ini manually 
 * to create the simulation of interest.
 */

#include <ctime>

#include "helpers.h"

// Forward delcare of helper functions.
void copyMetrics(Metric* nodeTotalMetrics[], std::vector<Node*> nodeVector);
void printSimulationMetrics(std::vector<Node*> nodeVector, unsigned int simIndex);
void printOverallMetrics(Metric* nodeTotalMetrics[], 
                         Configuration* configObj);

// Global that identifies the configuration INI file.
std::string GLOBAL_CONFIG_INI("./csma_config.ini");

int main(int argc, char* argv[]) {   
   // Retrieve configuration variables
   Configuration* configObj = new Configuration(GLOBAL_CONFIG_INI);
   
   // Update the log level.
   if (configObj->getVerboseEnabled()) {
      std::cout << "Enabling verbose logging." << std::endl;
      CLog::setLevel(CLog::ALL);
   }
   else {
      std::cout << "Disabling verbose logging." << std::endl;
      CLog::setLevel(CLog::METRICS);
   }
   
   // Use an array metrics objects to track each node.
   unsigned short nodeCount = configObj->getNodeCount();
   Metric* nodeTotalMetrics[nodeCount];
   for (unsigned short nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {
      Metric* metricObj = new Metric();
      nodeTotalMetrics[nodeIndex] = metricObj;
   }
   
   // TODO: METRICS print config file
   
   // For a clean simulation, all of the node objects will be recreated each time.
   unsigned short simCount = configObj->getSimulationCount();
   unsigned long timeSlots = configObj->getTimeSlotCount();
   for (unsigned short simIndex = 0; simIndex < simCount; simIndex++) {
      // Initialize the nodes and place into a vector.
      std::vector<Node*> nodeVector;
      for (unsigned short nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {
         Node* nodeObj = new Node(nodeIndex);
         nodeVector.push_back(nodeObj);
      }
      
      // Initialize random seed.
      srand (time(NULL));
      
      // TODO: Add simulation count.
      CLog::write(CLog::METRICS, "- simulation %u -\n", simIndex);
      
      // Loop through all of the time-slots.
      for (unsigned int timeIndex = 0; timeIndex < timeSlots; timeIndex++) {
         CLog::write(CLog::VERBOSE, "---- timeIndex: %u ----\n", timeIndex);
         determineNodeStates(nodeVector, timeIndex, configObj);
         CLog::write(CLog::VERBOSE, "\n", timeIndex);
      }
      
      // Report the metrics.
      printSimulationMetrics(nodeVector, simIndex);
      
      // Copy over the metrics from this simulation.
      copyMetrics(nodeTotalMetrics, nodeVector);
      
      // Cleanup node objects.
      for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
         delete *it;
      }
   }
   
   // Display the overall data.
   printOverallMetrics(nodeTotalMetrics, configObj);
   
   // Cleanup overall Node metrics objects.
   for (unsigned short nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {
      delete(nodeTotalMetrics[nodeIndex]);
   }
   
   // Cleanup config object.
   delete configObj;
   
   return 0;
}


//////////////////////////////////////////////////
//             HELPER FUNCTIONS
//////////////////////////////////////////////////


// Helper function used to copy over the node data from one simulation.
void copyMetrics(Metric* nodeTotalMetrics[], std::vector<Node*> nodeVector) {
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      // Save off copies of relevant data for ease of access.
      int nodeIndex = (*it)->getInternalAddress();
      Metric* nodeMetricCopy = (*it)->theNodeMetric;
      
      // Clock cycles idle.
      unsigned int lastValue = nodeTotalMetrics[nodeIndex]->getClockCyclesIdle();
      nodeTotalMetrics[nodeIndex]->setClockCyclesIdle(lastValue + nodeMetricCopy->getClockCyclesIdle());
      
      // Clock cycles transmitting.
      lastValue = nodeTotalMetrics[nodeIndex]->getClockCyclesTransmitting();
      nodeTotalMetrics[nodeIndex]->setClockCyclesTransmitting(lastValue 
                                                              + nodeMetricCopy->getClockCyclesTransmitting());
      
      // Count of messages generated.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesGenerated();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesGenerated(lastValue 
                                                               + nodeMetricCopy->getCountOfMessagesGenerated());
      
      // Count of transmission attempts.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfTransmissionAttempts();
      nodeTotalMetrics[nodeIndex]->setCountOfTransmissionAttempts(lastValue 
                                                                  + nodeMetricCopy->getCountOfTransmissionAttempts());
      
      // Count of collisions.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfCollisions();
      nodeTotalMetrics[nodeIndex]->setCountOfCollisions(lastValue 
                                                        + nodeMetricCopy->getCountOfCollisions());
      
      // Count of messages dropped.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesDropped();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesDropped(lastValue 
                                                             + nodeMetricCopy->getCountOfMessagesDropped());
      
      // Count of messages transmitted.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesTransmitted();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesTransmitted(lastValue 
                                                                 + nodeMetricCopy->getCountOfMessagesTransmitted());
      
      // Time slots messages spent waiting to be transmitted (time of completion - time of creation).
      nodeTotalMetrics[nodeIndex]->updateTimeMessagesWaited(nodeMetricCopy->getTimeMessagesWaited());
      
      // Maximum count of retransmission attempts.
      lastValue = nodeTotalMetrics[nodeIndex]->getMaximumRetransmissionAttempts();
      nodeTotalMetrics[nodeIndex]->setMaximumRetransmissionAttempts(lastValue 
                                                                    + nodeMetricCopy->getMaximumRetransmissionAttempts());
   }
}

// Helper function used to print the data from one simulation.
void printSimulationMetrics(std::vector<Node*> nodeVector, unsigned int simIndex) {
   CLog::write(CLog::METRICS, "[sim %d node metrics]\n", simIndex);
   for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
      CLog::write(CLog::METRICS, 
                 "   [node %d]\n", 
                 (*it)->getInternalAddress());
      CLog::write(CLog::METRICS, 
                 "      time slots idle: %d\n", 
                 (*it)->theNodeMetric->getClockCyclesIdle());
      CLog::write(CLog::METRICS,
                 "      time slots transmitting: %d\n", 
                 (*it)->theNodeMetric->getClockCyclesTransmitting());
      CLog::write(CLog::METRICS,
                 "      messages generated: %d\n",
                 (*it)->theNodeMetric->getCountOfMessagesGenerated());
      CLog::write(CLog::METRICS,
                 "      tranmissions attempted: %d\n",
                 (*it)->theNodeMetric->getCountOfTransmissionAttempts());
      CLog::write(CLog::METRICS,
                 "      collisions occurred: %d\n", 
                 (*it)->theNodeMetric->getCountOfCollisions());
      CLog::write(CLog::METRICS,
                 "      messages dropped: %d\n", 
                 (*it)->theNodeMetric->getCountOfMessagesDropped());
      CLog::write(CLog::METRICS,
                 "      messages transmitted: %d\n",
                 (*it)->theNodeMetric->getCountOfMessagesTransmitted());
      CLog::write(CLog::METRICS,
                 "      time slots messages spent waiting: %d\n",
                 (*it)->theNodeMetric->getTimeMessagesWaited());
      CLog::write(CLog::METRICS,
                 "      maximum retransmission attempts: %d\n", 
                 (*it)->theNodeMetric->getMaximumRetransmissionAttempts());
      CLog::write(CLog::METRICS, "\n");
   }
}

// Helper functions used to print the overall metrics for the entire execution.
void printOverallMetrics(Metric* nodeTotalMetrics[], Configuration* configObj) {
   // Determine looping conditions.
   unsigned short arraySize = configObj->getNodeCount();
   unsigned long timeSlots = configObj->getTimeSlotCount();
   unsigned short simCount = configObj->getSimulationCount();
   
   CLog::write(CLog::METRICS, "[averages over %u simulations of %lu timeslots]\n", simCount, timeSlots); 
   
   // Loop through the nodes.
   for (unsigned short nodeIndex = 0; nodeIndex < arraySize; nodeIndex++) {
      CLog::write(CLog::METRICS, "   [node %d]\n", nodeIndex);
                 
      // Time slots idle.
      float value = ((float )nodeTotalMetrics[nodeIndex]->getClockCyclesIdle()/(float )simCount);
      CLog::write(CLog::METRICS, "     time slots idle: %.2f (%.4f of clock cycles)\n", 
                                 value, 
                                 (value/(float )timeSlots));
      
      // Time slots transmitting.
      float avgMessagesTransmitted = ((float )nodeTotalMetrics[nodeIndex]->getClockCyclesTransmitting()/(float )simCount);
      CLog::write(CLog::METRICS, "     time slots transmitting: %.2f (%.4f of clock cycles)\n", 
                                 avgMessagesTransmitted, 
                                 (avgMessagesTransmitted/(float )timeSlots));
      
      // Count of messages generated.
      float avgMessagesGenerated = ((float )nodeTotalMetrics[nodeIndex]->getCountOfMessagesGenerated()/(float)simCount);
      CLog::write(CLog::METRICS, "     messages generated: %.2f (%.4f of clock cycles)\n", 
                                 avgMessagesGenerated, 
                                 ((float )avgMessagesGenerated/(float )timeSlots));
      
      // Count of transmission attempts.
      float avgTransmissionAttempts = ((float )nodeTotalMetrics[nodeIndex]->getCountOfTransmissionAttempts()/(float )simCount);
      CLog::write(CLog::METRICS, "     transmission attempts: %.2f\n", 
                                 avgTransmissionAttempts);
      
      // Count of collisions.
      value = ((float )nodeTotalMetrics[nodeIndex]->getCountOfCollisions()/(float )simCount);
      CLog::write(CLog::METRICS, "     collisions: %.2f (%.4f of transmission attempts)\n", 
                                 value, 
                                 (value/(float )avgTransmissionAttempts));
      CLog::write(CLog::METRICS, "                       (%.4f of clock cycles)\n",
                                 (value/(float )timeSlots));
      
      // Count of messages dropped.
      value = ((float )nodeTotalMetrics[nodeIndex]->getCountOfMessagesDropped()/(float )simCount);
      CLog::write(CLog::METRICS, "     messages dropped: %.2f (%.4f of messages generated)\n", 
                                 value, 
                                 ((float )value/(float )avgMessagesGenerated));
      
      // Count of messages transmitted.
      value = ((float )nodeTotalMetrics[nodeIndex]->getCountOfMessagesTransmitted()/(float )simCount);
      CLog::write(CLog::METRICS, "     messages transmitted: %.2f (%.4f of messages generated)\n", 
                                 value, 
                                 ((float )value/(float )avgMessagesGenerated));
                                 
      // Time slots messages spent waiting to be transmitted (time of completion - time of creation).
      value = ((float )nodeTotalMetrics[nodeIndex]->getTimeMessagesWaited()/(float )simCount);
      CLog::write(CLog::METRICS, "     time slots messages waited: %.2f (%.2f per message transmitted)\n", 
                                 value, 
                                 ((float )value/(float )avgMessagesTransmitted));
      
      // Maximum count of retransmission attempts.
      value = ((float )nodeTotalMetrics[nodeIndex]->getMaximumRetransmissionAttempts()/(float )simCount);
      CLog::write(CLog::METRICS, "     maximum retransmissions required before any one message was sent: %.0f\n", 
                                 value);
                                 
      CLog::write(CLog::METRICS, "\n");
   }
}
