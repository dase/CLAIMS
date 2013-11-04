/*
 * test_adaptiveendpoint.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: wangli
 */
#include <sstream>
#include "../AdaptiveEndPoint.h"
#include "../Executor/Coordinator.h"
int main234112234(int argc, char** argv){
	printf("0 for coordinate, 1 for AdaptiveEndPoint!\n");
	char input;
	scanf("%c",&input);
	if(input=='0'){
		Coordinator* C=new Coordinator();
		//AdaptiveEndPoint* AEP=new AdaptiveEndPoint("AdaptiveEndPoint","10.11.1.224","20202");

		while(true){
			sleep(1);
		}

		C->~Coordinator();
	}
	else{
		printf("Argc=%d\n",argc);
		if(argc!=3){
			printf("Please give the adaptive endpoint ip and port!\n");
			return 1;
		}
		std::ostringstream name;
		name<<"AdaptiveEndPoint://"<<argv[1]<<":"<<argv[2];
		AdaptiveEndPoint* AEP=new AdaptiveEndPoint(name.str().c_str(),argv[1],argv[2]);
		while(true){
			sleep(1);
		}
	}
}
