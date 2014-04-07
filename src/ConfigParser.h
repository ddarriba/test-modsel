/*
 * ConfigParser.h
 *
 *  Created on: Apr 7, 2014
 *      Author: diego
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include <vector>
#include <string>
#include <pll.h>
#include "SimpleIni.h"

#define MAX_PARTITIONS LONG_MAX
#define MAX_SECTIONS 10
typedef std::vector<unsigned int> t_partitionElementId;

using namespace std;

/**
 * @brief Structure with information about a single-gene partition
 */
struct partitionInfo {
	t_partitionElementId partitionId; /** Partition id */
	int start[MAX_SECTIONS]; /** Starting position */
	int end[MAX_SECTIONS]; /** Ending position */
	int stride[MAX_SECTIONS]; /** Stride for codon position (0 means no codon division) */
	int numberOfSections;
	string name; /** Name of the gene/partition */
	~partitionInfo(void) {
	}
};

class ConfigParser {
public:
	ConfigParser(const char * configFile);
	virtual ~ConfigParser();

	pllQueue * getPllPartitions() const {
		return pllPartitions;
	}
private:
	void parsePartitionsFile(const char * configFile);
	int parsePartitionDetails(char * line, struct partitionInfo * pInfo);

	vector<partitionInfo> * localPartitions; /** Vector of partitions */
	pllQueue * pllPartitions; /** Partitions definition for PLL */
};

#endif /* CONFIGPARSER_H_ */
