src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))

all: directories deps rpg

deps:
	$(MAKE) -C ../directory
	$(MAKE) -C ../creature
	$(MAKE) -C ../character
	$(MAKE) -C ../location
	$(MAKE) -C ../item

rpg : $(obj)
	clang++ -o rpg $^ -L../character/lib -L../creature/lib -L../directory/lib -L../location/lib -lcharacter -lcreature -ldirectory -llocation -lncurses

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -I../character/include -I../creature/include -I../directory/include -I../location/include -o $@ -c $<

.PHONY: directories

directories: obj/

obj/:
	mkdir -p $@

clean :
	rm -rf obj/
	rm rpg
