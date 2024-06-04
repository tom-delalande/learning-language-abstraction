.global _main
.p2align 3

_main:
    adr X1, helloworld
    mov X2, #13
    mov X16, #4     // MacOS write system call
    svc 0

    mov X0, #0
    mov X16, #1
    svc 0

helloworld:
    .ascii  "Hello, world!\n"

