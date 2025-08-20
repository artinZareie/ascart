ASSEMBLY_SOURCES = ./main.asm
C_PROJ_NAME = ascart
ASM_PROJ_NAME = main
NASM_FLAGS = -g

setup:
	mkdir -p build
	cmake -S . -B build/
	cp build/compile_commands.json compile_commands.json

compile_c: setup
	cmake --build ./build

run_c: compile_c
	./build/$(C_PROJECT_NAME)

compile_asm: setup
	nasm $(NASM_FLAGS) -f elf64 $(ASSEMBLY_SOURCES)
	ld -o build/$(ASM_PROJ_NAME) main.o

run_asm: compile_asm
	./build/$(ASM_PROJ_NAME)

clean:
	@rm -rf build

.PHONY: setup compile_c compile_asm clean run_c run_asm
