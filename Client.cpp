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
		std::string query;



		std::string input;
//		readStrigFromTerminal(input);

		get_one_command(input);
//		sleep(3);
//		input = "select count(*) from (select row_id ,count(*) from cj group by row_id order by sec_code) as b;";


		query.append(input.c_str());
		if( query == "exit" ){
			break;
		}else if( query.empty() ){
			continue;
		}
		ClientResponse* response = client.submitQuery(query);

		if( query == "shutdown" ){
			break;
		}

		if (response->status == OK) {
			ResultSet rs;
			ClientLogging::log("Client get server response ok: %s\n",
					response->content.c_str());
			if(response->status == CHANGE){
				printf("%s\n",
						response->content.c_str());
			}
			else{
				while (response->status != END) {

					switch(response->status){
					case SCHEMA:
						rs.schema_=response->getSchema();
						break;
					case HEADER:
						rs.column_header_list_=response->getAttributeName().header_list;
						break;
					case DATA:
						assert(rs.schema_!=0);
						rs.appendNewBlock(response->getDataBlock(rs.schema_));
						break;
					}

					response = client.receive();

					ClientLogging::log("Message: %s\n", response->getMessage().c_str());
				}
				rs.query_time_=atof(response->content.c_str());

				rs.print();
			}
		} else {
			printf("%s\n",
					response->content.c_str());
		}
	}
	client.shutdown();
}


