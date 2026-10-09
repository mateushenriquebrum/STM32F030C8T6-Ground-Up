TARGET = build/main
ELF = $(TARGET).elf
IMAGE := stm32-build

all: $(ELF)

$(ELF): build/boot.o build/main.o
	arm-none-eabi-ld.bfd -T stm32.lds -o $@ $^

build/boot.o: boot.S
	mkdir -p build
	arm-none-eabi-as boot.S -o $@

build/main.o: main.c
	mkdir -p build
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -g -c main.c -o $@	

flash: $(ELF)
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f0x.cfg \
		-c "program $(ELF) verify reset exit"
	
clean:
	rm -rf build

docker-build:
	docker build -t $(IMAGE) .

docker-flash: docker-build
	docker run --rm -it \
		--privileged \
		-v "$$PWD:/src:Z" \
		$(IMAGE) \
		bash -c "make clean && make flash"		