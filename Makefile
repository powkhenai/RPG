rpg : obj/rpg.o obj/player.o
	clang++ -o rpg obj/rpg.o obj/player.o

obj/%.o : src/%.cpp
	clang++ -std=c++17 -Iinclude -o $@ -c $<

clean :
	rm rpg
	rm obj/*
