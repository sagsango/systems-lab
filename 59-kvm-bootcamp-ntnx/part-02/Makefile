target=kvm_open guest
all:$(target)

kvm_open:kvm_open.c
	gcc -g3 -o kvm_open kvm_open.c

run:kvm_open guest
	./kvm_open

guest:guest.S
	as guest.S -o guest.o
	objcopy -O binary --only-section=.text guest.o guest
	xxd guest
clean:
	rm $(target)
