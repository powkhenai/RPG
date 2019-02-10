src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp,obj/%.o,$(src))

rpg : $(obj)
	clang++ -o rpg $^

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

clean :
	rm obj/*
	rm rpg
