# compile
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
TARGET = test_main

# inc files
INC_DIRS = ../lib/inc
INC_DIRS += unity
INCLUDES = $(addprefix -I, $(INC_DIRS))
CFLAGS += $(INCLUDES)

# src files
SRCS = test_main.c
SRCS += lib/test_adc_filter.c
SRCS += ../lib/src/adc_filter.c
SRCS += unity/unity.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)