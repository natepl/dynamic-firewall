CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: enforcer_daemon aggregator_svc analysis_engine

enforcer_daemon: enforcer/src/main.cpp enforcer/src/pf_manager.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -Ienforcer/src $^ -o bin/enforcer

aggregator_svc: aggregator/src/main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -Iaggregator/src $< -o bin/aggregator

analysis_engine: analysis/src/main.cpp analysis/src/mlfq.cpp analysis/src/packet_parser.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -Ianalysis/src $^ -lpcap -lpthread -o bin/analysis

clean:
	rm -rf bin
