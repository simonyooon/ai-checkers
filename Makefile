checker: main.o checker.o agent.o
	g++ -std=gnu++11 -o checker main.o checker.o agent.o

main.o: main.cpp Game.h checker.h
	g++ -std=gnu++11 -c main.cpp

checker.o: checker.cpp checker.h
	g++ -std=gnu++11 -c checker.cpp

agent.o: agent.cpp agent.h checker.h
	g++ -std=gnu++11 -c agent.cpp

debug:
	g++ -std=gnu++11 -g -o checker main.cpp Game.h checker.h agent.h

clean:
	rm -f *.exe *.out *.o *.stackdump *~
	rm -f checker

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups

