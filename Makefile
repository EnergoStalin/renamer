INCLUDE=-Iinclude

all:
	echo "No targets specified try type windows or linux build target."
windows:
	g++ $(INCLUDE) -std=c++11 main.cpp -o renamer.exe
linux:
	g++ $(INCLUDE) -std=c++11 main.cpp -o renamer
rengen_linux:
	g++ -std=c++11 src/rengen.cpp -o rengen
clear:
	rm renamer rengen
