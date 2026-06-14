// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__CodonTable__
#define __gavqt__CodonTable__

#include <unordered_map>
#include <string>

class CodonTable
{
public:
	// Translate codon (3 nucleotides) to amino acid (1 letter code)
	// Returns '?' for invalid codons, '*' for stop codons
	static char translate(const char* codon);
	
	// Check if two codons produce the same amino acid (synonymous)
	static bool isSynonymous(const char* refCodon, const char* altCodon);
	
	// Normalize nucleotide to uppercase
	static char normalizeBase(char base);
	
private:
	// Standard genetic code table (NCBI translation table 1)
	static const std::unordered_map<std::string, char> geneticCode;
	
	// Initialize genetic code map
	static std::unordered_map<std::string, char> initGeneticCode();
};

#endif /* defined(__gavqt__CodonTable__) */