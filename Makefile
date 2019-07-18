SILENCE := @
BUILD_DIR := ${CURDIR}/build
CFLAGS += -g -Wall -Werror -Wstrict-prototypes -Wextra -fPIC
LDFLAGS += -ljson-c

get_src_from_dir = $(wildcard $1/*.cpp) $(wildcard $1/*.cc) $(wildcard $1/*.c)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
__src_to = $(subst .c,$1, $(subst .cc,$1, $(subst .cpp,$1,$(notdir $2))))
src_to = $(addprefix $(BUILD_DIR)/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1)
add_inc_dir = $(addprefix -I,$1)

SRCS := $(filter-out $(wildcard ${CURDIR}/*_test.c), $(call get_src_from_dir_list, ${CURDIR}))
OBJS := $(call src_to_o, $(SRCS))

all: blob_test blobmsg_test

blob_test: $(BUILD_DIR)/blob_test.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/$@

blobmsg_test: $(BUILD_DIR)/blobmsg_test.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/$@

$(BUILD_DIR)/%.o: %.cc
	$(SILENCE)echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: %.cpp
	$(SILENCE)echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: %.c
	$(SILENCE)echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean
