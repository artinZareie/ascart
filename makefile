ASSEMBLY_SOURCES = ./main.asm ./functions.asm
ASM_OBJECTS = $(ASSEMBLY_SOURCES:.asm=.o)
C_PROJ_NAME = ascart
ASM_PROJ_NAME = main
NASM_FLAGS = -g -f elf64

setup:
	mkdir -p build
	cmake -S . -B build/
	cp build/compile_commands.json compile_commands.json

compile_c: setup
	cmake --build ./build

run_c: compile_c
	./build/$(C_PROJECT_NAME)

%.o: %.asm
	nasm $(NASM_FLAGS) $< -o $@

compile_asm: setup $(ASM_OBJECTS)
	# nasm $(NASM_FLAGS) -f elf64 $(ASSEMBLY_SOURCES)
	ld -o build/$(ASM_PROJ_NAME) $(ASM_OBJECTS)

run_asm: compile_asm
	./build/$(ASM_PROJ_NAME)

clean:
	@rm -rf build

.PHONY: setup compile_c compile_asm clean run_c run_asm
