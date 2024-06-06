.global _main
.align 2

_main:
    mov x0, #0b01010101

    mov x1, #2
    lsr x2, x0, x1
    and x2, x2, #1

    cmp x2, #0
    beq print_empty
    b print_filled

print_empty:
    mov x0, #0
    adr x1, empty
    mov x2, #1
    mov x16, #4
    svc 0

    mov x0, #0
    mov x16, #1
    svc 0
    
print_filled:
    mov x0, #0
    adr x1, filled
    mov x2, #1
    mov x16, #4
    svc 0

    mov x0, #0
    mov x16, #1
    svc 0

number:
    .byte 0b110101

empty:
    .ascii "-"

filled:
    .ascii "X"
