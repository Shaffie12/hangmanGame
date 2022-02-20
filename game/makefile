hangman: Main.o FileReader.o GameInstance.o Gamer.o Manager.o Person.o Readwords.o
	g++ Main.o FileReader.o GameInstance.o Gamer.o Manager.o Person.o Readwords.o -o hangman
Main.o: Main.cpp
	g++ -c Main.cpp
FileReader.o: FileReader.cpp
	g++ -c FileReader.cpp
GameInstance.o: GameInstance.cpp
	g++ -c GameInstance.cpp
Gamer.o: Gamer.cpp
	g++ -c Gamer.cpp
Manager.o: Manager.cpp
	g++ -c Manager.cpp
Person.o: Person.cpp
	g++ -c Person.cpp
Readwords.o: Readwords.cpp
	g++ -c Readwords.cpp

clean:
	del *.o hangman