src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))

all: directories rpg

rpg : $(obj)
	clang++ -o rpg $^ -L../character/lib -L../creature/lib -L../directory/lib -lcharacter -lcreature -ldirectory -lncurses

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -I../character/include -I../creature/include -I../directory/include -o $@ -c $<

.PHONY: directories

directories: obj/

obj/:
	mkdir -p $@

clean :
	rm -rf obj/
	rm rpg