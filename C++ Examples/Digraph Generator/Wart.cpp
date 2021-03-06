/*
Ben Golden
CPSC 2120
2/1/2016
*/

#include <iostream>
#include "Wart.h"
#include "Digraph.h"

//Constructor for Wart which takes a string
Wart::Wart(std::string & newString){

	string = newString;

};

//Encode encodes the given Wart class and takes an int for width
void Wart::encode(int width){

	int area = int(string.length());

	int k = 0;

	std::string e_string(string);

	//Nested for loop which takes values from string and rearranges them
	//First for loop goes along the width of the rectangle
	for(int i = 0; i < width; i++){

		//Second for loop goes along length (area/width) of the rectangle
		for(int j = i; j < area; j += width){
			
			//k increments with each loop, allowing each value in e_string to be filled
			e_string.at(k) = string.at(j);
			k++;
		}
	}

	//Replace string and append "."
	string = e_string + ".";

}

//Decodes a given encoded Wart and uses the digraphs to replace string with a decoded string
void Wart::decode(Digraph &digraph){

	//Uses a similar method to the encoder, taking area, length, and the index k
	int area = int(string.length());
	int length, k;

	//Array of strings to be compared, which are all initialized with the given string
	std::string d_string[area];

	for(int i = 0; i < area; i++){

		d_string[i] = string;

	}

	//Goes through each possible width and fills each d_string
	for(int width = 1; width <= area; width++){

		k = 0;

		length = (area + 1)/width;

		//Similar to the encode loop, but length replaces width
		for(int i = 0; i < length; i++){

			for(int j = i; j < area; j += length){
			
				d_string[width-1].at(k) = string.at(j);
				k++;
			}

		}

	}

	//Compares score of each d_string and replaces string with whichever scores the highest
	for(int i = 0; i < area; i++){

		if(digraph.getScore(d_string[i]) > digraph.getScore(string)){

			string = d_string[i];

		}

	}

}

//Overloaded operator for output
std::ostream& operator<<(std::ostream &out, const Wart &ob){
	
	out << ob.string << std::endl;

	return out;

}
