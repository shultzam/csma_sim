/*
 * Implementation of the Configuration class. A class used to initialize the system's configuration.
 */

#include <fstream>
#include <string.h>  // strtok

#include "configuration.h"

// Configuration class constructor with args.
Configuration::Configuration(std::string configurationIni) {
   // Open the file.
   std::ifstream fileStream(configurationIni.c_str());
   
   // Extract the contents of the file, line by line.
   std::string line;
   while (fileStream >> line) {
      // If line is a category or comment, skip it.
      if (!checkLineForConfigurationString(line)) {
         continue;  
      }
      
      // getConfigKeyAndValue() hates const char* so we have to copy the std::string in a round-about way.
      char* tempStr = new char[line.length() + 1];
      strcpy(tempStr, line.c_str());
      
      // Parse configuration line on the '='.
      std::string key;
      std::string value;
      getConfigKeyAndValue(tempStr, key, value);
      
      // Delete the string.
      delete [] tempStr;
      
      // Assign the value based on the key.
      if(!updateConfig(key, value)) {
         std::cout << "ERROR - failed to initialize from config INI file" << std::endl;
         exit(-1);
      }
   }
   
   // Close the config file.
   fileStream.close();
}

// Setter for theVerboseEnabled.
bool Configuration::setVerboseEnabled(bool isEnabled) {
   theVerboseEnabled = isEnabled;
   return true;
}

// Setter for theSimulationCount.
bool Configuration::setSimulationCount(unsigned int count) {
   // Validate the input.
   if (count > 100) {
      std::cout << "ERROR - invalid theSimulationCount value: " << count << "; Valid if [1, 100]" << std::endl;
      return false;
   }
   
   theSimulationCount = count;
   return true;
}

// Setter for theTimeSlotCount.
bool Configuration::setTimeSlotCount(unsigned long count) {
   // Validate the input.
   if (count > 2500000) {
      std::cout << "ERROR - invalid theTimeSlotCount value: " << count << "; Valid if [1, 2500000]" << std::endl;
      return false;
   }
   
   theTimeSlotCount = count;
   return true;
}

// Setter for theSimType.
bool Configuration::setCsmaType(CSMA_TYPE csmaType) {
   // Validate the input.
   if (csmaType != NON_PERSISTENT && csmaType != ONE_PERSISTENT && csmaType != P_PERSISTENT) {
      std::cout << "ERROR - unrecognized PROTOCOL_TYPE: " << csmaType << std::endl;
      return false;
   }
   
   theCsmaType = csmaType;
   return true;
}
      
// Setter for theProbabilityOfPersistance.
bool Configuration::setProbOfPersistance(float probability) {
   // Validate the input.
   if (0 > probability || probability > 1.0) {
      std::cout << "ERROR - invalid theProbabilityOfPersistance value: " << probability << "; Valid if [0.0, 1.0]" << std::endl;
      return false;
   }
   
   theProbabilityOfPersistance = probability;
   return true;
}

// Setter for theNodeCount.
bool Configuration::setNodeCount(int count) {
   // Validate the input.
   if (0 > count || count > 100) {
      std::cout << "ERROR - invalid theNodeCount value: " << count << "; Valid if [0, 100]" << std::endl;
      return false;
   }
   
   theNodeCount = count;
   return true;
}

// Setter for theProbFrameGeneration.
bool Configuration::setProbFrameGeneration(float probability) {
   // Validate the input.
   if (0 > probability || probability > 1.0) {
      std::cout << "ERROR - invalid theProbFrameGeneration value: " << probability << "; Valid if [0.0, 1.0]" << std::endl;
      return false;
   }
   
   theProbFrameGeneration = probability;
   return true;
}

// Setter for theFrameLength.
bool Configuration::setFrameLength(int length) {
   // Validate the input.
   if (3 > length || length > 20) {
      std::cout << "ERROR - invalid theFrameLength value: " << length << "; Valid if [3, 20]" << std::endl;
      return false;
   }
   
   theFrameLength = length;
   return true;
}

// Setter for theMaxBackoffRetransmitCount.
bool Configuration::setMaxBackoffRetransmitCount(int count) {
   // Validate the input.
   if (0 > count || count > 1024) {
      std::cout << "ERROR - invalid theMaxBackoffRetransmitCount value: " << count << "; Valid if [0, 25]" << std::endl;
      return false;
   }
   
   theMaxBackoffRetransmitCount = count;
   return true;
}

// Getter for theVerboseEnabled.
bool Configuration::getVerboseEnabled() {
   return theVerboseEnabled;
}

// Getter for theSimulationCount.
unsigned int Configuration::getSimulationCount() {
   return theSimulationCount;
}

// Getter for theTimeSlotCount.
unsigned long Configuration::getTimeSlotCount() {
   return theTimeSlotCount;
}

// Getter for theCsmaType.
CSMA_TYPE Configuration::getCsmaType() {
   return theCsmaType;
}
      
// Getter for theProbabilityOfPersistance.
float Configuration::getProbOfPersistance() {
	return theProbabilityOfPersistance;
}

// Getter for theNodeCount.
int Configuration::getNodeCount() {
   return theNodeCount;
}

// Getter for theProbFrameGeneration.
float Configuration::getProbFrameGeneration() {
   return theProbFrameGeneration;
}

// Getter for theFrameLength.
int Configuration::getFrameLength() {
   return theFrameLength;
}

// Getter for theMaxBackoffRetransmitCount.
int Configuration::getMaxBackoffRetransmitCount() {
   return theMaxBackoffRetransmitCount;
}

/**********************************************
 * Helper functions
 *******************/

// Helper function that checks if a line is blank, comment or category.
bool Configuration::checkLineForConfigurationString(std::string line) {
   // Get the first character of the string.
   int index = line.find_first_not_of(" \t");
   
   // Check if the line is empty.
   if (std::string::npos == index) {
      return false;  
   }
   
   // Check if the line is a category, identified by a '[' at index 0.
   if ('[' == line[index]) {
      return false;
   }
   
   // Check if the line is a comment, identified by a '#' at index 0.
   if ('#' == line[index]) {
      return false;
   }
   
   return true;
}

// Helper function that receives a configuration line in csma_config.ini and returns, via input arguments, 
// the key and value.
void Configuration::getConfigKeyAndValue(char* line, std::string& key, std::string& value) {
   // Tokenize the string and store the key and value.
   char* charPtr = strtok(line, "=");
   key = std::string(charPtr);
   charPtr = strtok(NULL, "=");
   value = std::string(charPtr);
}

// Helper function that sets the member variables based on a key/value from the INI file.
bool Configuration::updateConfig(std::string key, std::string value) {
   if ("VERBOSE_LOGGING" == key) {
      // Translate string as bool.
      if ("true" == value) {
         return setVerboseEnabled(true);
      }
      else if ("false" == value) {
         return setVerboseEnabled(false);
      }
      
      std::cout << "ERROR - unrecognized VERBOSE_LOGGING value: " << value << std::endl;
      return false;
   }
   else if ("SIMULATION_COUNT" == key) {
      return setSimulationCount(strtoul(value.c_str(), NULL, 0));
   }
   else if ("TIME_SLOT_COUNT" == key) {
      return setTimeSlotCount(strtoul(value.c_str(), NULL, 0));
   }
   else if ("PROTOCOL_TYPE" == key) {
      // Translate string as enum.
      if ("Non-Persistent" == value) {
         return setCsmaType(NON_PERSISTENT);
      }
      else if ("1-Persistent" == value) {
         return setCsmaType(ONE_PERSISTENT);
      }
      else if ("p-Persistent" == value) {
         return setCsmaType(P_PERSISTENT);
      }

      std::cout << "ERROR - unrecognized PROTOCOL_TYPE value: " << value << std::endl;
      return false;
   }
   else if ("PROB_PERSISTENCE" == key) {
      return setProbOfPersistance(atof(value.c_str()));
   }
   else if ("NODE_COUNT" == key) {
      return setNodeCount(atoi(value.c_str()));
   }
   else if ("PROB_FRAME_GENERATION" == key) {
      return setProbFrameGeneration(atof(value.c_str()));
   }
   else if ("FRAME_LENGTH" == key) {
      return setFrameLength(atoi(value.c_str()));
   }
   else if ("MAX_RETRANSMIT_ATTEMPTS" == key) {
      return setMaxBackoffRetransmitCount(atoi(value.c_str()));
   }
   
   // Unexpected key, error.
   std::cout << "ERROR - unrecognized key: " << key << std::endl;
   return false;
}
