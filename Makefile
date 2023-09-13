# Compiler and Linker
CXX         := g++

# The Target Binary Program
TARGET      := my_api_project

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := include
BUILDDIR    := build
TARGETDIR   := $(BUILDDIR)

# Flags, Libraries and Includes
CXXFLAGS    := -std=c++11 -Wall -I$(INCDIR)

# Source and Object Files
SOURCES     := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS     := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Targets

$(TARGETDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo " $(CXX) $^ -o $@";
	@$(CXX) $^ -o $@;

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<...";
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGETDIR)/$(TARGET)";
	@$(RM) -r $(BUILDDIR) $(TARGETDIR)/$(TARGET)

.PHONY: clean
