all: a.out

a.out: a.cc data.pb.cc data.cc msg.cc
	g++ $(CXXFLAGS) -Wno-invalid-offsetof a.cc data.pb.cc data.cc msg.cc -o a.out -lbenchmark -lprotobuf

data.pb.cc: data.proto Makefile
	protoc --cpp_out=. data.proto

data.proto: autogen Makefile

data.cc: autogen

autogen: gen Makefile
	./gen $(DN)
	touch autogen

gen: gen.cc
	g++ $(CXXFLAGS) gen.cc -o gen

clean:
	rm gen data.proto data.pb.* data.h data.cc a.out autogen

run: a.out
	./a.out --color_print=false
