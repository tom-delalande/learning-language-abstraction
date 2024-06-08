.global _main
.align 2

_main:
    mov x5, #0b00111000
    mov x4, #0b00111000
    mov x6, #0b00111000

    mov x20, #4

    mov x14, #0

    lsr x15, x5, x20 ; row: -1, col: 0
    and x15, x15, #1
    add x14, x14, x15 ; add to counter

    lsr x15, x6, x20 ; row: +1, col: 0
    and x15, x15, #1
    add x14, x14, x15 ; add to counter

    add x20, x20, #1

    lsr x15, x6, x20
    and x15, x15, #1
    add x14, x14, x15

    lsr x15, x5, x20
    and x15, x15, #1
    add x14, x14, x15

    lsr x15, x4, x20
    and x15, x15, #1
    add x14, x14, x15

    sub x20, x20, #2

    lsr x15, x6, x20
    and x15, x15, #1
    add x14, x14, x15

    lsr x15, x5, x20
    and x15, x15, #1
    add x14, x14, x15

    lsr x15, x4, x20
    and x15, x15, #1
    add x14, x14, x15

    cmp x14, #0
    beq print_zero

    cmp x14, #1
    beq print_one

    cmp x14, #2
    beq print_two

    cmp x14, #3
    beq print_three

    cmp x14, #4
    beq print_four

    cmp x14, #5
    beq print_five

    cmp x14, #6
    beq print_six

    cmp x14, #7
    beq print_seven

    cmp x14, #8
    beq print_eight

    b print_empty

print_zero:
    adr x3, zero
    b print_x3

print_one:
    adr x3, one
    b print_x3

print_two:
    adr x3, two
    b print_x3

print_three:
    adr x3, three
    b print_x3

print_four:
    adr x3, four
    b print_x3

print_five:
    adr x3, five
    b print_x3

print_six:
    adr x3, six
    b print_x3

print_seven:
    adr x3, seven
    b print_x3

print_eight:
    adr x3, eight
    b print_x3

print_x3:
    mov x0, #0
    mov x1, x3
    mov x2, #1
    mov x16, #4
    svc 0

    mov x0, #0
    mov x16, #1
    svc 0

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

zero:
    .ascii "0"

one:
    .ascii "1"

two:
    .ascii "2"

three:
    .ascii "3"

four:
    .ascii "4"

five:
    .ascii "5"

six:
    .ascii "6"

seven:
    .ascii "7"

eight:
    .ascii "8"
