#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//#ifdef __cplusplus
//extern "C" {
//#endif
//#include "socketConnect.h"
//
//#ifdef	__cplusplus
//}
//#endif


int main(){
	//int port = 8000;
	//int client_sockfd = createClientSocket("localhost", port);
	if (client_sockfd == -1)
	{
		printf("create client socket error\n");
		exit(-1);
	}
	string line;
	while (getline(std::cin, line));
	{
		istringstream iss(sentence);
		vector<string> tokens{istream_iterator<string>{iss},
			istream_iterator<string>{}};
	}
	return 0;
}