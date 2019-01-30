/*
Ben Golden
CPSC 2120
2/1/2016
*/

#include <string>
#include <iostream>

#include "Digraph.h"
#include "Wart.h"

//Function to print the highest scoring digraph
void print_highScore(Digraph &digraph){

	double score = 0.0;

	char a, b;

	for(int i = 97; i < 123; i++){
		for(int j = 97; j < 123; j++){

			if(digraph.getScore(char(i), char(j)) > score){

				a = char(i);
				b = char(j);

				score = digraph.getScore(char(i), char(j));

			}

		}
	}

	std::cout << a << b << " with a proportion of: " << score << std::endl;

}

int main(){

	std::string filename = " ";
	std::string to_decode = " ";

	std::cout << "Enter file name for digraph: ";

	getline(std::cin, filename);

	Digraph digraph(filename);

	std::cout << "Most common was: ";

	print_highScore(digraph);

	std::cout << "Enter sentence terminated by <ENTER>: ";

	getline(std::cin, to_decode);

	Wart decodable(to_decode);

	std::cout << "This gets a score of: " << digraph.getScore(to_decode) << std::endl;

	std::cout << "The decoded sentence is: " << std::endl;

	decodable.decode(digraph);

	std::cout << decodable << std::endl;

	return 0;

}















