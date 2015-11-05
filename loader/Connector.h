/*
 * Connector.h
 *
 *  Created on: Sep 19, 2014
 *      Author: scdong
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include <vector>
#include <string>

#include "../common/error_define.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/file_handle/file_handle_imp_factory.h"

using std::vector;
using std::string;

enum open_flag { CREATEE, APPENDD };
class Connector {
 public:
  explicit Connector(vector<vector<string>> writepath) : writepath(writepath) {}
  virtual ~Connector() {}

  virtual bool OpenFiles(open_flag open_flag_) = 0;
  virtual bool CloseFiles() = 0;

  virtual int Flush(unsigned projection_offset, unsigned partition_offset,
                    const void* source, unsigned length) = 0;

 private:
 protected:
  vector<vector<string>> writepath;
};

#endif /* CONNECTOR_H_ */
