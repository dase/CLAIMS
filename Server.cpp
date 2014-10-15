#include <Test/TestMain.cpp>

#include <startup.h>
#include "Parsetree/ExecuteLogicalQueryPlan.h"
#include "/usr/include/assert.h"
#include "Test/set_up_environment.h"

#define AUTU_MASTER
//#define FORK

int main(int argc, char** argv){

	bool master;
#ifndef AUTU_MASTER
	printf("If Master, please print 1!\n");
	int master=0;
	scanf("%d",&master);
#else
	Config::getInstance();
	master=Config::master;
#endif
	std::string actor;
	if(master)
		actor="master";
	else
		actor="slave";

#ifndef FORK
	if(master){
		Environment::getInstance(master);
//		create_poc_data_four_partitions();
//		create_poc_data_one_partitions();
//		print_welcome();
//		ExecuteLogicalQueryPlan();
		while(true)
			sleep(1);
	}
	else {
		Environment::getInstance(master);
		while(true)
			sleep(1);
	}
#else
		int pid=fork();
		if(pid!=0){
			printf("Daemon process id is %d\n",pid);
			sleep(1);
			return 0;
		}
		Config::getInstance();
		printf("Creating %s daemon...\n",actor.c_str());
		Environment::getInstance(master);
		printf("The %s daemon is successfully created.!\n",actor.c_str());
		printf("Logs are redirected to %s\n",Config::logfile.c_str());
		FILE *file;
		if((file=freopen(Config::logfile.c_str(),"a",stdout))==NULL){
			printf("output redirection fails!\n");
		}
		freopen(Config::logfile.c_str(),"a",stderr);

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
#endif
//	maina(argc, argv);
	return 0;
}
