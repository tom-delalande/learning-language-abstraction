.text
.global _main
.align 2

_main:
    adrp x1, question@PAGE
    add x1, x1, question@PAGEOFF
    mov x2, #19
    mov x16, #4
    svc 0
    mov x0, #0

    mov x0, #0
    adrp x1, buffer@PAGE
    add x1, x1, buffer@PAGEOFF
    mov x2, #10
    mov x16, #3
    svc 0
    mov x19, x0

    mov x0, #1
    adrp x1, hello@PAGE
    add x1, x1, hello@PAGEOFF
    mov x2, #7
    mov x16, #4
    svc 0
    mov x0, #0

    adrp x1, buffer@PAGE
    add x1, x1, buffer@PAGEOFF
    mov x2, #10
    mov x16, #4
    svc 0
    mov x0, #0

    mov x16, #1
    svc 0

.data

hello:
    .ascii  "Hello, "

question:
    .ascii  "What is your name?\n"

.align 2
buffer: .space 10

