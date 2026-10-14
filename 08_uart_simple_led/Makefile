TARGET = build/main
ELF = $(TARGET).elf
IMAGE := stm32-build

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld.bfd

CFLAGS = -mcpu=cortex-m0 -mthumb -g -Wall -Wextra -Werror -std=c11 -ffreestanding -Iinclude
LDFLAGS = -T stm32.lds

FW_SOURCES = \
	src/main.c \
	src/app.c \
	src/command.c \
	src/line_buffer.c \
	src/gpio.c \
	src/usart1.c

FW_OBJECTS = $(patsubst src/%.c,build/%.o,$(FW_SOURCES))

.PHONY: all clean flash docker-build docker-flash test test-command test-line-buffer test-app

all: $(ELF)

$(ELF): build/boot.o $(FW_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

build/boot.o: boot.S
	mkdir -p build
	$(AS) -mcpu=cortex-m0 -mthumb $< -o $@

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(ELF)
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f0x.cfg \
		-c "program $(ELF) verify reset exit"

clean:
	rm -rf build
	rm -f test_line_buffer_san test_command_san test_app_san

docker-build:
	docker build -t $(IMAGE) .

docker-flash: docker-build
	docker run --rm -it \
		--privileged \
		-v "$$PWD:/src:Z" \
		$(IMAGE) \
		bash -c "make clean && make flash"	

test-line-buffer:
	gcc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude src/line_buffer.c tests/test_line_buffer.c -o test_line_buffer_san
	./test_line_buffer_san

test-command:
	gcc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude src/command.c tests/test_command.c -o test_command_san
	./test_command_san

test-app:
	gcc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -Iinclude src/command.c src/app.c tests/test_app.c -o test_app_san
	./test_app_san

test: test-command test-line-buffer test-app
	