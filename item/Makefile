src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))
dirs = obj/ lib/

all: directories libitem

libitem : $(obj)
	ar -r lib/libitem.a $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

.PHONY: directories

directories: $(dirs)

obj/:
	mkdir -p $@

lib/:
	mkdir -p $@

clean : $(dirs)
	rm -rf $^
