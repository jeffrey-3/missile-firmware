CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs \
           -u _printf_float -lm -lc -lgcc -Wl,--gc-sections
SOURCES = src/startup.c \
          src/syscalls.c \
          src/hal/clock.c \
          src/hal/gpio.c \
          src/hal/uart.c \
          src/hal/spi.c \
          src/hal/pwm.c \
          src/peripherals/icm45686.c \
          src/peripherals/w25q128jv.c \
          src/math/quaternion.c \
          src/math/math.c \
          src/math/matrix.c \
          src/util/ring_buffer.c \
          src/board.c \
          src/estimator.c \
          src/logger.c \
          src/control.c \
          src/indicator.c \
          src/vehicle.c
SOURCES_SIM = src/startup.c \
              src/syscalls.c \
              src/hal/clock.c \
              src/hal/gpio.c \
              src/hal/uart.c \
              src/hal/spi.c \
              src/peripherals/w25q128jv.c \
              src/math/quaternion.c \
              src/math/math.c \
              src/math/matrix.c \
              src/util/ring_buffer.c \
              src/board.c \
              src/estimator.c \
              src/logger.c \
              src/control.c \
              src/indicator.c \
              src/vehicle.c \
              test/sim/icm45686_sim.c \
              test/sim/pwm_sim.c \
              test/sim/sim_interface.c

all: firmware integration unit sim

firmware: $(SOURCES) src/main.c
	mkdir -p build/$@
	arm-none-eabi-gcc $^ $(CFLAGS) $(LDFLAGS) -Wl,-Map=build/$@/$@.map -o build/$@/$@.elf
	arm-none-eabi-objcopy -O binary build/$@/$@.elf build/$@/$@.bin

integration: $(SOURCES) test/integration/main.c
	mkdir -p build/$@
	arm-none-eabi-gcc $^ $(CFLAGS) $(LDFLAGS) -Wl,-Map=build/$@/$@.map -o build/$@/$@.elf
	arm-none-eabi-objcopy -O binary build/$@/$@.elf build/$@/$@.bin

unit: $(SOURCES) test/unit/main.c
	mkdir -p build/$@
	arm-none-eabi-gcc $^ $(CFLAGS) $(LDFLAGS) -Wl,-Map=build/$@/$@.map -o build/$@/$@.elf
	arm-none-eabi-objcopy -O binary build/$@/$@.elf build/$@/$@.bin

sim: $(SOURCES_SIM) test/sim/main.c
	mkdir -p build/$@
	arm-none-eabi-gcc $^ $(CFLAGS) $(LDFLAGS) -Wl,-Map=build/$@/$@.map -o build/$@/$@.elf
	arm-none-eabi-objcopy -O binary build/$@/$@.elf build/$@/$@.bin

debug-firmware:
	gdb-multiarch build/firmware/firmware.elf

debug-integration:
	gdb-multiarch build/integration/integration.elf

debug-unit:
	gdb-multiarch build/unit/unit.elf

flash-firmware:
	st-flash --reset write build/firmware/firmware.bin 0x8000000

flash-integration:
	st-flash --reset write build/integration/integration.bin 0x8000000

flash-unit:
	st-flash --reset write build/unit/unit.bin 0x8000000

clean:
	rm -rf build

openocd:
	openocd -f /usr/share/openocd/scripts/interface/stlink.cfg \
			-f /usr/share/openocd/scripts/target/stm32g0x.cfg
