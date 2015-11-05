/*
 * SchemaFix.h
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */

#ifndef SCHEMAFIX_H_
#define SCHEMAFIX_H_
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <string>
#include <vector>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "./Schema.h"
class SchemaFix : public Schema {
 public:
  SchemaFix(const std::vector<column_type>& columns);
  SchemaFix(const SchemaFix& r);
  SchemaFix() {}
  virtual ~SchemaFix();
  inline unsigned getTupleMaxSize() const;
  inline unsigned getTupleActualSize(void* tuple) const { return totalsize; }
  inline unsigned ingetTupleActualSize(void* tuple) const { return totalsize; }
  inline unsigned getTupleSize() const { return totalsize; }
  inline unsigned copy(void* src, void* desc) const {
    memcpy(desc, src, totalsize);
    return totalsize;
  }
  void getColumnValue(unsigned index, void* src, void* desc);
  inline void* getColumnAddess(const unsigned& index,
                               const void* const& column_start) const
      __attribute__((always_inline)) {
    return (char*)column_start + accum_offsets[index];
  }
  int getColumnOffset(unsigned index) const;
  inline unsigned copyTuple(void* src, void* desc) const {
    memcpy(desc, src, totalsize);
    return totalsize;
  }
  inline Schema::schema_type getSchemaType() const { return Schema::fixed; }
  Schema* getSubSchema(std::vector<unsigned>) const;
  Schema* duplicateSchema() const;
  //	void displayTuple(const void* tuple_start_address,const char*
  // spliter)const;
  void addColumn(column_type ct, unsigned size);
  unsigned getColumnOffset(unsigned index);
  vector<unsigned> toValue(std::string text_tuple, void* binary_tuple,
                           const char attr_separator);
  inline void showAccum_off() {
    for (int i = 0; i < accum_offsets.size(); i++) {
      printf("%d  %d\n", i, accum_offsets[i]);
    }
  }

 private:
  unsigned totalsize;
  std::vector<unsigned> accum_offsets;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<Schema>(*this) & totalsize &
        accum_offsets;
  }
};

#endif /* SCHEMAFIX_H_ */
