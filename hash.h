/*
    Copyright 2011, Spyros Blanas.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Modified by wangli On Otc.15,2013
 */

#ifndef __MYHASHFUNCTION__
#define __MYHASHFUNCTION__

class PartitionFunction {
	
	public:
		enum partition_fashion{hash_f,range_f,round_robin_f};
		/**
		 * Creates a new hashing object, rounding the number of buckets
		 * \a k to the next power of two. Formally \a k will become
		 * \f$ k = 2^{\lceil{\log_2{k}}\rceil} \f$
		 * @param min Minimum hash value
		 * @param max Maximum hash value
		 * @param k Number of buckets. Value will be rounded to the next
		 * higher power of two.
		 */
		PartitionFunction(int min, int max, unsigned int k);
		virtual ~PartitionFunction(){};
		virtual partition_fashion getPartitionFashion()const=0;
		/**
		 * Returns the bucket number \f$n\in[0,k)\f$ for this \a value.
		 * @param value Value to hash. Must be within bounds.
		 * @return Bucket number.
		 */
		virtual unsigned int get_partition_value(int value) = 0;

		virtual unsigned int get_partition_value(double value)=0;

		virtual unsigned int getNumberOfPartitions();

	protected:
		int min_, max_;
		unsigned long int k_;	/**< \f$ \_k=log_2(k) \f$, where \f$k\f$ is number of buckets */

};

/**
 * This class is used to achieve Round Robin Partition
 */
class RoundRobinPartitionFunction :public PartitionFunction{
public:
	RoundRobinPartitionFunction(int range)
		:PartitionFunction(0,0,0),range_(range),cur_(0){}
	~RoundRobinPartitionFunction(){};
	/*return the id of partition in round robin*/
	inline unsigned int get_partition_value(int value){
		return cur_++%range_;
	}
	inline unsigned int get_partition_value(double value){
		return cur_++%range_;
	}
	inline unsigned int getNumberOfPartitions(){
		return range_;
	}
	partition_fashion getPartitionFashion()const;
private:
	int range_;
	unsigned cur_;
};

/*
 * This class is used to achieve range partition, where each range is of equal size.
 * TODO: implement range partition that can specify each range size.
 */
class UniformRangePartitionFunction : public PartitionFunction {
	public:
		UniformRangePartitionFunction(int min, int max, unsigned int k)
			: PartitionFunction(min, max, k) { }
		~UniformRangePartitionFunction(){};
		inline unsigned int get_partition_value(int value) {
			unsigned long long val = (value-min_);
			val <<= k_;
			return val / (max_-min_+1);
		}
		inline unsigned int get_partition_value(double value) {
			unsigned long long val = (value-min_);
			val <<= k_;
			return val / (max_-min_+1);
		}
		partition_fashion getPartitionFashion()const;
};

class ModuloHashFunction : public PartitionFunction {
	public:
		/**
		 * Skipbits parameter defines number of least-significant bits which
		 * will be discarded before computing the hash.
		 */
		ModuloHashFunction(int min, unsigned int range, unsigned int skipbits)
			: PartitionFunction(min, -1, range) {
			skipbits_ = skipbits;
			k_ = ((1 << k_) - 1) << skipbits_;	// _k is used as the modulo mask
		}
		~ModuloHashFunction(){};
		/** Return h(x) = x mod k. */
		inline unsigned int get_partition_value(int value) {
			return ((value-min_) & k_) >> skipbits_;
		}
		inline unsigned int get_partition_value(double value) {
			return ((*(long*)&value-min_) & k_) >> skipbits_;
		}
		partition_fashion getPartitionFashion()const;
		inline unsigned int getNumberOfPartitions() {
			return (k_ >> skipbits_) + 1;
		}


	private:
		unsigned int skipbits_;
};

class GeneralModuloFunction:public PartitionFunction{
public:
	GeneralModuloFunction(const int &range, const int & min, const int & skipbits)
	: PartitionFunction(min,-1,range*(1>>12)),range_(range),skipbits_(skipbits){

	}
	~GeneralModuloFunction(){};
	inline unsigned int get_partition_value(int value){
		return (((value-min_) & k_) >> skipbits_)%range_;
	}
	inline unsigned int get_partition_value(double value){
		const long tmp=*(long*)&value;
		return ((tmp*16807)%2836+(tmp*19))%range_;
	}
	partition_fashion getPartitionFashion()const;
private:
	unsigned long range_;
	unsigned skipbits_;
};





class PartitionFunctionFactory {
	public:

	/* Modulo hash function supporting arbitary range.*/
	static PartitionFunction* createGeneralModuloFunction(const int &range, const int &min=0,const int &skipbits=0);
	/* The range of ModuloFunction should be a power of 2. If you want to use other range, you should consider
	 * to use GeneralModuloFunction. */
	static PartitionFunction* createModuloFunction(const int &range, const int &min=0, const int &skipbits=0);
	static PartitionFunction* createUniformRangeHashFunction(const int &range, const int &min, const int &max);
	static PartitionFunction* createRoundRobinPartitionHashFunction(int range);
};

#endif
