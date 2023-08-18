g++ -IC:/SFML-2.5.1/include -c Main.cpp -o Main.o -lpthread
g++ -LC:/SFML-2.5.1/lib ./main.o -o mandel -lpthread -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

./mandel > output.txt

rm -f *.o
rm mandel