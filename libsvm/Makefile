CXX ?= g++
CFLAGS = -Wall -Wconversion -O3 -fPIC -g
SHVER = 2
OS = $(shell uname)
LIBS = -lGL -lglut -lGLU -lm

all: svm-train svm-predict svm-scale

lib: svm.o
	if [ "$(OS)" = "Darwin" ]; then \
		SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,libsvm.so.$(SHVER)"; \
	else \
		SHARED_LIB_FLAG="-shared -Wl,-soname,libsvm.so.$(SHVER)"; \
	fi; \
	$(CXX) $${SHARED_LIB_FLAG} svm.o -o libsvm.so.$(SHVER)

svm-predict: svm-predict.c svm.o
	$(CXX) $(CFLAGS) svm-predict.c svm.o -o svm-predict $(LIBS)
svm-train: svm-train.c svm.o
	$(CXX) $(CFLAGS) svm-train.c svm.o -o svm-train $(LIBS)
svm-scale: svm-scale.c
	$(CXX) $(CFLAGS) svm-scale.c -o svm-scale $(LIBS)
svm.o:
	$(CXX) $(CFLAGS) -c svm.cpp $(LIBS)
clean:
	rm -f *~ svm.o svm-train svm-predict svm-scale libsvm.so.$(SHVER)
