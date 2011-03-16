root = ../..

-include $(root)/configuration

SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o,$(SOURCE_FILES))

default: all

all: all-recursive

all-here: list_glx_fb_configurations

all-recursive: all-here

clean: clean-recursive

clean-here: 
	$(RM) $(OBJECT_FILES)
	$(RM) list_glx_fb_configurations

clean-recursive: clean-here

list_glx_fb_configurations: $(OBJECT_FILES) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) $(OBJECT_FILES) -lGL -lX11 -o $@

list_glx_fb_configurations.o: list_glx_fb_configurations.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECT_FILES): $(CXXFLAGS_configuration)

.PHONY: all all-here all-recursive clean clean-here clean-recursive default
