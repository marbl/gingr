// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "CodonTable.h"
#include <cctype>
#include <cstring>

// Standard genetic code (NCBI table 1)
const std::unordered_map<std::string, char> CodonTable::geneticCode = CodonTable::initGeneticCode();

std::unordered_map<std::string, char> CodonTable::initGeneticCode()
{
	std::unordered_map<std::string, char> code;
	
	// Phenylalanine (F)
	code["TTT"] = 'F'; code["TTC"] = 'F';
	// Leucine (L)
	code["TTA"] = 'L'; code["TTG"] = 'L';
	code["CTT"] = 'L'; code["CTC"] = 'L'; code["CTA"] = 'L'; code["CTG"] = 'L';
	// Isoleucine (I)
	code["ATT"] = 'I'; code["ATC"] = 'I'; code["ATA"] = 'I';
	// Methionine (M) - Start codon
	code["ATG"] = 'M';
	// Valine (V)
	code["GTT"] = 'V'; code["GTC"] = 'V'; code["GTA"] = 'V'; code["GTG"] = 'V';
	// Serine (S)
	code["TCT"] = 'S'; code["TCC"] = 'S'; code["TCA"] = 'S'; code["TCG"] = 'S';
	code["AGT"] = 'S'; code["AGC"] = 'S';
	// Proline (P)
	code["CCT"] = 'P'; code["CCC"] = 'P'; code["CCA"] = 'P'; code["CCG"] = 'P';
	// Threonine (T)
	code["ACT"] = 'T'; code["ACC"] = 'T'; code["ACA"] = 'T'; code["ACG"] = 'T';
	// Alanine (A)
	code["GCT"] = 'A'; code["GCC"] = 'A'; code["GCA"] = 'A'; code["GCG"] = 'A';
	// Tyrosine (Y)
	code["TAT"] = 'Y'; code["TAC"] = 'Y';
	// Histidine (H)
	code["CAT"] = 'H'; code["CAC"] = 'H';
	// Glutamine (Q)
	code["CAA"] = 'Q'; code["CAG"] = 'Q';
	// Asparagine (N)
	code["AAT"] = 'N'; code["AAC"] = 'N';
	// Lysine (K)
	code["AAA"] = 'K'; code["AAG"] = 'K';
	// Aspartic acid (D)
	code["GAT"] = 'D'; code["GAC"] = 'D';
	// Glutamic acid (E)
	code["GAA"] = 'E'; code["GAG"] = 'E';
	// Cysteine (C)
	code["TGT"] = 'C'; code["TGC"] = 'C';
	// Tryptophan (W)
	code["TGG"] = 'W';
	// Arginine (R)
	code["CGT"] = 'R'; code["CGC"] = 'R'; code["CGA"] = 'R'; code["CGG"] = 'R';
	code["AGA"] = 'R'; code["AGG"] = 'R';
	// Glycine (G)
	code["GGT"] = 'G'; code["GGC"] = 'G'; code["GGA"] = 'G'; code["GGG"] = 'G';
	// Stop codons (*)
	code["TAA"] = '*'; code["TAG"] = '*'; code["TGA"] = '*';
	
	return code;
}

char CodonTable::normalizeBase(char base)
{
	base = toupper(base);
	// Convert U to T for RNA sequences
	if (base == 'U') base = 'T';
	return base;
}

char CodonTable::translate(const char* codon)
{
	if (!codon || strlen(codon) < 3)
		return '?';
	
	// Normalize to uppercase and handle RNA (U->T)
	std::string normalized;
	normalized += normalizeBase(codon[0]);
	normalized += normalizeBase(codon[1]);
	normalized += normalizeBase(codon[2]);
	
	// Check for gaps or ambiguous bases
	if (normalized.find('-') != std::string::npos ||
		normalized.find('N') != std::string::npos)
	{
		return '?';
	}
	
	auto it = geneticCode.find(normalized);
	if (it != geneticCode.end())
	{
		return it->second;
	}
	
	return '?'; // Unknown/invalid codon
}

bool CodonTable::isSynonymous(const char* refCodon, const char* altCodon)
{
	char refAA = translate(refCodon);
	char altAA = translate(altCodon);
	
	// If either translation is invalid, return false
	if (refAA == '?' || altAA == '?')
		return false;
	
	return refAA == altAA;
}