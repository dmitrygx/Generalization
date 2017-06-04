CFLAGS += -g -ggdb -std=c++11

LDFLAGS +=

CXXFLAGS += -Wall -O3 -fopenmp

CPPFLAGS +=

PROGNAME:=generalization

SOURCES =				\
	GeneralizationCurve.cpp		\
	GeneralizationRequestCurve.cpp	\
	GeneralizationBenchmark.cpp	\
	GeneralizationLogging.cpp	\
	main.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)

# Which Boost modules to use
BOOST_MODULES = 	\
	timer		\
	system
BOOST_MODULES_TYPE := -mt
BOOST_MODULES_LIBS := $(addsuffix $(BOOT_MODULES_TYPE),$(BOOST_MODULES))
BOOST_LDFLAGS := $(addprefix -lboost_,$(BOOST_MODULES_LIBS))

MKL_LDFLAGS := -L/usr/intel/common/pkgs/mkl/11.0/mkl/lib/ -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

CPPFLAGS += $(BOOST_CPPFLAGS) 
LDFLAGS += $(BOOST_LDFLAGS) $(MKL_LDFLAGS)


all : $(PROGNAME)

$(PROGNAME) : $(OBJECTS) Makefile
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

%.o: %.cpp $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

Generalization: main.o GeneralizationBenchmark.o GeneralizationRequestCurve.o GeneralizationCurve.o

GeneralizationCurve.o: GeneralizationCurve.cpp GeneralizationCurve.h
	g++ -O3 -fopenmp -I/usr/intel/common/pkgs/mkl/11.0/mkl/include/ -std=c++11 -c GeneralizationCurve.cpp

GeneralizationRequestCurve.o: GeneralizationRequestCurve.cpp GeneralizationRequestCurve.h
	g++ -O3 -fopenmp  -I/usr/intel/common/pkgs/mkl/11.0/mkl/include/ -std=c++11 -c GeneralizationRequestCurve.cpp

GeneralizationBenchmark.o: GeneralizationBenchmark.cpp GeneralizationBenchmark.h
	g++ -O3 -fopenmp -I/usr/intel/common/pkgs/mkl/11.0/mkl/include/ -std=c++11 -c GeneralizationBenchmark.cpp

main.o: main.cpp
	g++ -O3 -fopenmp -I/usr/intel/common/pkgs/mkl/11.0/mkl/include/ -std=c++11 -c main.cpp

clean:
	rm -rf *.o fi
