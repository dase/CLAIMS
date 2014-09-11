#include <Test/TestMain.cpp>
#include <startup.cpp>
#include "Parsetree/ExecuteLogicalQueryPlan.h"
#include "/usr/include/assert.h"
#include "Test/set_up_environment.h"
int main(int argc, char** argv){


	Config::getInstance();
	if(Config::master==true){

		printf("If Master, please print 1!\n");
		int master=0;
		scanf("%d",&master);
		if(master!=1){
			Client client;
					client.connection("127.0.0.1", 8001);
			//		std::cout << "Please input the query cmd!" << std::endl;
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
							printf("Client get server response ok: %s\n",
									response->content.c_str());

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

								printf("Message: %s\n", response->getMessage().c_str());
							}
							rs.query_time_=atof(response->content.c_str());
							rs.print();

						} else {
							printf("Client does not get response: %s\n",
									response->content.c_str());
						}
					}
					client.shutdown();
		}

		else{
			Environment::getInstance(true);
			print_welcome();
			create_poc_data();
			ExecuteLogicalQueryPlan();
	//		Environment::getInstance()->~Environment();
			while(true)
				sleep(1);
		}
	}
	else{
		int pid=fork();
		if(pid!=0){
			printf("Daemon process id is %d\n",pid);
			sleep(1);
			exit(0);
		}
		Config::getInstance();
		printf("Creating Slave daemon...\n");
		printf("Logs are redirected to %s\n",Config::logfile.c_str());
		FILE *file;
		if((file=freopen(Config::logfile.c_str(),"a",stdout))==NULL){
			printf("output redirection fails!\n");
		}
		freopen(Config::logfile.c_str(),"a",stderr);

		Environment::getInstance(false);
		printf("The Slave daemon is successfully created-_-!\n");
		fflush(stdout);
		fclose(stdout);
		//redirect back to console
		freopen("/dev/tty","a",stdout);
		printf("The Slave daemon is successfully created!\n");
		//redirect again to the log file.
		if((file=freopen(Config::logfile.c_str(),"a",stdout))==NULL){
			printf("output redirection fails!\n");
		}
		freopen(Config::logfile.c_str(),"a",stderr);
		while(true)
			sleep(1);
	}

//	maina(argc, argv);
	return 0;
}
