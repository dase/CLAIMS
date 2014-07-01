/*
 * ExportSerializeIo.h
 *
 *  Created on: 2014年1月16日
 *      Author: SCDONG
 */

#ifndef EXPORTSERIALIZEIO_H_
#define EXPORTSERIALIZEIO_H_

#include <cstring>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
namespace decimal {

class ExportSerializeOutput {
public:
	ExportSerializeOutput(void *buffer, size_t capacity) :
		buffer_(NULL), position_(0), capacity_(0)
    {
        buffer_ = reinterpret_cast<char*>(buffer);
        assert(position_ <= capacity);
        capacity_ = capacity;
    }

    virtual ~ExportSerializeOutput() {
        // the serialization wrapper never owns its data buffer
    };

    // this explicitly accepts char* and length (or ByteArray)
    // as std::string's implicit construction is unsafe!
    inline void writeBinaryString(const void* value, size_t length) {
        int32_t stringLength = static_cast<int32_t>(length);
        assureExpand(length + sizeof(stringLength));

        char* current = buffer_ + position_;
        memcpy(current, &stringLength, sizeof(stringLength));
        current += sizeof(stringLength);
        memcpy(current, value, length);
        position_ += sizeof(stringLength) + length;
    }

    inline void assureExpand(size_t next_write) {
        size_t minimum_desired = position_ + next_write;
        if (minimum_desired > capacity_) {
            // TODO: die
        }
        assert(capacity_ >= minimum_desired);
    }

    void position(std::size_t pos) {
        position_ = pos;
    }

public:
    // Beginning of the buffer.
    char* buffer_;
    // Current write position in the buffer.
    size_t position_;
    // Total bytes this buffer can contain.
    size_t capacity_;
};

} //endfor namespace
#endif /* EXPORTSERIALIZEIO_H_ */
