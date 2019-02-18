src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))

all: directories rpg

rpg : $(obj)
	clang++ -o rpg $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

.PHONY: directories

directories: obj/

obj/:
	mkdir -p $@

clean :
	rm -rf obj/
	rm rpg
