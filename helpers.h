/*
 * Declaration of helper functions that belong to no class.
 */

#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <iostream>
#include <cstdlib>   // atof, atoi, exit, rand
#include <vector>

#include "configuration.h"
#include "node.h"
#include "message.h"
#include "metric.h"
#include "CLog.h"

// Forward declarations. Resolves circular dependency issues.
class Configuration;
class Node;
class Message;

// Helper function that loops through each node and determines the state of each node based on the current configuration.
void determineNodeStates(std::vector<Node*> nodeVector, unsigned int currentTime, Configuration* configObj);

// Helper function used to generate a random float [0.0, 1.0].
float generateRandomFloatZeroToOne();

// Helper function used to generate a random integer between minimum and maximum.
int generateRandomIntegerMinToMax(unsigned int min, unsigned int max);

#endif // __HELPERS_H__
