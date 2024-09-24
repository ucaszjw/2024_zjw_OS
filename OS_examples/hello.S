.section .data
message:
    .string "Hello OS!\n"

.section .text
.global _start

_start:
    # Load the address of the message into a1
    la a1, message

    # Load the length of the message into a2
    li a2, 10  # "Hello OS!\n" has 10 characters including the newline

    # Load the syscall number for write (64) into a7
    li a7, 64

    # Load the file descriptor for stdout (1) into a0
    li a0, 1

    # Make the syscall
    ecall

    # Exit the program
    li a7, 93  # syscall number for exit
    li a0, 0   # exit code 0
    ecall
    