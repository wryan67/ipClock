# Build command line VoltageCatcher
#
# Use this command to place "vc" into /usr/local/bin 
# $ make clean && make && make install 
#
DIR_OBJ  := ../obj

CC       := g++
CFLAGS   := -c -O2 -std=gnu++11 
DFLAGS   := -c -O2 
LDFLAGS  := -pthread -lwiringPi -lm -lwiringPiDev
SOURCES  := main.cpp 

TOOLS    := stringUtil.cpp threads.cpp lcd.cpp
TOOLDIR  := tools/util

TDIR     := $(addprefix $(TOOLDIR)/,$(TOOLS))
TOBJ     := $(TOOLS:%.cpp=../obj/$(TOOLDIR)/%.o)

SRC      := $(foreach sdir,$(SRCDIR),$(wildcard $(sdir)/*.cpp))

OBJECTS  := $(SOURCES:%.cpp=../obj/%.o)
HEADERS  := $(SOURCES:%.cpp=%.h)

EXECUTABLE=../bin/ipClock

INCLUDES := tools/include

all:   debug $(TDIR) $(HEADERS) $(SOURCES) $(EXECUTABLE)

debug:
	@echo ":::::::: all ::::::::::"
	@echo SOURCES=$(SOURCES)
	@echo TOOLS=$(TOOLS)
	@echo -      
	@echo OBJECTS=$(OBJECTS)
	@echo TOBJ=$(TOBJ)
	@echo -      
	@echo TOOLDIR=$(TOOLDIR)
	@echo TDIR=$(TDIR)
	@echo HEADERS=$(HEADERS)
	@echo SRC=$(SRC)
	@echo OBJ=$(OBJ)
	@echo OBJ_C=$(OBJ_C)
	@echo OBJ_O=$(OBJ_O)
	@echo ":::::::: all ::::::::::"

tasks: all

$(EXECUTABLE): $(OBJECTS) $(TOBJ) $(OBJ_O)
	@echo linking...
	@mkdir -p ../bin
	@$(CC) $(OBJECTS) $(TOBJ) $(LDFLAGS) -o $@

../obj/$(TOOLDIR)/%.o: 
	@echo compiling tool $(*D)/$(TOOLDIR)/$(*F).cpp
	@echo "$< -> $@"
	@mkdir -p ../obj/tools/util
	$(CC) $(CFLAGS) $(*D)/$(TOOLDIR)/$(*F).cpp -o $@  

../obj/%.o: %.h %.cpp
	@echo compiling module $(*D)/$(*F).cpp
	@echo "$< -> $@"
	@mkdir -p ../obj
	$(CC) $(CFLAGS) $(*D)/$(*F).cpp -o $@ 


${DIR_BIN}/%.o : $(DIR_GUI)/%.c
	gcc $(DFLAGS) -c  $< -o $(DIR_OBJ)/$@ $(LIB) -I $(DIR_Config)  -I $(DIR_LCD)  -I $(DIR_Fonts)

${DIR_BIN}/%.o : $(DIR_LCD)/%.c
	gcc $(DFLAGS) -c  $< -o $(DIR_OBJ)/$@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_Fonts)/%.c
	gcc $(DFLAGS) -c  $< -o $(DIR_OBJ)/$@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_Config)/%.c
	gcc $(DFLAGS) -c  $< -o $(DIR_OBJ)/$@ $(LIB)




install: all
	@echo Installing $(EXECUTABLE) to /usr/local/bin
	@sudo cp $(EXECUTABLE) /usr/local/bin

clean:
	@rm -rf ../obj/ $(EXECUTABLE)

