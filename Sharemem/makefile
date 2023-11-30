all:client server

client:processb.cc
	g++ -o $@ $^ -g -std=c++11
server:processa.cc
	g++ -o $@ $^ -g -std=c++11
clean:
	rm -f client server
.PHONY:clean