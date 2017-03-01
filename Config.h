/*
 * Config.h
 *
 *  Created on: Apr 4, 2014
 *      Author: wangli
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <libconfig.h++>

/**
 * This class maintains all the parameter values, which are specified in
 * "config" file.
 * In the debug version, in the initialization phase, default values are usded
 * in case
 * that the value is not given in "config" file.
 */

class Config {
 public:
  static Config* getInstance();
  void print_configure() const;
  virtual ~Config();

 private:
  Config();
  void initialize();

  /**
   * Get the configured string value specified by "attribute_name". Return
   * "default_vale"
   * if the "attribute_name" is not found in the configure file.
   */
  std::string getString(std::string attribute_name, std::string default_value);

  /**
   * Get the configured integer value specified by "attribute_name". Return
   * "default_vale"
   * if the "attribute_name" is not found in the configure file.
   */
  int getInt(std::string attribute_name, int default_value);

  /**
   * Get the configured boolean value specified by "attribute_name". Return
   * "default_vale"
   * if the "attribute_name" is not found in the configure file.
   * true=1, false=0.
   */
  bool getBoolean(std::string attribute_name, bool defalut_value);

  static void setConfigFile(std::string file_name);

 public:
  static std::string config_file;
  /**
   * parameter values.
   * Note: these values are static, so that they can be accessed directly, like
   * Config::data_dir.
   * Static value must be defined in *.cpp file, otherwise compiler will
   * complain.
   */
  static std::string data_dir;
  static int max_degree_of_parallelism;
  static int initial_degree_of_parallelism;
  static bool enable_expander_adaptivity;
  static int expander_adaptivity_check_frequency;
  static int scan_batch;
  static std::string hdfs_master_ip;
  static int hdfs_master_port;
  static std::string logfile;
  static bool master;
  static bool local_disk_mode;
  static bool pipelined_exchange;
  static int client_listener_port;
  static bool enable_codegen;
  static bool enable_prune_column;
  static std::string catalog_file;
  static int thread_pool_init_thread_num;
  static int memory_utilization;

  static int load_thread_num;

 private:
  static Config* instance_;
  libconfig::Config cfg;
};

#endif /* CONFIG_H_ */
