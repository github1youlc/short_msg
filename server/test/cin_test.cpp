#include <string>
#include <iostream>
using namespace std;

#include <stdio.h>


int main(){
	printf("test start\n");

	string line;
	while(getline(std::cin, line)){
		printf("line: %s\n",line.c_str());
	}
	return 0;
}