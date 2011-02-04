default: bake

bake:
	g++ main.cpp -lsfml-graphics -lsfml-window -olife 

run:
	./life

go: bake run
