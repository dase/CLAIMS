/*
 * Config.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: wangli
 */

#include "Config.h"
#include "Debug.h"
#include <libconfig.h++>
#include <iosfwd>
#include <iostream>
#define DEBUG_Config

std::string Config::config_file="./conf/config";
Config* Config::instance_=0;

/**
 * This parameter specifies the maximum degrees of parallelism
 * for each expander.
 */
int Config::max_degree_of_parallelism;

/*
 * This parameter specifies the data path to search for data file,
 * e.g., T0G0P0,...
 */
std::string Config::data_dir;

/* this parameter control whether expander tracker change the degree
 *  of parallelism in each expander based on the workload.*/
bool Config::enable_expander_adaptivity;

/*
 * This parameter sets the frequency, at which expander tracker keeps
 * track of expander's workload and adaptively change the degree of
 * parallelism if needed.
 */
int Config::expander_adaptivity_check_frequency;

/**
 * This parameter sets the initial degree of parallelism for each segment.
 */
int Config::initial_degree_of_parallelism;


int Config::scan_batch;

std::string Config::hdfs_master_ip;

int Config::hdfs_master_port;

Config* Config::getInstance() {
	if(instance_==0){
		instance_=new Config();
	}
	return instance_;
}

Config::Config() {
	initialize();
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

void Config::initialize() {

	/**
	 * open configure file, which path is specified in CONFIG.
	 */
	cfg.readFile(config_file.c_str());

	/*
	 * The following lines set the search attribute name and default value for each parameter.
	 */

	data_dir=getString("data","/home/imdb/data/tpc-h/4_partitions/SF-20/");

	max_degree_of_parallelism=getInt("max_degree_of_parallelism",4);

	expander_adaptivity_check_frequency=getInt("expander_adaptivity_check_frequency",1000);

	enable_expander_adaptivity=getBoolean("enable_expander_adaptivity",false);

	initial_degree_of_parallelism=getInt("initial_degree_of_parallelism",1);

	scan_batch=getInt("scan_batch",10);

	hdfs_master_ip=getString("hdfs_master_ip","10.11.1.190");
	hdfs_master_port=getInt("hdfs_master_port",9000);



#ifdef DEBUG_Config
	print_configure();
#endif
}

std::string Config::getString(std::string attribute_name,
		std::string default_value) {
	std::string ret;
	try{
		ret=(const char*)cfg.lookup(attribute_name.c_str());
	}
	catch (libconfig::SettingNotFoundException &e) {
		ret=default_value;
	}
	return ret;
}

int Config::getInt(std::string attribute_name, int default_value) {
	int ret;
	try{
		ret=cfg.lookup(attribute_name.c_str());
	}
	catch (libconfig::SettingNotFoundException &e) {
		ret=default_value;
	}
	return ret;
}


bool Config::getBoolean(std::string attribute_name, bool defalut_value) {
	bool ret;
	try{
		ret=((int)cfg.lookup(attribute_name.c_str()))==1;
	}
	catch (libconfig::SettingNotFoundException &e) {
		ret=defalut_value;
	}
	return ret;
}

void Config::print_configure() const {
	std::cout<<"data:"<<data_dir<<std::endl;
	std::cout<<"max_degree_of_parallelism:"<<max_degree_of_parallelism<<std::endl;
	std::cout<<"expander_adaptivity_check_frequency:"<<expander_adaptivity_check_frequency<<std::endl;
	std::cout<<"enable_expander_adaptivity:"<<enable_expander_adaptivity<<std::endl;
	std::cout<<"initial_degree_of_parallelism:"<<initial_degree_of_parallelism<<std::endl;
	std::cout<<"hdfs master ip:"<<hdfs_master_ip<<std::endl;
	std::cout<<"hdfs_master_port:"<<hdfs_master_port<<std::endl;


}
