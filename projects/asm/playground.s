.global _main
.align 2

_main:
    mov x16, #0b01010101

    mov x15, #0
    lsr x14, x16, x15
    and x14, x14, #1

    cmp x14, #0
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
