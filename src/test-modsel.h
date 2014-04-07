#include <pll.h>

enum DataType {
	DT_NUCLEIC = 1, DT_PROTEIC = 2, DT_DEFAULT = 0
};


pllInstance * tr;
pllAlignmentData * phylip;
partitionList * partitions;
int numSeqs, numSites, numPatterns;

DataType dataType;
