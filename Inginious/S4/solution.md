# Réponses questions Inginious

## Q1

```nasm
mp:
  subl $8, %esp         ;%esp=%esp-8
  movl 16(%esp), %edx   ;On prend le deuxième argument
  movl 12(%esp), %ecx   ;On prend le premier argument
  movl %ecx,%eax
  addl %ecx,%ecx
  addl %eax,%ecx
  cmpl %edx,%ecx
  jle m1
  movl %edx, %eax
  addl $8, %esp
  ret
 m1:
  movl %ecx, %eax
  addl $8, %esp
  ret

```