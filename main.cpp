/*
 * This file is the main driver file for csma_sim, a simulation of a very, very basic CSMA protocol. Please 
 * execute java Configure to configure the system via the GUI. You can also edit csma_config.ini manually 
 * to create the simulation of interest.
 */

#include <ctime>

#include "helpers.h"

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
   
   // TODO: make a 3-D array storing metrics for each node.
   
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
      
      // Loop through all of the time-slots.
      unsigned long timeSlots = configObj->getTimeSlotCount();
      for (unsigned int timeIndex = 0; timeIndex < timeSlots; timeIndex++) {
         CLog::write(CLog::VERBOSE, "---- timeIndex: %u ----\n", timeIndex);
         determineNodeStates(nodeVector, timeIndex, configObj);
         CLog::write(CLog::VERBOSE, "\n", timeIndex);
      }
      
      // Loop through all of the nodes and report the metrics.
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
                    "      maximum retransmission attempts: %d\n", 
                    (*it)->theNodeMetric->getMaximumRetransmissionAttempts());
         CLog::write(CLog::METRICS, "\n");
      }
      
      // Cleanup node objects.
      for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++) {
         delete *it;
      }
   }
   
   // Cleanup config object.
   delete configObj;
   
   return 0;
}
