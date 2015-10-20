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

using std::vector;
using std::string;
enum open_flag { CREATEE, APPENDD };
class Connector {
 public:
  explicit Connector(vector<vector<string> > writepath)
      : writepath(writepath) {}
  virtual ~Connector() {}

  virtual bool openFiles(open_flag open_flag_) = 0;
  virtual bool closeFiles() = 0;

  virtual int flush(unsigned projection_offset, unsigned partition_offset,
                    const void* source, unsigned length) = 0;

 private:
 protected:
  vector<vector<string> > writepath;
};

class FileConnector {
 public:
  virtual ~FileConnector();
  virtual bool Open(open_flag flag) = 0;
  virtual bool Close() = 0;
  virtual int flush(unsigned projection_offset, unsigned partition_offset,
                    const void* source, unsigned length) = 0;
};

class SingleFileConnector : public FileConnector {
 public:
  explicit SingleFileConnector(string file_name) : file_name_(file_name) {}
  virtual bool Open(open_flag flag) = 0;
  virtual bool Close() = 0;
  virtual int flush(unsigned projection_offset, unsigned partition_offset,
                    const void* source, unsigned length) = 0;

  virtual bool CanAccess() = 0;
  /**
   * @brief Method description: load total file into memory
   * @param buffer: set buffer point to a new memory allocated by this method,
   * length is the length of file
   * @param length: set to the length of file, also the length of new memory
   * @return  kSuccess if succeed.
   * @details   (additional) this method will modify buffer, set to a new memory
   */
  virtual RetCode LoadTotalFile(void*& buffer, int64_t& length) = 0;

  /**
   * @brief Method description: load a part of file into memory pointed by
   * buffer
   * @param buffer: point to the memory where file is load
   * @param start: the position from which to read
   * @param length: the length of data need to read
   * @return  kSuccess if OK
   * @details   (additional)
   */
  virtual RetCode LoadFile(void* buffer, int64_t start, int64_t length) = 0;

 private:
  string file_name_;
};
#endif /* CONNECTOR_H_ */
