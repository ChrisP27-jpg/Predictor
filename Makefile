# Specifiy the target
all: predictors

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable
predictors: Predictors.o
	g++ Predictors.o -o predictors

# Specify how the object files should be created from source files
Predictors.o: Predictors.cpp
	g++ -c Predictors.cpp


# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f *.o predictors
