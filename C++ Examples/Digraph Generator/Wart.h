/*
Ben Golden
CPSC 2120
2/1/2016
*/

#ifndef WART_H
#define WART_H

#include <iostream>
#include "Digraph.h"

class Wart{

public :

	Wart(std::string & newString);
	void encode(int width);
	void decode(Digraph &digraph);
	friend std::ostream& operator<<(std::ostream &out, const Wart &ob);

private :

	std::string string;
};

#endif
