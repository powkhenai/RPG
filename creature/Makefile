src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))
dirs = obj/ lib/

all: directories libcreature

libcreature : $(obj)
	ar -r lib/libcreature.a $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -I../directory/include -o $@ -c $<

.PHONY: directories

directories: $(dirs)

obj/:
	mkdir -p $@

lib/:
	mkdir -p $@

clean : $(dirs)
	rm -rf $^
