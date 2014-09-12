#include <Test/TestMain.cpp>

#include <startup.cpp>
#include "Parsetree/ExecuteLogicalQueryPlan.h"
int main(int argc, char** argv){


//	Config::getInstance();
//	if(Config::master==true){
//
//		Environment::getInstance(true);
//		print_welcome();
//		ExecuteLogicalQueryPlan();
//		Environment::getInstance()->~Environment();
//	}
//	else{
//		int pid=fork();
//		if(pid!=0){
//			printf("Daemon process id is %d\n",pid);
//			sleep(1);
//			exit(0);
//		}
//		Config::getInstance();
//		printf("Creating Slave daemon...\n");
//		printf("Logs are redirected to %s\n",Config::logfile.c_str());
//		FILE *file;
//		if((file=freopen(Config::logfile.c_str(),"a",stdout))==NULL){
//			printf("output redirection fails!\n");
//		}
//		freopen(Config::logfile.c_str(),"a",stderr);
//
//		Environment::getInstance(false);
//		printf("The Slave daemon is successfully created-_-!\n");
//		fflush(stdout);
//		fclose(stdout);
//		//redirect back to console
//		freopen("/dev/tty","a",stdout);
//		printf("The Slave daemon is successfully created!\n");
//		//redirect again to the log file.
//		if((file=freopen(Config::logfile.c_str(),"a",stdout))==NULL){
//			printf("output redirection fails!\n");
//		}
//		freopen(Config::logfile.c_str(),"a",stderr);
//		while(true)
//			sleep(1);
//	}

	maina(argc, argv);
	return 0;
}
