/*
 * ConfigParser.cc
 *
 *  Created on: Apr 7, 2014
 *      Author: diego
 */

#include "ConfigParser.h"
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <parsePartition.h>

struct comparePartitionInfos {
	inline bool operator()(partitionInfo p1, partitionInfo p2) {

		if (p1.numberOfSections < 1)
			return true;
		if (p2.numberOfSections < 1)
			return false;
		if (p1.start[0] != p2.start[0])
			return p1.start[0] < p2.start[0];
		else
			return p1.stride[0] < p2.stride[0];
	}
};

ConfigParser::ConfigParser(const char * configFile) {
	localPartitions = 0;
	parsePartitionsFile(configFile);
}

ConfigParser::~ConfigParser() {
	// TODO Auto-generated destructor stub
}

void ConfigParser::parsePartitionsFile(const char * configFile) {

	cout << " - parsing " << configFile << endl;

	CSimpleIniA ini;
	ini.SetUnicode();
	SI_Error rc = ini.LoadFile(configFile);
	if (rc < 0)
		exit(-1);

	CSimpleIniA::TNamesDepend keys;

	ini.GetAllKeys("partitions", keys);
	int numberOfPartitions = keys.size();

	cout << " - numParts: " << numberOfPartitions << endl;

	localPartitions = new vector<partitionInfo>(numberOfPartitions);
	char * lineBuffer = (char *) malloc(150);
	int partitionId = 0;
	for (CSimpleIniA::TNamesDepend::iterator it = keys.begin();
			it != keys.end(); it++) {
		CSimpleIniA::Entry entry = *it;
		localPartitions->at(partitionId).name = entry.pItem;
		strcpy(lineBuffer, ini.GetValue("partitions", entry.pItem, "default"));
		parsePartitionDetails(lineBuffer, &localPartitions->at(partitionId));
		cout << "      " << localPartitions->at(partitionId).name << ": "
				<< localPartitions->at(partitionId).start[0] << "-"
				<< localPartitions->at(partitionId).end[0] << endl;
		partitionId++;
	}
	free(lineBuffer);

	std::sort(localPartitions->begin(), localPartitions->end(), comparePartitionInfos());

	pllPartitionRegion * pregion;
	pllPartitionInfo * pinfo;

	pllQueueInit (&pllPartitions);

	for (int i = 0; i < numberOfPartitions; i++) {
		localPartitions->at(i).partitionId.push_back(i);
		pinfo = (pllPartitionInfo *) malloc(sizeof(pllPartitionInfo));
		pllQueueInit(&(pinfo->regionList));
		pllQueueAppend(pllPartitions, (void *) pinfo);

		pinfo->partitionName = (char *) malloc(
				(localPartitions->at(i).name.size() + 1) * sizeof(char));
		strcpy(pinfo->partitionName, localPartitions->at(i).name.c_str());
		pinfo->partitionModel = (char *) malloc(1);

		pinfo->protModels = -1;
		pinfo->protFreqs = -1;
		pinfo->dataType = PLL_DNA_DATA;
		pinfo->optimizeBaseFrequencies = PLL_TRUE;
		for (int j = 0; j < localPartitions->at(i).numberOfSections; j++) {
			pregion = (pllPartitionRegion *) malloc(sizeof(pllPartitionRegion));
			pregion->start = localPartitions->at(i).start[j];
			pregion->end = localPartitions->at(i).end[j];
			pregion->stride = localPartitions->at(i).stride[j];
			pllQueueAppend(pinfo->regionList, (void *) pregion);
		}
	}
}

int ConfigParser::parsePartitionDetails(char * line,
		struct partitionInfo * pInfo) {
	int numberOfSections = 0;
	char * parsed = strtok(line, "-");

	while (parsed != NULL) {
		int start = atoi(parsed);
		parsed = strtok(NULL, ",");
		int end = atoi(parsed);
		pInfo->start[numberOfSections] = start;
		pInfo->end[numberOfSections] = end;
		pInfo->stride[numberOfSections] = 1;

		numberOfSections++;
		parsed = strtok(NULL, "-");
	}
	pInfo->numberOfSections = numberOfSections;

	return 0;
}
