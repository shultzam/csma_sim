OBJS = ./*.o
INCLUDES = ./*.h
CXXFILES = ./*.cpp
CXXFLAG = -Wall -g
CXXC = g++
EXECUTABLE = csma_sim

.PHONY: help
help:
	@echo "Valid commands:"
	@echo "    make help     -- display help message"
	@echo "    make clean    -- clean object files and binary"
	@echo "    make csma_sim -- build the MAC simulation"

.PHONY: all
all:
	help

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXECUTABLE)

$(EXECUTABLE):$(CXXFILES) $(INCLUDES)
	$(CXXC) $(CXXFLAGS) $(INCLUDES) -o $@ $^
