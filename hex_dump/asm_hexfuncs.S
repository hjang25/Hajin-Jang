/*
 * Assembly language implementation of hexdump functions
 * Jiwon Moon, Hajin Jang
 */

	.section .rodata
	.hex_digits:
		.string	"0123456789abcdef"

	.section .text

/*
 * Write given null-terminated string to standard output.
 *
 * Parameters:
 *   s - a char array that stores the data to 'write'(print)
 * 
 */
	.globl	hex_write_string
hex_write_string:
	pushq %rbp 						# save original value of %rbp
	movq %rsp, %rbp 				# move the stack pointer to the base pointer
	subq $32, %rsp  				# allocate space on the stack for local variables
	movq %rdi, -24(%rbp) 			# store the 's' parameter in the stack frame
	movq $0, -8(%rbp) 				# initialize a loop counter to 0
	jmp .LcurCharNotNullChar 		# jump to the loop condition
.LincrementCounter: 				# start of the loop body
	addq $1, -8(%rbp) 				# increment the loop counter by 1
.LcurCharNotNullChar:					
	movq -24(%rbp), %rdx 			# load the address of the hex string into %rdx
	movq -8(%rbp), %rax 			# load the current loop index into %rax
	addq %rdx, %rax 				# add the index to the hex string address to get the address of the next character
	movzbl (%rax), %eax 			# load the next character into %eax and zero-extend it to a long integer
	testb %al, %al 					# test if the character is null-terminated
	jne .LincrementCounter 			# jump to the loop body if the character is not the null character
	movq -8(%rbp), %rdx 			# load the loop counter into %rdx
	movq -24(%rbp), %rax 			# load the hex string address into %rax
	movq %rax, %rsi 				# move the hex string address to %rsi (second argument for write)
	movl $1, %edi 					# move the integer value 1 to %edi (first argument for write)
	call write 						# call the write system function to output the string
	leave 							# restore the previous stack frame
	ret 							# return

/*
 * Format an unsigned value as an offset string consisting of exactly 8
 * hex digits.  The formatted offset is stored in sbuf, which must
 * have enough room for a string of length 8.
 *
 * Parameters:
 *   offset - an unsigned variable that contains the hexadecimal memory address
 *   sbuf - a char array that will contain the formatted offset string
 * 
 */
	.globl	hex_format_offset
hex_format_offset:
	pushq %rbp						# Save the previous stack frame pointer
	movq %rsp, %rbp					# Set the new stack frame pointer
	movl %edi, -20(%rbp)            # Store the offset parameter on the stack
	movq %rsi, -32(%rbp)            # Store the sbuf parameter on the stack
	movl $7, -4(%rbp)               # Initialize a counter to 7
	jmp	.LcounterNonNegative        # Jump to the beginning of the loop
.LsetFormattedChar:
	movl -20(%rbp), %eax            # Load the offset parameter into the eax register
	andl $15, %eax                  # Mask off the low-order 4 bits
	movl %eax, %ecx                 # Copy the masked value to ecx
	movl -4(%rbp), %eax             # Load the counter into eax
	movslq %eax, %rdx               # Sign-extend the counter to a 64-bit value and store it in rdx
	movq -32(%rbp), %rax            # Load the address of sbuf into rax
	addq %rax, %rdx                 # Add the counter to the address of sbuf to get the address of the next character in the output string
	movl %ecx, %eax                 # Copy the masked value to eax
	movzbl .hex_digits(%rax), %eax  # Look up the corresponding hex digit in the hex_digits table
	movb %al, (%rdx)                # Store the hex digit in the output string
	shrl $4, -20(%rbp)              # Shift the offset parameter right by 4 bits
	subl $1, -4(%rbp)               # Decrement the counter
.LcounterNonNegative:
	cmpl $0, -4(%rbp)               # Compare the counter to 0
	jns	.LsetFormattedChar          # If the counter is not negative, jump to the beginning of the loop
	movq -32(%rbp), %rax            # Load the address of sbuf into rax
	addq $8, %rax                   # Add 8 to the address of sbuf to get the address of the null terminator
	movb $0, (%rax)                 # Store the null terminator in the output string
	popq %rbp						# restore the stack pointer
	ret								# return

/*
 * Read up to 16 bytes from standard input into data_buf.
 *
 * Parameters:
 *   data_buf - a char array to store the hex data read
 *
 * Returns:
 *   unsigned variable that contains number of characters read
 *   a UInt256 object created from val (a single uint64_t value)
 *
 */
	.globl	hex_read
hex_read:
	pushq %rbp               # Save the previous stack frame pointer
	movq %rsp, %rbp          # Set the new stack frame pointer
	subq $32, %rsp           # Allocate 32 bytes of space on the stack
	movq %rdi, -24(%rbp)     # Save the data_buf parameter
	movq $0, -8(%rbp)        # Initialize character counter to 0
	jmp	.LloopNumChars       # Jump to the loop
.LincCounter:
	addq $1, -8(%rbp)        # Increment the character counter
.LloopNumChars:
	movq -24(%rbp), %rdx     # Load the data_buf pointer
	movq -8(%rbp), %rax      # Load the character counter
	addq %rdx, %rax          # Add the counter to the data_buf pointer to get the current position
	movzbl (%rax), %eax      # Load the next character from standard input
	testb %al, %al           # Test if the character is a null terminator
	jne	.LincCounter         # Jump to the loop if it's not a null terminator
	movq -24(%rbp), %rax     # Load the data_buf pointer
	movl $16, %edx           # Set the maximum number of characters to read
	movq %rax, %rsi          # Pass the data_buf pointer as the buffer argument to read
	movl $0, %edi            # Set the file descriptor for standard input (stdin)
	call read                # Call the read function to read characters from stdin
	movq %rax, -16(%rbp)     # Store the number of characters read
	movq -16(%rbp), %rax     # Load the number of characters read
	leave                    # Restore the previous stack frame pointer and deallocate the current stack frame
	ret                      # Return to the caller with the number of characters read and the UInt256 object

/*
 * Format a byte value (in the range 0-255) as string consisting
 * of two hex digits.  The string is stored in sbuf.
 * 
 * Parameters:
 *   byteval (%rdi) - the byte value to be formatted
 *   sbuf (%rsi) - a char array that will contain the formatted byte value
 *
 */
	.globl	hex_format_byte_as_hex
hex_format_byte_as_hex:
	pushq %rbp						  # Save the previous stack frame pointer
	movq %rsp, %rbp					  # Set the new stack frame pointer
	movl %edi, %eax                   # Move byteval into %eax
	movq %rsi, -16(%rbp)              # Save sbuf on stack
	movb %al, -4(%rbp)                # Save byteval on stack
	movzbl -4(%rbp), %eax             # Get the upper 4 bits of byteval
	shrb $4, %al                      # Shift the upper 4 bits to the right
	movzbl %al, %eax                  # Clear the upper 24 bits of %eax
	cltq                              # Sign-extend %eax to %rdx
	movzbl .hex_digits(%rax), %edx    # Get the hex digit for the upper 4 bits
	movq -16(%rbp), %rax              # Get the address of sbuf
	movb %dl, (%rax)                  # Store the hex digit in sbuf
	movzbl -4(%rbp), %eax             # Get the lower 4 bits of byteval
	andl $15, %eax                    # Clear the upper 28 bits of %eax
	movl %eax, %ecx                   # Move the lower 4 bits of byteval into %ecx
	movq -16(%rbp), %rax              # Get the address of sbuf
	leaq 1(%rax), %rdx                # Get the address of the second byte in sbuf
	movslq %ecx, %rax                 # Sign-extend %ecx to %rax
	movzbl .hex_digits(%rax), %eax    # Get the hex digit for the lower 4 bits
	movb %al, (%rdx)                  # Store the hex digit in the second byte of sbuf
	movq -16(%rbp), %rax              # Get the address of sbuf
	addq $2, %rax                     # Move the pointer past the two hex digits in sbuf
	movb $0, (%rax)                   # Null-terminate the string in sbuf
	popq %rbp						  # restore the stack pointer
	ret							   	  # return

/*
 * Convert a byte value (in the range 0-255) to a printable character
 * value.  If byteval is already a printable character, it is returned
 * unmodified.  If byteval is not a printable character, then the
 * ASCII code for '.' should be returned.
 *
 * Parameters:
 *   byteval (%rdi) - the byte value to be converted
 *
 * Returns:
 *   a char variable that contains the byte value converted
 *   to a printable character
 *
 */
.globl hex_to_printable
hex_to_printable:
	pushq %rbp			   # Save the previous stack frame pointer
	movq %rsp, %rbp		   # Set the new stack frame pointer
	movl %edi, %eax        # move the byte value to the eax register
	movb %al, -4(%rbp)     # save the byte value on the stack
	cmpb $31, -4(%rbp)     # compare the byte value to the ASCII code for ' '
	jbe	.LnotPrintable     # if the byte value is less than or equal to ' ', jump to .LnotPrintable
	cmpb $126, -4(%rbp)    # compare the byte value to the ASCII code for '~'
	ja .LnotPrintable      # if the byte value is greater than '~', jump to .LnotPrintable
	movzbl -4(%rbp), %eax  # zero-extend the byte value to a 32-bit integer in eax
	jmp	.LendIf            # jump to .LendIf
.LnotPrintable:
	movl $46, %eax         # move the ASCII code for '.' to eax (default value for non-printable characters)
.LendIf:
	popq %rbp              # restore the stack pointer
	ret                    # return the byte value (in eax) as a printable character or '.' if not printable

/* vim:ft=gas:
 */