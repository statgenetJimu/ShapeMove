TARGET = spinxform
CFLAGS  = -Wall -Werror -pedantic -ansi -O3 -Iinclude
LDFLAGS = -Wall -Werror -pedantic -ansi -O3
LIBS =
OBJS = EigenSolver.o Image.o LinearSolver.o Mesh.o Quaternion.o QuaternionMatrix.o Vector.o main.o


all: $(TARGET)

test:
	@echo "testing example 'bumpy'...";
	./spinxform examples/bumpy/sphere.obj examples/bumpy/bumpy.tga examples/bumpy/solution.obj
	@echo ""
	-diff examples/bumpy/solution.obj examples/bumpy/reference_solution.obj
	@echo ""
	@echo "testing example 'spacemonkey'...";
	./spinxform examples/spacemonkey/capsule.obj examples/spacemonkey/spacemonkey.tga examples/spacemonkey/solution.obj
	@echo ""
	-diff examples/spacemonkey/solution.obj examples/spacemonkey/reference_solution.obj

$(TARGET): $(OBJS)
	g++ $(OBJS) $(LDFLAGS) $(LIBS) -o $(TARGET)

EigenSolver.o: src/EigenSolver.cpp include/EigenSolver.h include/QuaternionMatrix.h include/Quaternion.h include/Vector.h include/pcg/sparse_matrix.h include/pcg/util.h include/LinearSolver.h
	g++ $(CFLAGS) -c src/EigenSolver.cpp

Image.o: src/Image.cpp include/Image.h
	g++ $(CFLAGS) -c src/Image.cpp

LinearSolver.o: src/LinearSolver.cpp include/LinearSolver.h include/QuaternionMatrix.h include/Quaternion.h include/Vector.h include/pcg/sparse_matrix.h include/pcg/util.h include/pcg/pcg_solver.h include/pcg/sparse_matrix.h
	g++ $(CFLAGS) -c src/LinearSolver.cpp

Mesh.o: src/Mesh.cpp include/Mesh.h include/Quaternion.h include/Vector.h include/QuaternionMatrix.h include/pcg/sparse_matrix.h include/pcg/util.h include/Image.h include/LinearSolver.h include/EigenSolver.h include/Utility.h
	g++ $(CFLAGS) -c src/Mesh.cpp

Quaternion.o: src/Quaternion.cpp include/Quaternion.h include/Vector.h
	g++ $(CFLAGS) -c src/Quaternion.cpp

QuaternionMatrix.o: src/QuaternionMatrix.cpp include/QuaternionMatrix.h include/Quaternion.h include/Vector.h include/pcg/sparse_matrix.h include/pcg/util.h
	g++ $(CFLAGS) -c src/QuaternionMatrix.cpp

Vector.o: src/Vector.cpp include/Vector.h
	g++ $(CFLAGS) -c src/Vector.cpp

main.o: src/main.cpp
	g++ $(CFLAGS) -c src/main.cpp
	

clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f examples/bumpy/solution.obj
	rm -f examples/spacemonkey/solution.obj

