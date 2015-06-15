####### Compiler, tools and options
CXX           = g++-5
DEFINES       = -std=c++14 -Wall -Wextra -fmax-errors=2 -pg
CFLAGS        = $(DEFINES)
CXXFLAGS      = $(DEFINES)
INCPATH       = -Iinclude
LINK          = $(CXX)
LFLAGS        =
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
MAKE_DIR      = mkdir

####### Output directory
OBJECTS_DIR   = ./obj
BIN_DIR       = ./bin
DOC_DIR       = ./doc

####### Files
OBJECTS       = normal_example.o gaussian_test.o
TARGET        = normal_example gaussian_test

first: all
####### Implicit rules

all: $(TARGET)

$(TARGET):

makedir:
	test -z $(OBJECTS_DIR) || $(MAKE_DIR) -p $(OBJECTS_DIR)
	test -z $(BIN_DIR) || $(MAKE_DIR) -p $(BIN_DIR)

clean:
	-$(DEL_FILE) $(OBJECTS_DIR)/*.o
	-$(DEL_FILE) $(BIN_DIR)/*
	-$(DEL_FILE) *~ core *.core

distclean: clean
	-$(DEL_FILE) $(BIN_DIR)/*

####### Compile

normal_example.o : test/normal_example.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/normal_example.o test/normal_example.cc
normal_example: normal_example.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/normal_example $(OBJECTS_DIR)/normal_example.o $(OBJCOMP) $(LIBS)

gaussian_test.o : test/gaussian_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/gaussian_test.o test/gaussian_test.cc
gaussian_test: gaussian_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/gaussian_test $(OBJECTS_DIR)/gaussian_test.o $(OBJCOMP) $(LIBS)

uniform_test.o : test/uniform_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/uniform_test.o test/uniform_test.cc
uniform_test: uniform_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/uniform_test $(OBJECTS_DIR)/uniform_test.o $(OBJCOMP) $(LIBS)

normal_test.o : test/normal_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/normal_test.o test/normal_test.cc
normal_test: normal_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/normal_test $(OBJECTS_DIR)/normal_test.o $(OBJCOMP) $(LIBS)

gamma_test.o : test/gamma_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/gamma_test.o test/gamma_test.cc
gamma_test: gamma_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/gamma_test $(OBJECTS_DIR)/gamma_test.o $(OBJCOMP) $(LIBS)

beta_test.o : test/beta_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/beta_test.o test/beta_test.cc
beta_test: beta_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/beta_test $(OBJECTS_DIR)/beta_test.o $(OBJCOMP) $(LIBS)

bernoulli_test.o : test/bernoulli_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/bernoulli_test.o test/bernoulli_test.cc
bernoulli_test: bernoulli_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/bernoulli_test $(OBJECTS_DIR)/bernoulli_test.o $(OBJCOMP) $(LIBS)

binomial_test.o : test/binomial_test.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/binomial_test.o test/binomial_test.cc
binomial_test: binomial_test.o
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/binomial_test $(OBJECTS_DIR)/binomial_test.o $(OBJCOMP) $(LIBS)

