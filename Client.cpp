/*
 * Client.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: wangli
 */
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <string>
#include <stdio.h>
#include "Client/ClientResponse.h"
#include "common/Block/ResultSet.h"
#include "Client/Client.h"
#include "common/Logging.h"
#include "startup.h"
#include "utility/command_line.h"
#include "utility/rdtsc.h"

void readStrigFromTerminal(string & input){
	while(true){
		std::cin.clear();
		std::cin.sync();
		std::string str;
		if(getline(std::cin,str)){
			bool finish=false;
			for(unsigned i=0;i<str.length();i++){
				if(str[i]==';'){
					input+=str.substr(0,i+1);
					finish=true;
					break;
				}

			}
			if(finish)
				break;
			input+=str+" ";
		}
	}
}

int main(int argc, char** argv){
	/* Client */

	if(argc!=3){
		printf("argc=%d, Illegal input. \nPlease use client master_ip client_listener_port.\n",argc);
		printf("HINT: the master ip and the client_listener_port can be found in the configure file.\n");
		return 0;
	}

	print_welcome();

	Client client;
	client.connection(argv[1], atoi(argv[2]));
	std::cout << std::endl;
	init_command_line();


	while(1){
		std::string command,message;

		get_one_command(command);

		command=trimSpecialCharactor(command);

		if( command == "exit;"||command=="shutdown;" ){
			break;
		}else if( command.empty() ){
			continue;
		}


		ResultSet rs;
		switch(client.submit(command,message,rs)){
		case Client::result:
			rs.print();
			break;
		case Client::message:
			printf("%s",message.c_str());
			break;
		case Client::error:
			printf("%s",message.c_str());
			break;
		default:
			assert(false);
			break;
		}

	}
	client.shutdown();
}


