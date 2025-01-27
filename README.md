# Platformer
Prototype platformer game
# Compile
Windows: gcc -o game.exe src/*.c -I./include -L./lib -lglfw3dll -lm -lopengl32

Linux: gcc -o game src/*.c -I./include -L./lib -lGL -lglfw -lm -ldl -lpthread