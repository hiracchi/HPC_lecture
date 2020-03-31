CC = gcc
CXX = gxx
CFLAGS = -Wall -g 
CXXFLAGS = -Wall -g

# for MacOSX
#BLASLIB = -framework vecLib
#LAPACKLIB = -framework vecLib
# for MacOSX(El Capitan)
BLASLIB = -framework Accelerate
LAPACKLIB = -framework Accelerate

OBJS = matrix.o

.SUFFIXES: .o .c .cpp .cxx

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.cxx.o:
	$(CXX) $(CXXFLAGS) -c $<


.PHONY: all extra clean
all:    sample show_matrix make_matrix make_unit check_matrix
extra:	mul_matrix inv_matrix

clean:
	$(RM) *.o

sample:  sample.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

show_matrix: show_matrix.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

make_matrix: make_matrix.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

make_unit: make_unit.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

check_matrix: check_matrix.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

mul_matrix: mul_matrix.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(BLASLIB)

inv_matrix: inv_matrix.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LAPACKLIB)

