CXX=g++
CP=cp
CXXFLAGS= -fPIC -shared -c -Wall -g -std=c++0x
INCLUDE = -Iinclude -I/usr/include/python2.7/
LDFLAGS= -lboost_python -ltins -lpython2.7 -shared
RM=rm
SOURCES= $(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=tins_core.so
DEPS = $(SOURCES:.cpp=.d)

all: $(SOURCES) $(EXECUTABLE)

compile: $(OBJECTS)

recompile: clean all

depends: $(SOURCES)
	rm -f ./depends.d
	make do_make_deps

do_make_deps: $(DEPS)

install:
	$(CP) $(EXECUTABLE) /usr/bin/
uninstall:
	$(RM) /usr/bin/$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@

%.d : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MG -MM -MP -MT"$(<:.cpp=.o)" $< >> depends.d

clean:
	rm $(OBJECTS) $(EXECUTABLE)

-include depends.d
