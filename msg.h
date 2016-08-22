#ifndef MSG_H__
#define MSG_H__

#include <google/protobuf/io/coded_stream.h>

#include <cstdint>

class Msg {
 public:
	virtual ~Msg();
	virtual bool ParseFromCodedStream(google::protobuf::io::CodedInputStream* input) = 0;

	virtual bool ParseFromArray(const char* input, int length);
};

inline const char* ParseVarint(const char* start, uint32_t* output) {
	uint32_t result = 0;
	int length = 0;
	while (true) {
		unsigned char v = static_cast<unsigned char>(*start++);
		result |= (v & 0x7F) << length;
		if ((v & 0x80) == 0) break;
		length += 7;
	}
	*output = result;
	return start;
}

#endif  // MSG_H__
