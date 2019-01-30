/*
Ben Golden
CPSC 2120
2/1/2016
*/

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <string>

class Digraph{

public :

	Digraph();
	Digraph(std::string filename);
	double getScore(char, char);
	double getScore(std::string);

private :

	float proportions[26][26];

};

#endif
