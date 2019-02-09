rpg : obj/rpg.o obj/character.o
	clang++ -o rpg obj/rpg.o obj/character.o

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

clean :
	rm rpg
	rm obj/*
