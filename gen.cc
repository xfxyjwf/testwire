#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	int count;
	string id;
	if (argc == 1) {
		count = 10;
		id = "10";
	} else {
		istringstream in(argv[1]);
		in >> count;
		id = argv[1];
	}

	string n1 = "data.h";
	string n2 = "data.cc";
	string n3 = "data.proto";

	ofstream f1(n1.c_str());
	f1 << "#include \"msg.h\"" << endl;
	f1 << "#include \"data.pb.h\"" << endl;

	f1 << "class Data : public Msg {" << endl;
	f1 << " public:" << endl;
	for (int i = 1; i <= count; ++i) {
		f1 << "  int value" << i << ";" << endl;
	}
	f1 << "  bool ParseFromCodedStream(google::protobuf::io::CodedInputStream* input);" << endl;
	f1 << "  bool ParseFromArray(const char* input, int length);" << endl;
	f1 << "  bool ParseFromTable(const char* input, int lenght);" << endl;
	f1 << "  void SerializeToArray(char* output) const;" << endl;
	f1 << "  int ByteSize() const;" << endl;
	f1 << "};" << endl;

	f1 << "void GenerateProto(DataProto* output);" << endl;
	f1 << "void CopyProto(const DataProto& input, Data* output);" << endl;
	f1 << "bool ProtoFromArray(const char* input, int length, DataProto* output);" << endl;
	f1 << "bool ProtoFromDirect(const char* input, int length, DataProto* output);" << endl;
	f1 << "int Sum(const Data& input);" << endl;
	f1 << "int Sum(const DataProto& input);" << endl;
	f1.close();

	ofstream f2(n2.c_str());
	f2 << "#include \"data.h\"" << endl;

	f2 << "int offset_table[] = {" << endl;
	f2 << "  0," << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  offsetof(Data, value" << i << ")," << endl;
	}
	f2 << "};" << endl;

	f2 << "using google::protobuf::io::CodedInputStream;" << endl;
	f2 << "bool Data::ParseFromTable(const char* d, int length) {" << endl;
	f2 << "  CodedInputStream input(reinterpret_cast<const google::protobuf::uint8*>(d), length);" << endl;
	f2 << "  google::protobuf::uint32 key, value;" << endl;
	f2 << "  while (input.ReadLittleEndian32(&key) && input.ReadLittleEndian32(&value)) {" << endl;
	f2 << "    if (key < 1 || key > " << count << ") return false;" << endl;
	f2 << "    memcpy(reinterpret_cast<char*>(this) + offset_table[key], &value, 4);" << endl;
	f2 << "  }" << endl;
	f2 << "  return true;" << endl;
	f2 << "}" << endl;

	f2 << "bool Data::ParseFromCodedStream(google::protobuf::io::CodedInputStream* input) {" << endl;
	f2 << "  google::protobuf::uint32 key, value;" << endl;
	f2 << "  while (input->ReadLittleEndian32(&key) && input->ReadLittleEndian32(&value)) {" << endl;
	f2 << "switch_start:" << endl;
	f2 << "    switch (key) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "      case " << i << ": {" << endl;
		f2 << "case" << i << ":" << endl;
		f2 << "        value" << i << " = value;" << endl;
		if (i < count) {
			f2 << "        if (input->ReadLittleEndian32(&key) && input->ReadLittleEndian32(&value)) {" << endl;
			f2 << "          if (key == " << i + 1 << ") goto case" << i + 1 << ";" << endl;
			f2 << "          goto switch_start;" << endl;
			f2 << "        }" << endl;
		}
		f2 << "        break;" << endl;
		f2 << "      }" << endl;
	}
	f2 << "      default: return false;" << endl;
	f2 << "    }" << endl;
	f2 << "  }" << endl;
	f2 << "  return true;" << endl;
	f2 << "}" << endl;


	f2 << "bool ProtoFromDirect(const char* input, int length, DataProto* output) {" << endl;
	f2 << "  const char* end = input + (length & ~0x7);" << endl;
	f2 << "  while (input != end) {" << endl;
	f2 << "    int key, value;" << endl;
	f2 << "    memcpy(&key, input, 4);" << endl;
	f2 << "    memcpy(&value, input + 4, 4);" << endl;
	f2 << "    input += 8;" << endl;
	f2 << "switch_start:" << endl;
	f2 << "    switch (key) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "      case " << i << ": {" << endl;
		f2 << "case" << i << ":" << endl;
		f2 << "        output->set_value" << i << "(value);" << endl;
		if (i < count) {
			f2 << "        if (input != end) {" << endl;
			f2 << "          memcpy(&key, input, 4);" << endl;
			f2 << "          memcpy(&value, input + 4, 4);" << endl;
			f2 << "          input += 8;" << endl;
			f2 << "          if (key == " << i + 1 << ") goto case" << i + 1 << ";" << endl;
			f2 << "          goto switch_start;" << endl;
			f2 << "        }" << endl;
		}
		f2 << "        break;" << endl;
		f2 << "      }" << endl;
	}
	f2 << "      default: return false;" << endl;
	f2 << "    }" << endl;
	f2 << "  }" << endl;
	f2 << "  return true;" << endl;
	f2 << "}" << endl;

	f2 << "bool Data::ParseFromArray(const char* input, int length) {" << endl;
	f2 << "  const char* end = input + (length & ~0x7);" << endl;
	f2 << "  while (input != end) {" << endl;
	f2 << "    int key, value;" << endl;
	f2 << "    memcpy(&key, input, 4);" << endl;
	f2 << "    memcpy(&value, input + 4, 4);" << endl;
	f2 << "    input += 8;" << endl;
	f2 << "switch_start:" << endl;
	f2 << "    switch (key) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "      case " << i << ": {" << endl;
		f2 << "case" << i << ":" << endl;
		f2 << "        value" << i << " = value;" << endl;
		if (i < count) {
			f2 << "        if (input != end) {" << endl;
			f2 << "          memcpy(&key, input, 4);" << endl;
			f2 << "          memcpy(&value, input + 4, 4);" << endl;
			f2 << "          input += 8;" << endl;
			f2 << "          if (key == " << i + 1 << ") goto case" << i + 1 << ";" << endl;
			f2 << "          goto switch_start;" << endl;
			f2 << "        }" << endl;
		}
		f2 << "        break;" << endl;
		f2 << "      }" << endl;
	}
	f2 << "      default: return false;" << endl;
	f2 << "    }" << endl;
	f2 << "  }" << endl;
	f2 << "  return true;" << endl;
	f2 << "}" << endl;
 

	f2 << "bool ProtoFromArray(const char* input, int length, DataProto* output) {" << endl;
	f2 << "  const char* end = input + length;" << endl;
	f2 << "  while (input != end) {" << endl;
	f2 << "    uint32_t key, value;" << endl;
	f2 << "    input = ParseVarint(input, &key);" << endl;
	f2 << "    input = ParseVarint(input, &value);" << endl;
	f2 << "switch_start:" << endl;
	f2 << "    switch (key) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "      case " << (i << 3) << ": {" << endl;
		f2 << "case" << i << ":" << endl;
		f2 << "        output->set_value" << i << "(value);" << endl;
		if (i < count) {
			f2 << "        if (input != end) {" << endl;
			f2 << "          input = ParseVarint(input, &key);" << endl;
			f2 << "          input = ParseVarint(input, &value);" << endl;
			f2 << "          if (key == " << (i + 1) * 8 << ") goto case" << i + 1 << ";" << endl;
			f2 << "          goto switch_start;" << endl;
			f2 << "        }" << endl;
		}
		f2 << "        break;" << endl;
		f2 << "      }" << endl;
	}
	f2 << "      default: return false;" << endl;
	f2 << "    }" << endl;
	f2 << "  }" << endl;
	f2 << "  return true;" << endl;
	f2 << "}" << endl;

	f2 << "int Data::ByteSize() const {" << endl;
	f2 << "  return " << count * 8 << ";" << endl;
	f2 << "}" << endl;
	f2 << "void Data::SerializeToArray(char* output) const {" << endl;
	f2 << "  int key;" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  key = " << i << ";" << endl;
		f2 << "  memcpy(output, &key, 4);" << endl;
		f2 << "  memcpy(output + 4, &value" << i << ", 4);" << endl;
		f2 << "  output += 8;" << endl;
	}
	f2 << "}" << endl;

	f2 << "void GenerateProto(DataProto* output) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  output->set_value" << i << "(" << random() % 1000 << ");" << endl;
	}
	f2 << "}" << endl;

	f2 << "void CopyProto(const DataProto& input, Data* output) {" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  output->value" << i << " = input.value" << i << "();" << endl;
	}
	f2 << "}" << endl;

	f2 << "int Sum(const DataProto& input) {" << endl;
	f2 << "  int result = 0;" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  result += input.value" << i << "();" << endl;
	}
	f2 << "  return result;" << endl;
	f2 << "}" << endl;

	f2 << "int Sum(const Data& input) {" << endl;
	f2 << "  int result = 0;" << endl;
	for (int i = 1; i <= count; ++i) {
		f2 << "  result += input.value" << i << ";" << endl;
	}
	f2 << "  return result;" << endl;
	f2 << "}" << endl;
	f2.close();

	ofstream f3(n3.c_str());
	f3 << "message DataProto {" << endl;
	for (int i = 1; i <= count; ++i) {
		f3 << "  optional int32 value" << i << " = " << i << ";" << endl;
	}
	f3 << "}" << endl;
	f3.close();
}
