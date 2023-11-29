section .bss
    j resd 1 ; Define a single 32-bit (4-byte) uninitialized variable j

section .text
global _start

_start:
    ; Initialize j to 3
    mov dword [j], 3

.LBB3_1:
    ; Compare the value of j with 0
    cmp dword [j], 0
    jle .LBB3_3 ; If j is less than or equal to 0, jump to the end

    ; Subtract 3 from j
    mov eax, [j]
    sub eax, 3
    mov [j], eax

    jmp .LBB3_1 ; Jump back to the beginning of the loop

.LBB3_3:
    ; Your program logic when the loop ends goes here

section .data
; Define any data you need here (if necessary)

section .bss
; Define any other uninitialized variables here (if necessary)

