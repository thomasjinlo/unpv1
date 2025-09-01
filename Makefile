# ===================== Toolchain & flags =====================
CC       := gcc
AR       := ar
ARFLAGS  := rcs                   # r=replace/add, c=create, s=write index

CFLAGS   := -std=c99 -O2 -Wall -Wextra -Wpedantic -I.
LDFLAGS  :=
DEPFLAGS := -MMD -MP              # auto header deps for .o files

# ===================== Project layout ========================
BUILD_DIR    := build
COMMON_DIR  := common
INTRO_DIR    := intro
SANDBOX_DIR  := sandbox

# All common sources -> objects under build/common/
COMMON_SRCS := $(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJS := $(patsubst $(COMMON_DIR)/%.c,$(BUILD_DIR)/$(COMMON_DIR)/%.o,$(COMMON_SRCS))
COMMON_DEPS := $(COMMON_OBJS:.o=.d)

# All intro sources -> objects and final bins under build/intro/
INTRO_SRCS   := $(wildcard $(INTRO_DIR)/*.c)
INTRO_OBJS   := $(patsubst $(INTRO_DIR)/%.c,$(BUILD_DIR)/$(INTRO_DIR)/%.o,$(INTRO_SRCS))
INTRO_BINS   := $(patsubst $(BUILD_DIR)/$(INTRO_DIR)/%.o,$(BUILD_DIR)/$(INTRO_DIR)/%,$(INTRO_OBJS))
INTRO_DEPS   := $(INTRO_OBJS:.o=.d)

# Sandbox
SANDBOX_SRCS   := $(wildcard $(SANDBOX_DIR)/*.c)
SANDBOX_OBJS   := $(patsubst $(SANDBOX_DIR)/%.c,$(BUILD_DIR)/$(SANDBOX_DIR)/%.o,$(SANDBOX_SRCS))
SANDBOX_BINS   := $(patsubst $(BUILD_DIR)/$(SANDBOX_DIR)/%.o,$(BUILD_DIR)/$(SANDBOX_DIR)/%,$(SANDBOX_OBJS))
SANDBOX_DEPS   := $(SANDBOX_OBJS:.o=.d)

LIBCOMMON   := $(BUILD_DIR)/libcommon.a

# Default target: build every intro program
all: $(INTRO_BINS) $(SANDBOX_BINS)

# ========== Build libcommon.a from all common objects ==========
$(LIBCOMMON): $(COMMON_OBJS) | $(BUILD_DIR)
	@echo "AR      $@"
	@$(AR) $(ARFLAGS) $@ $^

# ========== Compile rules (.c -> .o with header deps) ===========
$(BUILD_DIR)/$(COMMON_DIR)/%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR)/$(COMMON_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/$(INTRO_DIR)/%.o: $(INTRO_DIR)/%.c | $(BUILD_DIR)/$(INTRO_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SANDBOX_DIR)/%.o: $(SANDBOX_DIR)/%.c | $(BUILD_DIR)/$(SANDBOX_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# ========== Link each intro object with the common lib =========
$(BUILD_DIR)/$(INTRO_DIR)/%: $(BUILD_DIR)/$(INTRO_DIR)/%.o $(LIBCOMMON) | $(BUILD_DIR)/$(INTRO_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ========== Link each intro object with the common lib =========
$(BUILD_DIR)/$(SANDBOX_DIR)/%: $(BUILD_DIR)/$(SANDBOX_DIR)/%.o $(LIBCOMMON) | $(BUILD_DIR)/$(SANDBOX_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ========== Make sure build directories exist ===================
$(BUILD_DIR) \
$(BUILD_DIR)/$(COMMON_DIR) \
$(BUILD_DIR)/$(SANDBOX_DIR) \
$(BUILD_DIR)/$(INTRO_DIR):
	@mkdir -p $@

# ========== Maintenance ========================================
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

# Include auto-generated header dependency files (safe if missing)
-include $(COMMON_DEPS) $(INTRO_DEPS)
