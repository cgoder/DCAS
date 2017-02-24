
# **********************************************
TARGET_CPU=arm
CA_MODE = DVNCAS

# **********************************************

LIBPATH   = ./lib
LIBNAME  = libskyworthcas

ifeq ($(CA_MODE),TFCAS)
PATH_LIB_CAS = ./tfca
CAS_NAME = YF000-msd801-20130108
CAS_OBJ = $(PATH_LIB_CAS)/hello-jni.o
endif

ifeq ($(CA_MODE),DVNCAS)
PATH_LIB_CAS = ./dvnca
CAS_NAME = dvnca_v4036-licenselib-testing
CAS_OBJ = $(PATH_LIB_CAS)/arith.o
CAS_OBJ += $(PATH_LIB_CAS)/cak_api.o
CAS_OBJ += $(PATH_LIB_CAS)/crc32.o
CAS_OBJ += $(PATH_LIB_CAS)/crc64.o
CAS_OBJ += $(PATH_LIB_CAS)/ddd.o
CAS_OBJ += $(PATH_LIB_CAS)/dh.o
CAS_OBJ += $(PATH_LIB_CAS)/dvn_api.o
CAS_OBJ += $(PATH_LIB_CAS)/dvn_ecm.o
CAS_OBJ += $(PATH_LIB_CAS)/dvn_util.o
CAS_OBJ += $(PATH_LIB_CAS)/Kernel.o
CAS_OBJ += $(PATH_LIB_CAS)/ttt.o
endif

CROSS_TOOL_PREFIX = arm-none-linux-gnueabi-
CXX = $(CROSS_TOOL_PREFIX)g++
CC  = $(CROSS_TOOL_PREFIX)gcc
AR  = $(CROSS_TOOL_PREFIX)ar
LD  = $(CROSS_TOOL_PREFIX)ld
RANLIB  = $(CROSS_TOOL_PREFIX)ranlib

#CC_OPTS += -mfloat-abi=softfp -fPIC -shared -mlittle-endian -march=armv7-a -mfpu=neon -mcpu=cortex-a9 -Wall Werror -Wpointer-arith  -fno-strict-aliasing -fno-optimize-sibling-calls -g -O2
CC_OPTS += -D$(CA_MODE) -Wall -Werror -O2

ifeq ($(CA_MODE),TFCAS)
CC_OPTS += -DCASCD
endif
ifeq ($(CA_MODE),DVNCAS)
CC_OPTS += -DCASDVN
endif
# **********************************************

SRCDIR += ./common/src
ifeq ($(CA_MODE),TFCAS)
SRCDIR += ./tfca/src
endif
ifeq ($(CA_MODE),DVNCAS)
SRCDIR += ./dvnca/src
endif
SRC_C += $(foreach dir,$(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CPP += $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))

INCDIR += ./common/inc
ifeq ($(CA_MODE),TFCAS)
INCDIR += ./tfca/inc
endif
ifeq ($(CA_MODE),DVNCAS)
INCDIR += ./dvnca/inc
endif
INCLUDE = $(foreach dir,$(INCDIR), -I$(dir))

STATIC_OBJ_C0  = $(patsubst %.c, %.o, $(SRC_C))
STATIC_OBJ_C1  = $(patsubst %.cpp, %.o, $(SRC_CPP))

STATIC_OBJ_C  = $(foreach file, $(STATIC_OBJ_C0), $(file) )
STATIC_OBJ_CPP = $(foreach file, $(STATIC_OBJ_C1), $(file))


# **********************************************

compile: setup $(STATIC_OBJ_C) $(STATIC_OBJ_CPP) 

static_lib : compile ca_lib makeStatic

dynamic_lib : compile ca_lib makeDynamic

libs: dynamic_lib
all: dynamic_lib


$(STATIC_OBJ_C) : %.o : %.c 
#	@echo "begin compiling for c file   ........." 
	@$(CXX) $(INCLUDE) $(CC_OPTS) -c $< -o $@ 
#	@echo "make c file obj ok....."

$(STATIC_OBJ_CPP) : %.o : %.cpp
#	@echo "begin compiling for cpp file....."
	@$(CXX) $(INCLUDE) $(CC_OPTS) -c $< -o $@
#	@echo "make cpp file objs ok...."


makeStatic:$(CAS_OBJ) $(STATIC_OBJ_C) $(STATIC_OBJ_CPP)
	@$(AR) cru $(LIBPATH)/$(LIBNAME).a $(CAS_OBJ) $(STATIC_OBJ_C) $(STATIC_OBJ_CPP)
	@echo "make static lib ok ........."
makeDynamic:$(CAS_OBJ) $(STATIC_OBJ_C) $(STATIC_OBJ_CPP)
	@$(CXX) $(CAS_OBJ) $(STATIC_OBJ_C) $(STATIC_OBJ_CPP) -fPIC -shared -o $(LIBPATH)/$(LIBNAME).so
	@echo "make dynamic lib ok ......."
allobj:
	@$(AR) ru $(LIBPATH)/$(LIBNAME).o $(CAS_OBJ) $(STATIC_OBJ_C) $(STATIC_OBJ_CPP)	
setup :
	@mkdir -p $(LIBPATH)
#	@find . -regex '.*\.c\|.*\.cpp\|.*\.h'

	
ca_lib:
#	@echo $(PATH_LIB_CAS)
	@cd $(PATH_LIB_CAS);$(AR) x $(CAS_NAME).a
	
clean :
	@rm -f $(STATIC_OBJ_C)
	@rm -f $(STATIC_OBJ_CPP)
	@rm -f $(LIBPATH)/$(LIBNAME).a
	@rm -f $(LIBPATH)/$(LIBNAME).so
	@rm -rf $(LIBPATH)
	@rm $(CAS_OBJ)

test: $(LIBPATH)/$(LIBNAME).a
	@$(CXX) $(INCLUDE) -o sky.app ./test/main.c $(LIBPATH)/$(LIBNAME).a
	


