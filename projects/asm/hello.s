.global _main
.align 2

_main:
    adr x1, helloworld
    mov x2, #13
    mov x16, #4
    svc 0

    mov x0, #0
    mov x16, #1
    svc 0

helloworld:
    .ascii  "Hello, world!\n"

