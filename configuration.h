/*
 * Declaration of the Configuration class. A class used to initialize the system's configuration.
 */

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "helpers.h"

// Enum representing the protocol type.
typedef enum CSMA_TYPE {
   NON_PERSISTENT = 0,
   ONE_PERSISTENT,
   P_PERSISTENT
} CSMA_TYPE;

class Configuration {
   public:
      // Constructor with args.
      Configuration(std::string configurationIni);
   
      // Destructor not declared since the default will suffice.
   
	   /*
       * SETTERS
       */
      // Setter for theVerboseEnabled.
      bool setVerboseEnabled(bool isEnabled);
      
      // Setter for theSimulationCount.
      bool setSimulationCount(unsigned int count);
      
      // Setter for theTimeSlotCount.
      bool setTimeSlotCount(unsigned long count);
      
      // Setter for theSimType.
      bool setCsmaType(CSMA_TYPE csmaType);
      
      // Setter for theProbabilityOfPersistance.
      bool setProbOfPersistance(float probability);
      
      // Setter for theNodeCount.
      bool setNodeCount(int count);
   
      // Setter for theProbFrameGeneration.
      bool setProbFrameGeneration(float probability);
   
      // Setter for theFrameLength.
      bool setFrameLength(int length);
   
      // Setter for theMaxBackoffRetransmitCount.
      bool setMaxBackoffRetransmitCount(int count);
   
      /*
       * GETTERS
       */
      // Getter for theVerboseEnabled.
      bool getVerboseEnabled();
       
      // Getter for theSimulationCount.
      unsigned int getSimulationCount();
      
      // Getter for theTimeSlotCount.
      unsigned long getTimeSlotCount();
       
      // Getter for theSimType.
      CSMA_TYPE getCsmaType();
      
      // Getter for theProbabilityOfPersistance.
      float getProbOfPersistance();
      
      // Getter for theNodeCount.
      int getNodeCount();
   
      // Getter for theProbFrameGeneration.
      float getProbFrameGeneration();
   
      // Getter for theFrameLength.
      int getFrameLength();
   
      // Getter for theMaxBackoffRetransmitCount.
      int getMaxBackoffRetransmitCount();
   
   private:
      // Stores the status of verbose logging, true or false.
      bool theVerboseEnabled;
      
      // Stores the count simulations to execute (helps) prevent noisy data if you want accurate metrics).
      unsigned int theSimulationCount;
   
      // Stores the count of time slots for the simulation.
      unsigned long theTimeSlotCount;
   
      // Stores the  protocol type.
      CSMA_TYPE theCsmaType;
	
      // Stores the probability of persistance.
      float theProbabilityOfPersistance;
   
      // Stores the count of nodes.
      int theNodeCount;
   
      // Stores the probability of frame/message generation.
      float theProbFrameGeneration;
   
      // Stores the frame length.
      int theFrameLength;
   
      // Stores the maximum backoff retransmit count.
      int theMaxBackoffRetransmitCount;
      
      // Helper function that checks if a line is blank, comment or category.
      bool checkLineForConfigurationString(std::string line);
   
      // Helper function that receives a configuration line in csma_config.ini and returns, 
      // via input arguments, the key and value.
      void getConfigKeyAndValue(char* line, std::string& key, std::string& value);
   
      // Helper function that sets the member variables based on a key/value from the INI file.
      bool updateConfig(std::string key, std::string value);
};

#endif	// __CONFIGURATION_H__
