#include "msg.h"

using google::protobuf::io::CodedInputStream;

Msg::~Msg() {
}

bool Msg::ParseFromArray(const char* input, int length) {
	CodedInputStream stream(reinterpret_cast<const google::protobuf::uint8*>(input), length);
	return ParseFromCodedStream(&stream);
}
