SRC_DIR = src/
OBJ_DIR = src/
PLL_DIR = /usr/local/include/pll

CLIBRARIES = -lm -lpthread -lpll-sse3 # -fopenmp
CPP = g++
CXXFLAGS = -O3 -I$(PLL_DIR) -D_PLL -DHAVE_CONFIG_H -D_GNU_SOURCE -g \
   -Waddress -Warray-bounds -Wc++11-compat -Wchar-subscripts -Wenum-compare -Wcomment -Wformat -Wmain -Wmaybe-uninitialized -Wmissing-braces -Wnonnull -Wparentheses -Wreturn-type -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -DPTHREADS  #-fopenmp 


PARTEST_CXXFILES = \
	ConfigParser.cc \
	test-modsel.cc
	
GLOBAL_DEPS = pll.h globalVariables.h

PARTEST_OBJ = $(patsubst %.cc, $(OBJ_DIR)%.o, $(PARTEST_CXXFILES))

all: partest 
	$(CPP) $(CXXFLAGS) \
	$(PARTEST_OBJ) -o test-modsel $(CLIBRARIES)


clean:
	-rm $(PARTEST_OBJ) 
 
partest: $(PARTEST_OBJ)
	@echo "==> Finished compiling PARTEST files"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CPP) $(CXXFLAGS) -c $+ -I$(SRC_DIR) -o $@


.PHONY: partest
