src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))
dirs = obj/ lib/

all: directories liblocation

liblocation : $(obj)
	ar -r lib/liblocation.a $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -I../directory/include -I../creature/include -I../cpptoml/include -o $@ -c $<

.PHONY: directories

directories: $(dirs)

obj/:
	mkdir -p $@

lib/:
	mkdir -p $@

clean : $(dirs)
	rm -rf $^
