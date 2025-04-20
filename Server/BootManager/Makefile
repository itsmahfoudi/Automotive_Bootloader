.PHONY: all build cmake format flash-stlink clean clean-all
############################### Native Makefile ###############################

PROJECT_NAME ?= BootManager 
BUILD_DIR ?= build
BUILD_TYPE ?= Debug
PLATFORM = $(if $(OS),$(OS),$(shell uname -s))

#ifeq ($(PLATFORM),Windows_NT)
#    BUILD_SYSTEM ?= MinGW Makefiles
#else
#    ifeq ($(PLATFORM),Linux)
#        BUILD_SYSTEM ?= Unix Makefiles
#    else
#        @echo "Unsuported platform"
#        exit 1
#    endif
#endif

BUILD_SYSTEM ?= Unix Makefiles

all: build

build: cmake
	$(MAKE) -C $(BUILD_DIR) --no-print-directory

cmake: $(BUILD_DIR)/Makefile

$(BUILD_DIR)/Makefile:
	cmake \
		-G "$(BUILD_SYSTEM)" \
		-B$(BUILD_DIR) \
		-DPROJECT_NAME=$(PROJECT_NAME) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DDUMP_ASM=OFF

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
format: $(addsuffix .format,$(SRCS))
%.format: %
	clang-format -i $<

## Device specific!
DEVICE = STM32H755

flash-st: build
	st-flash --reset write $(BUILD_DIR)/$(PROJECT_NAME)-cm7.bin 0x08000000

#$(BUILD_DIR)/jlink-script:
#	touch $@
#	@echo device $(DEVICE) > $@
#	@echo si 1 >> $@
#	@echo speed 4000 >> $@
#	@echo loadfile $(FIRMWARE),0x08000000 >> $@
#	@echo -e "r\ng\nqc" >> $@
#
#flash-jlink: build | $(BUILD_DIR)/jlink-script
#	JLinkExe -commanderScript $(BUILD_DIR)/jlink-script

clean:
	rm -rf $(BUILD_DIR)
	rm -rf cmake-build-debug
