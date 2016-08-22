#include <benchmark/benchmark.h>
#include <iostream>
#include <functional>
#include <algorithm>
#include <string>

#include "data.h"

using namespace std;

string proto_bytes;
string data_bytes;
int sum;

void Init() {
	DataProto p;
	GenerateProto(&p);
	Data d;
	CopyProto(p, &d);

	proto_bytes = p.SerializeAsString();
	data_bytes.resize(d.ByteSize());
	d.SerializeToArray(&data_bytes[0]);

	sum = Sum(p);
	if (sum != Sum(d)) {
		std::cerr << "[ERROR] Sum mismatch: " << sum << " != " << Sum(d) << endl;
	}

	std::cout << "ProtoSize: " << proto_bytes.length() << std::endl;
	std::cout << "DataSize: " << data_bytes.length() << std::endl;
	std::cout << p.DebugString() << std::endl;
}

static void BM_Empty(benchmark::State& state) {
	while (state.KeepRunning()) {
	}
}

BENCHMARK(BM_Empty);

static void BM_ParseDirect(benchmark::State& state) {
	Data object;
	size_t total = 0;
	while (state.KeepRunning()) {
		object.ParseFromArray(&data_bytes[0], data_bytes.length());
		if (Sum(object) != sum) {
			std::cerr << "[ERROR] wrong sum: " << Sum(object) << " != " << sum << endl;
			break;
		}
		total += data_bytes.length();
	}
	state.SetBytesProcessed(total);
}

BENCHMARK(BM_ParseDirect);

static void BM_ParseTable(benchmark::State& state) {
	Data object;
	size_t total = 0;
	while (state.KeepRunning()) {
		object.ParseFromTable(&data_bytes[0], data_bytes.length());
		if (Sum(object) != sum) {
			std::cerr << "[ERROR] wrong sum: " << Sum(object) << " != " << sum << endl;
			break;
		}
		total += data_bytes.length();
	}
	state.SetBytesProcessed(total);
}

BENCHMARK(BM_ParseTable);

static void BM_ParseProto(benchmark::State& state) {
	DataProto object;
	size_t total = 0;
	while (state.KeepRunning()) {
		object.ParseFromArray(&proto_bytes[0], proto_bytes.length());
		if (Sum(object) != sum) {
			std::cerr << "[ERROR] wrong sum: " << Sum(object) << " != " << sum << endl;
			break;
		}
		total += proto_bytes.length();
	}
	state.SetBytesProcessed(total);
}

BENCHMARK(BM_ParseProto);

static void BM_ParseProtoDirect(benchmark::State& state) {
	DataProto object;
	size_t total = 0;
	while (state.KeepRunning()) {
		ProtoFromArray(&proto_bytes[0], proto_bytes.length(), &object);
		if (Sum(object) != sum) {
			std::cerr << "[ERROR] wrong sum: " << Sum(object) << " != " << sum << endl;
			std::cerr << object.DebugString() << std::endl;
			break;
		}
		total += proto_bytes.length();
	}
	state.SetBytesProcessed(total);
}

BENCHMARK(BM_ParseProtoDirect);

static void BM_ParseProtoFromDirect(benchmark::State& state) {
	DataProto object;
	size_t total = 0;
	while (state.KeepRunning()) {
		ProtoFromDirect(&data_bytes[0], data_bytes.length(), &object);
		if (Sum(object) != sum) {
			std::cerr << "[ERROR] wrong sum: " << Sum(object) << " != " << sum << endl;
			std::cerr << object.DebugString() << std::endl;
			break;
		}
		total += data_bytes.length();
	}
	state.SetBytesProcessed(total);
}

BENCHMARK(BM_ParseProtoFromDirect);

int main(int argc, char* argv[]) {
	benchmark::Initialize(&argc, argv);

	Init();

	benchmark::RunSpecifiedBenchmarks();
	return 0;
}
