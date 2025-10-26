


#include <asm/kvm.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <assert.h>

#define GUEST_SIZE 8
#define GUEST_BINAY "guest"

void load_opcode_from_elf(void * addr) {
	char * buffer = addr;
	int fd = open(GUEST_BINAY, O_RDONLY);
	if (fd < 0) {
		exit(101);
	}
	int bytes = read(fd, buffer, GUEST_SIZE);
	if (bytes != GUEST_SIZE) {
		exit(102);
	}
}

int main() {
	int status;
	int kvm_fd = open("/dev/kvm", O_RDWR);
	printf("kvm_fd:%d\n", kvm_fd);
	if (kvm_fd < 0) {
		exit(1);
	}
	int kvm_version = ioctl(kvm_fd, KVM_GET_API_VERSION);
	if (kvm_version < 0) {
		exit(2);
	}
	printf("kvm_version:%d\n", kvm_version);
	if (kvm_version != KVM_API_VERSION) {
		exit(3);
	}
	int vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, KVM_X86_DEFAULT_VM);
	if (vm_fd < 0) {
		exit(4);
	}
	printf("vm_fd:%d\n", vm_fd);
	
	int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, /*[0, KVM_CAP_NR_VCPUS]*/ 0);
	if (vcpu_fd < 0) {
		exit(5);
	}
	printf("vcpu_fd:%d\n", vcpu_fd);
	struct kvm_regs	kvm_rs;
	status = ioctl(vcpu_fd,  KVM_GET_REGS, &kvm_rs);
	if (status < 0) {
		exit(6);
	}

	printf("kvm_rs.rip:%p\n", kvm_rs.rip);
	struct kvm_sregs kvm_srs;


	status = ioctl(vcpu_fd,  KVM_GET_SREGS, &kvm_srs);
	if (status < 0) {
		exit(6);
	}
	printf("vcpu regs:%p\n", kvm_srs.cs.base);

	// void * start_addr = kvm_srs.cs.base 

	void * addr = mmap(0, PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		exit(7);
	}
	printf("addr:%p\n", addr);	

	struct kvm_userspace_memory_region kvm_mem;
	kvm_mem.flags = 0;
	kvm_mem.slot = 0;
	kvm_mem.memory_size = PAGE_SIZE;
	kvm_mem.guest_phys_addr = 0;
	kvm_mem.userspace_addr = (__u64) addr;

	status = ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &kvm_mem);
	if (status < 0) {
		exit(8);
	}

	/*
	.code16
	_start:
		mov $0x2a, %rax    # Move 0x2a (42 decimal) into RAX
		hlt                # Halt the CPU
	*/

	/*
	char * opcode = (char*)addr;
	opcode[0] = 0xb8;
	opcode[1] = 0x2a;
	opcode[2] = 0x00;
	opcode[3] = 0xf4;
	*/

	load_opcode_from_elf(addr);


	kvm_rs.rip = 0;

	status = ioctl(vcpu_fd,  KVM_SET_REGS, &kvm_rs);
	if (status < 0) {
		exit(9);
	}

	// Code segmnet contains the code start addredd.
	// rip is just an offset under the code segment.
	kvm_srs.cs.base = 0;
	kvm_srs.cs.selector = 0;

	status = ioctl(vcpu_fd,  KVM_SET_SREGS, &kvm_srs);
	if (status < 0) {
		exit(10);
	}

	int kvm_run_size = ioctl(kvm_fd,KVM_GET_VCPU_MMAP_SIZE);
	if (kvm_run_size < 0) {
		exit(13);
	}

	struct kvm_run * kvm_rn = mmap(0, kvm_run_size, PROT_READ | MAP_SHARED, MAP_SHARED, vcpu_fd, 0);
	if (kvm_rn == MAP_FAILED) {
		exit(14);
	}


	while (kvm_rn->exit_reason != KVM_EXIT_INTERNAL_ERROR) {
		/* XXX:
		*
		* CPU starts from 16 bit mode or real mode)
		*
		* */
		status = ioctl(vcpu_fd,  KVM_RUN, 0);
		printf("ioct:%d\n", status);
		if (status < 0) {
			exit(11);
		}
		printf("Hello\n");
		status = ioctl(vcpu_fd,  KVM_GET_REGS, &kvm_rs);
		if (status < 0) {
			exit(12);
		}

		printf("kvm_rs.rip:%p\n", kvm_rs.rip);
		printf("kvm_rs.rax:%p\n", kvm_rs.rax);

		printf("exit_reason:%d\n", kvm_rn->exit_reason);

		/*
			struct {
				#define KVM_EXIT_IO_IN  0
				#define KVM_EXIT_IO_OUT 1
				__u8 direction;
				__u8 size; // bytes 
				__u16 port;
				__u32 count;
				__u64 data_offset; // relative to kvm_run start 
			} io;
		*/
		if (kvm_rn->exit_reason ==  KVM_EXIT_IO ) {
			printf("port io: size:%d\n", kvm_rn->io.size); // 16 bit
			//int16_t data = kvm_rn->io.data_offset;
		}
	}

	close (vcpu_fd);
	close (vm_fd);
	close (kvm_fd);

	//while(1);
}
