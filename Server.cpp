#include <Test/TestMain.cpp>

#include <startup.h>
#include "Parsetree/ExecuteLogicalQueryPlan.h"
#include "/usr/include/assert.h"
#include "Test/set_up_environment.h"
#include <getopt.h>

#define AUTU_MASTER
//#define FORK
//#define DEBUG_MODE


struct option long_options[] = {
   {"config-file", required_argument, 0, 'c'},
   {"help", no_argument, 0, 'h'},
   {"disable-codegen",no_argument,0,'d'}
 };

std::string help_info=std::string("-c --config-file FILE_NAME\n\t\tSpecify the configure file\n")+
		"-h --help\n\t\tPrint the help info\n";


void handle_parameters(int argc, char** argv){
	int opt;
	while ((opt = getopt_long(argc, argv, "c:h", long_options, NULL)) != -1) {
		switch (opt) {
		case 'c':
			Config::config_file = std::string(optarg);
			break;

		case 'h':
			printf("%s",help_info.c_str());
			exit(1);
			break;
		case 'd':
			Config::enable_codegen=false;
			break;
		default:
			printf("Invalid parameters! Try -h/--help\n");
			exit(1);
			break;
		}
	}

}
int main(int argc, char** argv){
	handle_parameters(argc,argv);
#ifndef DEBUG_MODE
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
#else
	maina(argc, argv);
	return 0;
#endif
}
