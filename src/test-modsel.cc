#include "test-modsel.h"

#include <string>
#include <iostream>
#include "ConfigParser.h"

using namespace std;

void buildAlignment(string alignfile, DataType dt, pllQueue * partitions);

int main(int argc, char *argv[]) {

	if (argc < 3) {
		cout << "Usage: " << argv[0] << " alignment configfile" << endl;
		exit(-1);
	}

	dataType = DT_NUCLEIC;

	cout << "Reading partitions" << endl;
	ConfigParser cp(argv[2]);
	cout << "Building alignment" << endl;
	string alignFile(argv[1]);
	buildAlignment(alignFile, dataType, cp.getPllPartitions());

	pllLoadAlignment(tr, phylip, partitions, PLL_SHALLOW_COPY);
	pllComputeRandomizedStepwiseAdditionParsimonyTree(tr, partitions);
	tr->start = tr->nodep[1];

	// (t1,(t2,t3),t4);

	pllInitModel(tr, partitions, phylip);
	for (int i = 0; i < 2; i++) {
		partitions->partitionData[i]->optimizeAlphaParameter = PLL_TRUE;
		partitions->partitionData[i]->optimizeBaseFrequencies = PLL_TRUE;
		partitions->partitionData[i]->optimizeSubstitutionRates = PLL_TRUE;
	}

//	pllTreeToNewick(tr->tree_string, tr, partitions, tr->start, PLL_TRUE, PLL_TRUE, PLL_FALSE, PLL_TRUE,
//			PLL_FALSE, PLL_FALSE, PLL_FALSE, PLL_FALSE);
	pllTreeToNewick(tr->tree_string, tr, partitions, tr->start->back, PLL_FALSE,
		PLL_TRUE, PLL_TRUE, PLL_FALSE, PLL_FALSE, PLL_SUMMARIZE_LH,
		PLL_FALSE, PLL_FALSE);
	cout << "Tr : " << tr->tree_string;

	pllEvaluateLikelihood(tr, partitions, tr->start, PLL_TRUE, PLL_FALSE);
	cout << "LK is " << tr->likelihood << endl;

	pllRaxmlSearchAlgorithm(tr, partitions, PLL_TRUE);
	cout << "LK is " << tr->likelihood << endl;

	pllTreeToNewick(tr->tree_string, tr, partitions, tr->start->back, PLL_TRUE,
			PLL_TRUE, PLL_TRUE, PLL_FALSE, PLL_FALSE, PLL_SUMMARIZE_LH,
			PLL_FALSE, PLL_FALSE);
		cout << "Tr : " << tr->tree_string;
}

void buildAlignment(string alignfile, DataType dt, pllQueue * pllPartitions) {

	pllInstanceAttr * attr = (pllInstanceAttr *) malloc(
			sizeof(pllInstanceAttr));
	attr->rateHetModel = PLL_GAMMA;
	attr->fastScaling = PLL_FALSE;
	attr->saveMemory = PLL_FALSE;
	attr->useRecom = PLL_FALSE;
	attr->randomNumberSeed = rand();
#ifdef PTHREADS
	attr->numberOfThreads = 2;
#else
	attr->numberOfThreads = 1;
#endif

	tr = pllCreateInstance(attr);
	free(attr);
	phylip = pllParseAlignmentFile(PLL_FORMAT_PHYLIP, alignfile.c_str());
	/* commit the partitions and build a partitions structure */
	partitions = pllPartitionsCommit(pllPartitions, phylip);

	for (int i = 0; i < partitions->numberOfPartitions; i++) {
		partitions->partitionData[i]->dataType = dataType;
	}

	pllTreeInitTopologyForAlignment(tr, phylip);

	numSeqs = phylip->sequenceCount;
	numSites = phylip->sequenceLength;
	pllAlignmentRemoveDups(phylip, partitions);
	numPatterns = phylip->sequenceLength;

	cout << " - " << numSeqs << "x" << numSites << " (" << numPatterns
			<< " patterns)" << endl;
}
