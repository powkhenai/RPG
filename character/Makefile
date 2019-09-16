src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))
dirs = obj/ lib/

all: directories libcharacter

libcharacter : $(obj)
	ar -r lib/libcharacter.a $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -I../creature/include -o $@ -c $<

.PHONY: directories

directories: $(dirs)

obj/:
	mkdir -p $@

lib/:
	mkdir -p $@

clean : $(dirs)
	rm -rf $^
