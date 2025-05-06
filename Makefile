CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17

BUILD_DIR = $(CURDIR)/build

SOURCES = $(subst \,/,$(shell for /R %%f in (*.cpp) do @echo %%f))
OBJECTS = $(patsubst %.cpp, %.o,$(subst $(CURDIR),$(BUILD_DIR),$(SOURCES)))
TARGET = $(BUILD_DIR)/compiled.exe

$(BUILD_DIR)/%.o: %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

crun: $(TARGET)
	$(TARGET)

run: $(TARGET)
	$(TARGET)
