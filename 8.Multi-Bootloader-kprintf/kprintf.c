#include <system.h>

// Define a type alias for the variable argument list
#define args_list char *

// Macro to calculate the stack size required for a type
#define _arg_stack_size(type) (((sizeof(type)-1)/sizeof(int)+1)*sizeof(int))

// Macro to initialize the variable argument list
#define args_start(ap, fmt) do { \
	ap = (char *)((unsigned int)&fmt + _arg_stack_size(&fmt));\
} while (0)

// Macro to end the variable argument list (no implementation provided)
#define args_end(ap)

// Macro to retrieve the next argument of a specified type
#define args_next(ap, type) (((type *)(ap+=_arg_stack_size(type)))[-1])

// Static buffer and pointer for formatted output
static char buf[1024] = {-1};
static int ptr = -1;

// Function to parse and convert integers to strings
static void parse_num(unsigned int value, unsigned int base) {
	unsigned int n = value / base;
	int r = value % base;
	if (r < 0) {
		r += base;
		--n;
	}
	if (value >= base) {
		parse_num(n, base);
	}
	buf[ptr++] = (r+'0');
}

// Function to parse and convert integers to hexadecimal strings
static void parse_hex(unsigned int value) {
	int i = 8;
	while (i-- > 0) {
		buf[ptr++] = "0123456789abcdef"[(value>>(i*4))&0xF];
	}
}

// Kernel printf function with support for format specifiers
void kprintf(const char *fmt, ...) {
	int i = 0;
	char *s;
	args_list args;
	args_start(args, fmt); // Initialize the variable argument list
	ptr = 0; // Initialize the buffer pointer

	// Iterate over each character in the format string
	for (; fmt[i]; ++i) {
		// Copy regular characters to the buffer
		if ((fmt[i] != '%') && (fmt[i] != '\\')) {
			buf[ptr++] = fmt[i];
			continue;
		} else if (fmt[i] == '\\') { // Handle escape sequences
			switch (fmt[++i]) {
				case 'a': buf[ptr++] = '\a'; break;
				case 'b': buf[ptr++] = '\b'; break;
				case 't': buf[ptr++] = '\t'; break;
				case 'n': buf[ptr++] = '\n'; break;
				case 'r': buf[ptr++] = '\r'; break;
				case '\\':buf[ptr++] = '\\'; break;
			}
			continue;
		}
		/* fmt[i] == '%' */
		switch (fmt[++i]) {
			// Handle string format specifier (%s)
			case 's':
				s = (char *)args_next(args, char *);
				while (*s) {
					buf[ptr++] = *s++;
				}
				break;
			// Handle character format specifier (%c)
			case 'c':
				buf[ptr++] = (char)args_next(args, int);
				break;
			// Handle hexadecimal format specifier (%x)
			case 'x':
				parse_hex((unsigned long)args_next(args, unsigned long));
				break;
			// Handle decimal format specifier (%d)
			case 'd':
				parse_num((unsigned long)args_next(args, unsigned long), 10);
				break;
			// Handle percent sign format specifier (%%)
			case '%':
				buf[ptr++] = '%';
				break;
			default:
				buf[ptr++] = fmt[i];
				break;
		}
	}
	buf[ptr] = '\0'; // Null-terminate the buffer
	args_end(args); // Cleanup the variable argument list
	puts(buf); // Output the formatted string
}
