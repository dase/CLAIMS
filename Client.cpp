#include <stdio.h>
/*
 * Client.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: wangli
 */
#include <stdio.h>
#include "Client/ClientResponse.h"
#include "common/Block/ResultSet.h"
#include "Client/Client.h"
#include "common/Logging.h"
int main(){
	/* Client */
	Client client;
	client.connection("127.0.0.1", 10000);
	std::cout << "Please input the query cmd!" << std::endl;
	while(1){
		std::cout<<">";
		std::string query;
		char *args = new char[65535];
		std::cin.getline(args, 65535);
		query.append(args);
		delete args;
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


