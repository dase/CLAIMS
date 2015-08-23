/*
 * my_glog.cpp
 *  Created on: Jun 12, 2015 11:07:40 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include <glog/logging.h>
#include <stdlib.h>
#include <iosfwd>
#include <string>
#include <ios>
#include "log/logging.h"
namespace claims {
namespace common {

#define LOGDIR "mylog"  // set output log directory
#define MKDIR "mkdir -p "LOGDIR
void GlogSignalHandle(const char* data, int size) {
    std::string str = std::string(data, size);
    /*
     std::ofstream fs("glog_dump.log",std::ios::app);
     fs<<str;
     fs.close();
     */
    LOG(ERROR)<< str;
}
Logging::Logging(char* program) {
    system(MKDIR);

    google::InitGoogleLogging(program);

//    google::ParseCommandLineFlags(&argc, &argv, true);

//    set standard output level>=()
//    google::SetStderrLogging(google::INFO);

//  set standard output color
    FLAGS_colorlogtostderr = true;

    // set output path for different level
    FLAGS_log_dir = LOGDIR;
//    google::SetLogDestination(google::ERROR,"log/error_");
//    google::SetLogDestination(google::INFO,LOGDIR"/INFO_");
//    google::SetLogDestination(google::WARNING,LOGDIR"/WARNING_");
//    google::SetLogDestination(google::ERROR,LOGDIR"/ERROR_");

    FLAGS_stderrthreshold = google::FATAL;

// set every time output cache, default = 30, now to 0
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 100;
    FLAGS_stop_logging_if_full_disk = true;

// set extend name of file
//    google::SetLogFilenameExtension("91_");

// catch core dumped
    google::InstallFailureSignalHandler();
    // set self output way when catch SIGSEGV signal
    google::InstallFailureWriter(&GlogSignalHandle);
}

Logging::~Logging() {
//    google::ShutDownCommandLineFlags();
    google::ShutdownGoogleLogging();
}
}   // namespace common
}   // namespace claims
