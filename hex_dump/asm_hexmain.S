/*
 * Hexdump main function, assembly language version
 * Jiwon Moon, Hajin Jang
 */

  .section .rodata

sColonStr:      .string ": "              # define colon format
sSmallSpaceStr: .string " "               # define small space format
sLargeSpaceStr: .string "   "             # define large space format
sNewLineStr:    .string "\n"              # define newline format

	.section .text

	.globl	main
main:
	pushq	%rbp                             # save original %rbp value
	movq %rsp, %rbp                        # set the stack pointer to %rbp
	subq $64, %rsp                         # reserve space on the stack for local variables
	movl $0, -4(%rbp)                      # initialize offset to 0
	jmp	.LcheckWhileLoopCondition          # jump to the loop

            
.LwhileLoopConditionTrue:                # while loop body for printing hex bytes
  movl -4(%rbp), %edi                    # pass offset as the first parameter for hex_format_offset
  leaq -57(%rbp), %rsi                   # pass offset_buf as the second parameter
	call hex_format_offset                 # call the hex_format_offset function to format the byte's offset
  leaq -57(%rbp), %rdi                   # pass offset_buf as the parameter for hex_write_string
	call hex_write_string                  # call the hex_write_string function to write the offset to stdout
	movq $sColonStr, %rdi                  # pass ": " as the parameter for hex_write_string
	call hex_write_string                  # call the hex_write_string function to write the string literal to stdout
	movl $0, -8(%rbp)                      # initialize a second counter to 0
	jmp	.LdeterminePrintDigitOrBlank       # jump to the loop to print hex digits or blank depending on length


.LprintHexValForEachByte:
	movl -8(%rbp), %eax                    # load the current digit (to print) into %eax
	cltq                                   # convert the digit to a quadword (64-bit integer)
	movzbl -48(%rbp,%rax), %eax            # convert the digit to a character by adding the ASCII code for '0'
	movzbl %al, %eax                       # move zero-extended byte into %eax (since hex_format_byte_as_hex takes a single byte as input)

	leaq -60(%rbp), %rdx                   # calculate the address of the current digit
	movq %rdx, %rsi                        # pass byte_buf (char array of size 3) as the second parameter of hex_format_byte_as_hex
	movl %eax, %edi                        # pass data_buf[i] (current buffer digit) as the first parameter of hex_format_byte_as_hex
	call hex_format_byte_as_hex            # call the hex_format_byte_as_hex function to format the digit as a hexadecimal
	leaq -60(%rbp), %rdi                   # pass byte_buf as the parameter of hex_write_string
	call hex_write_string                  # call the hex_write_string function to write the formatted digit to stdout
	movq $sSmallSpaceStr, %rdi             # pass the small space format as the parameter of hex_write_string
	call hex_write_string                  # call the hex_write_string function to write the space to stdout
	addl $1, -8(%rbp)                      # increment the digit counter


.LdeterminePrintDigitOrBlank:             # if else statement that determines whether to print blanks or the digits
	movl -8(%rbp), %eax                     # load the value of the second counter into %eax
	cmpl -20(%rbp), %eax                    # compare the current digit (value in counter) with the maximum number of digits to print (bytes_read)
  jl .LprintHexValForEachByte             # if the current digit is less than bytes_read, continue printing
  movl -20(%rbp), %eax                    # otherwise, set the current digit to the maximum number of digits
  movl %eax, -12(%rbp)                    # and store it in the third loop counter (to be used in the next for loop)
  jmp .LprintSpaceWhenLenNotLessThan16    # jump to the loop that prints spaces


.LalignStringRepWhenLenLessThan16:        /* for loop body that aligns “string-like” representation of the 
                                             data when its length is less than 16 */
  movq $sLargeSpaceStr, %rdi              # move the large space format (3 spaces) into the destination index register
  call hex_write_string                   # call the hex_write_string function to write the space to stdout
  addl $1, -12(%rbp)                      # increment the third loop counter


.LprintSpaceWhenLenNotLessThan16:
  cmpl $15, -12(%rbp)                     # compare the third loop counter with 15 (the max number of digits to be printed per line)
  jle .LalignStringRepWhenLenLessThan16   # if the loop counter is less than or equal to 15, continue printing spaces
  movq $sSmallSpaceStr, %rdi              # otherwise, print a small space (single space) and reset the loop counter to 0
  call hex_write_string                   # call the hex_write_string function to write the space to stdout
  movl $0, -16(%rbp)                      # reset the loop counter for printable characters
  jmp .LwriteStringAndNewLine             # jump to the loop for printing printable characters


.LformatAndPrintPrintableChars:
  movl -16(%rbp), %eax                    # load the current value of the loop counter to %eax
  cltq                                    # convert the character to a 64-bit integer
  movzbl -48(%rbp,%rax), %eax             # convert the character to a printable character by adding the ASCII code for '0'
	movzbl %al, %eax                        # zero-extend byte to a long integer
  movl %eax, %edi                         # pass the integer as a parameter for hex_to_printable
  call hex_to_printable                   # call the function hex_to_printable to convert the integer to a printable character
  movb %al, -62(%rbp)                     # store the converted character to -62(%rbp)
  movb $0, -61(%rbp)                      # move 0 to -61(%rbp) to null-terminate the string stored
  leaq -62(%rbp), %rax                    # load the address of -62(%rbp) (which stores printable character) into %rax
  movq %rax, %rdi                         # pass the address of the memory that stores the printable character as the parameter of hex_write_string
  call hex_write_string                   # call the function hex_write_string to write the printable character
  addl $1, -16(%rbp)                      # increment the loop counter for printable characters


.LwriteStringAndNewLine:
  movl -16(%rbp), %eax                    # move the value at -16(%rbp) (the loop counter) to %eax
  cmpl -20(%rbp), %eax                    # compare bytes_read and %eax
  jl .LformatAndPrintPrintableChars       # jump to .LformatAndPrintPrintableChars if bytes_read is less than %eax
  movq $sNewLineStr, %rdi                 # pass the newline string format as the parameter of hex_write_string
  call hex_write_string                   # call the function hex_write_string to print the newline
  movl -20(%rbp), %eax                    # move bytes_read to %eax
  addl %eax, -4(%rbp)                     # add bytes_read to offset


.LcheckWhileLoopCondition:
  movq $0, -48(%rbp)                      # initialize any value in -48(%rbp) before using it
  leaq -48(%rbp), %rax                    # load the address of -48(%rbp) into %rax
  movq %rax, %rdi                         # move the address to %rdi, which is used as an argument register
  call hex_read                           # call the function hex_read to read user input
  movl %eax, -20(%rbp)                    # move the result of hex_read (bytes_read) to -20(%rbp)
  cmpl $0, -20(%rbp)                      # check that bytes_read is greater than 0
  jg .LwhileLoopConditionTrue             # jump to .LwhileLoopConditionTrue if at least one byte was read
  leave                                   # clean up the stack frame
  ret                                     # return from the function


/* vim:ft=gas:
 */