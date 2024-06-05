.global _main
.align 2

_main:
    mov x18, #0
    mov x19, #9
    mov x20, #0
    mov x21, #9
    bl print_grid
    bl exit

print_grid:
    cmp x18, x19
    b.ge exit

    b print_row

print_row:
    cmp x20, x21
    b.ge print_row_finished

    bl print_cell

    add x20, x20, #1
    b print_row

print_row_finished:
    adr x1, line
    mov x2, #1
    mov x16, #4

    svc 0
    mov x0, #0

    add x18, x18, #1
    mov x20, #0
    b print_grid

print_cell: 
    adr x1, empty
    mov x2, #1
    mov x16, #4
    svc 0
    mov x0, #0
    ret

exit:
    mov x0, #0
    mov x16, #1
    svc 0


empty:
    .ascii  "0"

filled:
    .ascii  "X"

line:
    .ascii  "\n"
