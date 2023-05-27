# -*- coding: utf-8 -*-
codons = {"UUU":"Phe", "UUC":"Phe", "UUA":"Leu", "UUG":"Leu",
          "UCU":"Ser", "UCC":"Ser", "UCA":"Ser", "UCG":"Ser",
          "UAU":"Tyr", "UAC":"Tyr", "UAA":"STOP", "UAG":"STOP",
          "UGU":"Cys", "UGC":"Cys", "UGA":"STOP", "UGG":"Trp",
          "CUU":"Leu", "CUC":"Leu", "CUA":"Leu", "CUG":"Leu",
          "CCU":"Pro", "CCC":"Pro", "CCA":"Pro", "CCG":"Pro",
          "CAU":"His", "CAC":"His", "CAA":"Gln", "CAG":"Gln",
          "CGU":"Arg", "CGC":"Arg", "CGA":"Arg", "CGG":"Arg",
          "AUU":"Ile", "AUC":"Ile", "AUA":"Ile", "AUG":"Met",
          "ACU":"Thr", "ACC":"Thr", "ACA":"Thr", "ACG":"Thr",
          "AAU":"Asn", "AAC":"Asn", "AAA":"Lys", "AAG":"Lys",
          "AGU":"Ser", "AGC":"Ser", "AGA":"Arg", "AGG":"Arg",
          "GUU":"Val", "GUC":"Val", "GUA":"Val", "GUG":"Val",
          "GCU":"Ala", "GCC":"Ala", "GCA":"Ala", "GCG":"Ala",
          "GAU":"Asp", "GAC":"Asp", "GAA":"Glu", "GAG":"Glu",
          "GGU":"Gly", "GGC":"Gly", "GGA":"Gly", "GGG":"Gly"}
      
def main():
    
    # Replication
    dna = readFile('apoe.txt')
    writeFile('replication.txt', replication(dna))
    
    # Restriction
    dna = readFile('apoe.txt')
    fragment_list = restriction(dna, 'CAGCTG')
    min_fragment_length = 1000000 # Initialize as a large random number so that the length of the first fragment is shorter than that number.
    for fragment in fragment_list:
        if len(fragment) < min_fragment_length:
            shortest_fragment = fragment
            min_fragment_length = len(fragment)
    writeFile('fragment.txt', shortest_fragment)
    
    # Allele
    allele_type = allele(dna)
    if allele_type == 4:
        print('This allele indicates a higher risk of Alzheimer\'s disease.')
    else:
        print('This allele indicates a lower risk of Alzheimer\'s disease.')
    
    pass

def readFile(fileName):
    """
    Reads a text file.
    
    Parameters
    ----------
    fileName : str
        File path to read.

    Returns
    -------
    str
        Text from file.
    """
    with open(fileName,'r') as dnaFile:
        bases = dnaFile.readlines()
        
    return ''.join(bases)
    
    
def writeFile(fileName,text):
    """
    Writes a text file.

    Parameters
    ----------
    fileName : str
        File path to write.
    text : str
        Text to write.

    Returns
    -------
    None.

    """
    with open(fileName,'w') as textFile:
        textFile.write(text)
    
def replication(dna):
    """
    Accepts DNA sequence as a string and returns the complementary sequence.

    Parameters
    ----------
    dna : str
        Given DNA sequence.

    Returns
    -------
    complementary_sequence : str
        Complementary sequence of given DNA sequence.

    """    
    complementary_sequence = ''
    for nucleic_base in dna:
        if nucleic_base == 'A':
            complementary_sequence += 'T'
        if nucleic_base == 'T':
            complementary_sequence += 'A'
        if nucleic_base == 'C':
            complementary_sequence += 'G'
        if nucleic_base == 'G':
            complementary_sequence += 'C'
    return complementary_sequence

def restriction(dna, seq): 
    """
    Cuts DNA at specific locations, given a recognition sequence.

    Parameters
    ----------
    dna : str
        Given DNA sequence.
    seq : str
        Given recognition sequence.

    Returns
    -------
    final_fragment_list : list
        List containing cut fragments.

    """
    if len(seq)%2 == 0:
        
        # Slice seq into two halves.
        n = len(seq)//2
        seq1 = seq[:n]
        seq2 = seq[n:]
        
        # Split dna using seq to create a list with fragments to process.
        initial_fragment_list = dna.split(seq)
        
        # First and last fragments have to be processes separately.
        first_fragment = initial_fragment_list[0] + seq1 # first fragment = first element in list + first half of seq
        last_fragment = seq2 + initial_fragment_list[-1] # last fragment = second half of seq + last element in list
        
        processed_fragment_list = []
        # Use a for loop to process the elements between the first and last elements
        # Add the first half of the sequence to the back and the second half to the front
        for x in range(1, len(initial_fragment_list)-1):
            processed_fragment_list.append(seq2 + initial_fragment_list[x] + seq1)
        
        final_fragment_list = [] 
        # Acknowledge two possibilities: [1] only 2 fragments (first and last) exist; [2] more than 2 fragments exist
        # In order to return the result, determine the elements to add to the final_fragment_list
        if len(initial_fragment_list) == 2:
            final_fragment_list.append(first_fragment)
            final_fragment_list.append(last_fragment)
        else:
            final_fragment_list.append(first_fragment)
            for y in range(len(initial_fragment_list)-2):
                final_fragment_list.append(processed_fragment_list[y])
            final_fragment_list.append(last_fragment)
        
        return final_fragment_list
    
    else:
        raise ValueError("Recognition sequence must be of even length.")


def transcription(dna):
    """
    Accepts DNA sequence as a string and replaces nucleic_base 'T' with 'U',
    then returns the complementary sequence.

    Parameters
    ----------
    dna : str
        Given DNA sequence.

    Returns
    -------
    mrna : str
        mRNA of the given DNA.

    """
    mrna = ''
    for nucleic_base in dna:
        if nucleic_base == 'T':
            mrna += 'U'
        else:
            mrna += nucleic_base
    return mrna

def translation(mrna):
    """
    Accepts mRNA sequence as a string and returns a translated amino acid sequence.

    Parameters
    ----------
    mrna : str
        Given mRNA sequence.

    Returns
    -------
    amino_acid_sequence : str
        Amino acid sequence of the given mRNA.

    """
    split_mrna = [mrna[i:i+3] for i in range(0, len(mrna), 3)] # Create a list by splitting the mrna string per 3 nucleic acids.
    amino_acid_sequence = ''
    for codon in split_mrna:
        if len(codon) == 3: # if-else statement used to ignore incomplete codons
            amino_acid_sequence += (codons[codon] + ' ')
        else:
            continue
    return amino_acid_sequence

def allele(dna):
    """
    Accepts dna sequence as a string and returns allele type (2, 3, or 4) as an integer.

    Parameters
    ----------
    dna : str
        Given DNA sequence.

    Returns
    -------
    2, 4, 3, or -1 : int
        Allele type of the given DNA sequence's YYY and ZZZ.

    """
    # By finding the index of the sequence, we can know which three nucleic bases come beforehand.
    yyy_index = dna.find('GGCCGCCTGGTGCAGTACCGCGGC')
    yyy = dna[yyy_index-3:yyy_index]
    yyy_mrna = transcription(yyy) # To use the codons dictionary, must transcribe dna to mrna
    yyy_amino = codons[yyy_mrna]
    
    zzz_index = dna.find('CTGGCAGTGTACCAGGCC')
    zzz = dna[zzz_index-3:zzz_index]
    zzz_mrna = transcription(zzz)
    zzz_amino = codons[zzz_mrna]
    
    if yyy_amino == 'Cys' and zzz_amino == 'Cys':
        return 2
    elif yyy_amino == 'Arg' and zzz_amino == 'Arg':
        return 4
    elif yyy_amino == 'Cys' and zzz_amino == 'Arg':
        return 3
    else:
        return -1

if __name__ == "__main__": main()