long long int _syscall(unsigned long long int n,unsigned long long int a1,unsigned long long int a2,unsigned long long int a3,unsigned long long int a4,unsigned long long int a5,unsigned long long int a6);
asm(".text");
asm("_syscall:");
asm("push %rdi");
asm("push %rsi");
asm("push %rdx");
asm("push %rcx");
asm("push %r8");
asm("push %r9");
asm("mov %rdi,%rax");
asm("mov %rsi,%rdi");
asm("mov %rdx,%rsi");
asm("mov %rcx,%rdx");
asm("mov %r8,%r10");
asm("mov %r9,%r8");
asm("mov 56(%rsp),%r9");
asm("syscall");
asm("pop %r9");
asm("pop %r8");
asm("pop %rcx");
asm("pop %rdx");
asm("pop %rsi");
asm("pop %rdi");
asm("ret");

#define syscall(ax,di,si,dx,r10,r8,r9) _syscall((ax),(unsigned long long int)(di),(unsigned long long int)(si),(unsigned long long int)(dx),(unsigned long long int)(r10),(unsigned long long int)(r8),(unsigned long long int)(r9))
#define sys_read(fd,buf,size) syscall(0,(fd),(buf),(size),0,0,0)
#define sys_write(fd,buf,size) syscall(1,(fd),(buf),(size),0,0,0)
#define sys_open(name,flags,mode) syscall(2,(name),(flags),(mode),0,0,0)
#define sys_close(fd) syscall(3,(fd),0,0,0,0,0)
#define sys_brk(addr) ((void *)syscall(12,(addr),0,0,0,0,0))
#define sys_ioctl(fd,cmd,buf) syscall(16,(fd),(cmd),(buf),0,0,0)
#define sys_fcntl(fd,cmd,arg) syscall(72,(fd),(cmd),(arg),0,0,0)
