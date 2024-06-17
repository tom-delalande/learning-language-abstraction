.global _main
.align 2

; x6: current row
; x18: outer loop counter
; x19: inner loop counter

; x7: current row edit
; x5: previous row
; x4: next row

_main:
    ; init grid
    mov x18, #0
    mov x19, #GRID_WIDTH
    mov x17, #0
    b init_grid_loop

init_grid_loop:
    cmp x18, x19
    b.ge print_grid

    cmp x17, #3
    b.eq reset_x17

    cmp x17, #0
    b.eq set_row_8

    cmp x17, #1
    b.eq set_row_9

    cmp x17, #2
    b.eq set_row_10


    mov x0, #0
    str x0, [sp]
    sub sp, sp, #16

    add x18, x18, #1
    b init_grid_loop

reset_x17:
    mov x17, #0
    b init_grid_loop

set_row_8:
    mov x0, #0b01000
    str x0, [sp]
    sub sp, sp, #16

    add x18, x18, #1
    b init_grid_loop

set_row_9:
    mov x0, #0b010000
    str x0, [sp]
    sub sp, sp, #16

    add x18, x18, #1
    b init_grid_loop

set_row_10:
    mov x0, #0b011100
    str x0, [sp]
    sub sp, sp, #16

    add x18, x18, #1
    b init_grid_loop

print_grid:
    mov x18, #0
    mov x19, #GRID_WIDTH
    mov x20, #0
    mov x21, #GRID_WIDTH
    mov x15, #0

    ldr x6, [sp]
    ldr x7, [sp]
    sub sp, sp, #16
    ldr x5, [sp]
    add sp, sp, #32
    ldr x4, [sp]
    sub sp, sp, #16

    bl print_grid_loop

print_grid_loop:
    cmp x18, x19 ; for each row
    b.ge print_grid_loop_finished

    b update_row

print_grid_loop_finished:
    mov x1, #64
    mov x2, #16
    mul x3, x1, x2
    sub sp, sp, x3
    ldr x19, =0x3B9ACA00 ; this is a phat number
    b delay_then_clear

update_row:
    cmp x20, x21
    b.ge update_row_finished

    lsr x13, x6, x20 ; store the current bit in x13 by moving bit right
    and x13, x13, #1

    ; use playground to validate this code

    mov x14, #0

    lsr x15, x5, x20 ; row: -1, col: 0
    and x15, x15, #1
    add x14, x14, x15 ; add to counter

    lsr x15, x4, x20 ; row: +1, col: 0
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

    add x20, x20, #1

    cmp x13, #1
    b.eq process_live_cell

    ; process dead cell
    cmp x14, #3
    b.eq revive_cell

    add x20, x20, #1 ; increment and loop
    b update_row

process_live_cell:
    cmp x14, #2
    b.lt kill_cell

    cmp x14, #3
    b.gt kill_cell

    add x20, x20, #1 ; increment and loop
    b update_row

kill_cell:
    mov x3, #1
    lsl x3, x3, x20
    mvn x3, x3
    and x7, x7, x3

    add x20, x20, #1 ; increment and loop
    b update_row

revive_cell:
    mov x3, #1
    lsl x3, x3, x20
    orr x7, x7, x3

    add x20, x20, #1 ; increment and loop
    b update_row

update_row_finished:
    mov x20, #0
    mov x21, #GRID_WIDTH
    b print_row

print_row:
    cmp x20, x21
    b.ge print_row_finished

    lsr x14, x6, x20
    and x14, x14, #1

    add x15, x15, #1

    cmp x14, #0
    b.eq print_cell_empty
    b print_cell_filled

print_row_finished:
    adr x1, line
    mov x2, #1
    mov x16, #4

    svc 0
    mov x0, #0

    str x7, [sp]
    add sp, sp, #16

    mov x5, x6
    mov x6, x4
    mov x7, x4

    add sp, sp, #16
    ldr x4, [sp]
    sub sp, sp, #16

    add x18, x18, #1
    mov x20, #0
    b print_grid_loop

print_cell_empty: 
    adr x1, empty
    mov x2, #1
    mov x16, #4
    svc 0
    mov x0, #0
    b print_cell_finish

print_cell_filled: 
    adr x1, filled
    mov x2, #1
    mov x16, #4
    svc 0
    mov x0, #0
    b print_cell_finish

print_cell_finish:
    add x20, x20, #1
    b print_row

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
    .ascii "-"

filled:
    .ascii "X"

line:
    .ascii "\n"

clear:
    .asciz "\x1b[2J"

.equ GRID_WIDTH, 64

grid:
   .fill 32
