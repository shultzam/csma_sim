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
void printOverallMetrics(Metric* nodeTotalMetrics[], unsigned int arraySize);

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
   Metric* nodeTotalMetrics[configObj->getNodeCount()];
   for (unsigned short nodeIndex = 0; nodeIndex < configObj->getNodeCount(); nodeIndex++) {
      Metric* metricObj = new Metric();
      nodeTotalMetrics[nodeIndex] = metricObj;
   }
   
   // TODO: METRICS print config file
   
   // For a clean simulation, all of the node objects will be recreated each time.
   for (unsigned short simIndex = 0; simIndex < configObj->getSimulationCount(); simIndex++) {
      // Initialize the nodes and place into a vector.
      std::vector<Node*> nodeVector;
      for (unsigned short nodeIndex = 0; nodeIndex < configObj->getNodeCount(); nodeIndex++) {
         Node* nodeObj = new Node(nodeIndex);
         nodeVector.push_back(nodeObj);
      }
      
      // Initialize random seed.
      srand (time(NULL));
      
      // TODO: Add simulation count.
      CLog::write(CLog::METRICS, "- simulation %u -\n", simIndex);
      
      // Loop through all of the time-slots.
      unsigned long timeSlots = configObj->getTimeSlotCount();
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
   printOverallMetrics(nodeTotalMetrics, configObj->getNodeCount());
   
   // Cleanup overall Node metrics objects.
   for (unsigned short nodeIndex = 0; nodeIndex < configObj->getNodeCount(); nodeIndex++) {
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
      
      // Clock cycles backed off
      lastValue = nodeTotalMetrics[nodeIndex]->getClockCyclesBackedOff();
      nodeTotalMetrics[nodeIndex]->setClockCyclesBackedOff(lastValue 
                                                           + nodeMetricCopy->getClockCyclesBackedOff());
      
      // Count of collisions.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfCollisions();
      nodeTotalMetrics[nodeIndex]->setCountOfCollisions(lastValue 
                                                        + nodeMetricCopy->getCountOfCollisions());
      
      // Count of transmission attempts.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfTransmissionAttempts();
      nodeTotalMetrics[nodeIndex]->setCountOfTransmissionAttempts(lastValue 
                                                                  + nodeMetricCopy->getCountOfTransmissionAttempts());
      
      // Count of messages generated.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesGenerated();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesGenerated(lastValue 
                                                               + nodeMetricCopy->getCountOfMessagesGenerated());
      
      // Count of messages Transmitted.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesTransmitted();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesTransmitted(lastValue 
                                                                 + nodeMetricCopy->getCountOfMessagesTransmitted());
      
      // Count of messages dropped.
      lastValue = nodeTotalMetrics[nodeIndex]->getCountOfMessagesDropped();
      nodeTotalMetrics[nodeIndex]->setCountOfMessagesDropped(lastValue 
                                                             + nodeMetricCopy->getCountOfMessagesDropped());
      
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
                 "      clock cycles idle: %d\n", 
                 (*it)->theNodeMetric->getClockCyclesIdle());
      CLog::write(CLog::METRICS,
                 "      clock cycles backed-off: %d\n", 
                 (*it)->theNodeMetric->getClockCyclesBackedOff());
      CLog::write(CLog::METRICS,
                 "      collisions occurred: %d\n", 
                 (*it)->theNodeMetric->getCountOfCollisions());
      CLog::write(CLog::METRICS,
                 "      tranmissions attempted: %d\n",
                 (*it)->theNodeMetric->getCountOfTransmissionAttempts());
      CLog::write(CLog::METRICS,
                 "      messages generated: %d\n",
                 (*it)->theNodeMetric->getCountOfMessagesGenerated());
      CLog::write(CLog::METRICS,
                 "      messages transmitted: %d\n",
                 (*it)->theNodeMetric->getCountOfMessagesTransmitted());
      CLog::write(CLog::METRICS,
                 "      messages dropped: %d\n", 
                 (*it)->theNodeMetric->getCountOfMessagesDropped());
      CLog::write(CLog::METRICS,
                 "      maximum retransmission attempts: %d\n", 
                 (*it)->theNodeMetric->getMaximumRetransmissionAttempts());
      CLog::write(CLog::METRICS, "\n");
   }
}

// Helper functions used to print the overall metrics for the entire execution.
void printOverallMetrics(Metric* nodeTotalMetrics[], unsigned int arraySize) {
   for (unsigned short nodeIndex = 0; nodeIndex < arraySize; nodeIndex++) {
      
   }
}
