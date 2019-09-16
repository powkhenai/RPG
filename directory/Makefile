src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))

all: directories libdirectory

libdirectory : $(obj)
	ar -r lib/libdirectory.a $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

.PHONY: directories

directories: obj/ lib/

obj/:
	mkdir -p $@

lib/:
	mkdir -p $@

clean :
	rm -rf directories
