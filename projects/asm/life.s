.global _main
.align 2

_main:
    bl print_grid

print_grid:
    mov x18, #0
    mov x19, #GRID_WIDTH
    mov x20, #0
    mov x21, #GRID_WIDTH
    bl print_grid_loop
     
print_grid_loop:
    cmp x18, x19
    b.ge print_grid_loop_finished

    bl print_cell

    add x20, x20, #1
    b print_row

print_grid_loop_finished:
    ldr x19, =0x3B9ACA00 ; this is a phat number
    b delay_then_clear

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
    b print_grid_loop

print_cell: 
    adr x1, empty
    mov x2, #1
    mov x16, #4
    svc 0
    mov x0, #0
    ret

delay_then_clear:
    subs x19, x19, #1
    b.ne delay_then_clear

    bl clear_grid

clear_grid:
    mov x0, #0
    adr x1, clear
    mov x2, #4
    mov x16, #4
    svc 0
    bl print_grid

exit:
    mov x0, #0
    mov x16, #1
    svc 0
    ret

fail:
    mov x0, #1
    mov x16, #1
    svc 0
    ret


empty:
    .ascii  "0"

filled:
    .ascii  "X"

line:
    .ascii  "\n"

clear:
    .asciz  "\x1b[2J"

.equ GRID_WIDTH, 64

array: 
    .space GRID_WIDTH * GRID_WIDTH

