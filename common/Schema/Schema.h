/*
 * Schema.h
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */

#ifndef SCHEMA_H_
#define SCHEMA_H_
#include <vector>
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../data_type.h"

enum RawDataSource { kFile = 0, kSQL };

class Schema {
 public:
  enum schema_type { fixed, varaible };
  Schema(const std::vector<column_type>& columns);
  Schema(const Schema& r);
  Schema(){};
  virtual ~Schema();
  virtual unsigned getTupleMaxSize() const = 0;

  inline virtual unsigned getTupleActualSize(void* tuple) const = 0;
  virtual void getColumnValue(unsigned index, void* src, void* desc) = 0;

  /*get the offset of the specified tuple
   * Note this function is only applicable for the fixed schema, since the
   * offset
   * for a given column in variable schema could varies from tuples to tuples.
   */
  virtual int getColumnOffset(unsigned index) const = 0;

  inline virtual void* getColumnAddess(const unsigned& index,
                                       const void* const& column_start) const
      __attribute__((always_inline)) = 0;
  inline virtual unsigned copyTuple(void* src, void* desc) const = 0;
  unsigned getncolumns() const;
  virtual Schema* getSubSchema(std::vector<unsigned>) const = 0;
  virtual Schema* duplicateSchema() const = 0;
  inline const column_type& getcolumn(const unsigned index) const {
    return columns[index];
  }
  virtual schema_type getSchemaType() const = 0;
  virtual void addColumn(column_type ct, unsigned size){};
  virtual void displayTuple(const void* tuple_start_address,
                            const char* spliter = "|") const;
  /**
   * @brief Method description: see more in its derived class
   */
  virtual bool CheckAndToValue(std::string text_tuple, void* binary_tuple,
                       const string attr_separator,
                       RawDataSource raw_data_source,
                       vector<unsigned>& warning_columns_index) {
    assert(false);
  }
  inline virtual void showAccum_off(){};
  bool hasSameSchema(Schema* schema);
  std::vector<column_type> columns;

  virtual std::string getColumnValue(const void* tuple_start_address, int i);

 protected:
 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& columns;
  }
};

#endif /* SCHEMA_H_ */
