.DEFAULT-TARGET=kernel

kernel:
	@echo "Compiling Boot Assembly.."
	nasm -f elf32 boot.asm -o boot.o
	@echo "DONE!"
	@echo "Compiling Kernel..."
	g++ -m32 -c kernel.cpp -o kernel.o -ffreestanding -fno-exceptions -fno-rtti
	@echo "DONE!"
	@echo "Linking objects and generating kern boot image..."
	# FIXED: Changed -melf_i386 to -m32 to let GCC link the 32-bit objects correctly
	gcc -m32 boot.o kernel.o -T linker.ld -o kern -nostdlib -nodefaultlibs -lgcc
	@echo "DONE!"
	ls


clean:
	rm -f *.o kern RootHarmony.iso
	rm -rf iso/
	@echo "Cleaned build files."
	ls

iso:
	@echo "Generating ISO disk Image"
	mkdir iso/boot/grub/ -p
	cp grub.cfg iso/boot/grub/
	cp kern iso/boot/
	grub-mkrescue iso --output=RootHarmony.iso
	@echo "Done Generating Iso Images"
	ls
