/*
 * hashmap.hpp
 *
 *  Created on: 2013-5-20
 *      Author: root
 */

#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_

#include <boost/unordered_map.hpp>
#include <boost/get_pointer.hpp>

#include "foreach.hpp"


template <typename Key, typename Value>
class hashmap: public boost::unordered_map<Key, Value>
{
public:
	// Checks whether this map contains a binding for a key.
	bool contains(const Key& key) const {return count(key) > 0;}

	// Checks whether there exists a bound value in this map.
	bool containsValue(const Value& v) const
	{
		foreachvalue (const Value& value, *this) {
			if(value == v){
				return true;
			}
		}
		return false;
	}

};

#endif /* HASHMAP_HPP_ */
