CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lcurl
TARGET = pv
SOURCES = src/main.cpp src/skill_fetcher.cpp src/level_calculator.cpp src/progress_bar.cpp src/config.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Load .env if it exists
ifneq (,$(wildcard .env))
    include .env
    export
endif

# command-line > environment variable > .env file
API_KEY ?= $(HYPIXEL_API_KEY)

	# if no key is found, fail 
ifeq ($(API_KEY),)
$(error No API key found. Set HYPIXEL_API_KEY in .env, export it, or pass make API_KEY="your_key")
endif

CXXFLAGS += -DHYPIXEL_API_KEY=\"$(API_KEY)\"

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clear:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clear
