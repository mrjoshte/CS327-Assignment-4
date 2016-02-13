all: sndinfo sndcat sndmix sndgen

sndinfo: sndinfo_main.cpp ComS229Util.o
	g++ -Wall -o sndinfo sndinfo_main.cpp ComS229Util.o

sndcat: sndcat_main.cpp ComS229Util.o sndcatUtil.o
	g++ -Wall -o sndcat sndcat_main.cpp ComS229Util.o sndcatUtil.o

sndmix: sndmix_main.cpp ComS229Util.o sndmixUtil.o sndcatUtil.o
	g++ -Wall -o sndmix sndmix_main.cpp sndmixUtil.o sndcatUtil.o ComS229Util.o

sndgen: sndgen_main.cpp ComS229Util.o sndgen.o
	g++ -Wall -o sndgen sndgen_main.cpp sndgen.o ComS229Util.o



sndgen.o: sndgen.cpp sndgen.h ComS229Util.o
	g++ -Wall -c sndgen.cpp ComS229Util.o

sndmixUtil.o: sndmixUtil.cpp ComS229Util.o sndmixUtil.h
	g++ -Wall -c sndmixUtil.cpp ComS229Util.o

sndcatUtil.o: sndcatUtil.cpp ComS229Util.o sndcatUtil.h
	g++ -Wall -c sndcatUtil.cpp ComS229Util.o

ComS229Util.o: ComS229Util.cpp ComS229Util.h
	g++ -Wall -c ComS229Util.cpp

clean:
	rm -f *.o sndinfo sndcat sndmix sndgen