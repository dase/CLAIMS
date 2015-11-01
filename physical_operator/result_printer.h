/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /CLAIMS/physical_operator/result_printer.h
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */

#ifndef PHYSICAL_OPERATOR_RESULT_PRINTER_H_
#define PHYSICAL_OPERATOR_RESULT_PRINTER_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../physical_operator/physical_operator_base.h"
#include "../common/Schema/Schema.h"
namespace claims {
namespace physical_operator {
class ResultPrinter : public PhysicalOperatorBase {
 public:
  struct State {
    Schema* schema_;
    PhysicalOperatorBase* child_;
    unsigned block_size_;
    std::string spliter_;
    std::vector<std::string> attribute_name_list_;
    State(){};
    ~State();
    State(Schema* const& schema, PhysicalOperatorBase* const& child,
          const unsigned& block_size, std::vector<std::string> attribute_name,
          std::string spliter = "\t")
        : schema_(schema),
          child_(child),
          block_size_(block_size),
          spliter_(spliter),
          attribute_name_list_(attribute_name){};
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar& schema_& child_& block_size_& spliter_& attribute_name_list_;
    }
  };
  ResultPrinter();
  ResultPrinter(State state);
  virtual ~ResultPrinter();
  bool Open(const PartitionOffset& offset);
  bool Next(BlockStreamBase* block);
  bool Close();
  void Print();

 private:
  State state_;
  BlockStreamBase* block_buffer_;
  unsigned long tuple_count_;
  /* for boost::serializtion*/
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this) & state_;
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  // PHYSICAL_OPERATOR_RESULT_PRINTER_H_
