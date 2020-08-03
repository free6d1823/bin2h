PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TARGET_NAME = bin2h
OBJS =   main.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else 
	CFLAGS += -O2
endif

INCLUDE_PATH = -I./
CFLAGS += $(INCLUDE_PATH)

LIB_PATH = 
LIB_LINK = 
LFLAGS += $(LIB_PATH) $(LIB_LINK)  
 
all:	$(TARGET_NAME)

$(TARGET_NAME):	$(OBJS) 
	$(CXX) $^ -o $@ $(LFLAGS)	

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr $(TARGET_NAME) $(OBJS) 
