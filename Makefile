SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o, $(SOURCE_FILES))
DEPENDENCY_FILES = $(patsubst %.cpp,.dependencies/%.dep, $(SOURCE_FILES))
TARGET_BINARY = galactic-fall

all: all-recursive

all-recursive: binary data

clean: clean-recursive
	$(RM) -r .dependencies
	$(RM) $(TARGET_BINARY) $(OBJECT_FILES)

clean-recursive:
	@$(MAKE) -C data clean

binary: check-dependencies galactic-fall

data:
	@$(MAKE) -C data all

check-dependencies:
	@if [ x`which pkg-config` = x ]; then \
	    echo -e "\nYour system is missing the \"pkg-config\" tool.\nThis tool is needed and you will need to install it in order to build the program.\n"; false; \
	fi
	@if ! pkg-config --exists libarxx; then \
	    echo -e "\nYour system is missing the \"libarxx\" library.\nYou can get this library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.7.8 libarxx; then \
	    echo -e "\nThe library \"libarxx\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion libarxx`\nYou need: Version 0.7.8\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi

.dependencies:
	@mkdir $@

.dependencies/%.dep: %.cpp .dependencies
	@$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -M "$<" -MT "$@ $(patsubst %.cpp,%.o,$<)" -MF "$@"

install:
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

$(TARGET_BINARY): $(OBJECT_FILES)
	$(CXX) $(LDFLAGS) `pkg-config --libs libarxx` $^ -lGL -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

.PHONY: data

-include $(DEPENDENCY_FILES)
