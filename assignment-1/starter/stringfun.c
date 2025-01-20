#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int reverse_string(char*, int, int);
int word_print(char*, int, int);


int setup_buff(char *buffer, char *user_input, int buff_sz) {
    char *source = user_input;
    char *destination = buffer;
    int real_length = 0;
    int total_used = 0;
    int in_whitespace = 1;
	 int buffer_length = buff_sz;

    while (*source != '\0') {
        if (*source == ' ' || *source == '\t') {
		  		//if we encounter whitespace after a word, add a single space to the buffer
            if (!in_whitespace) {
                if (total_used >= buffer_length) return -1;
                *destination++ = ' ';
                total_used++;
                real_length++;
                in_whitespace = 1;
            }
        } else {
            if (total_used >= buffer_length) return -1;
				//if we encounter whitespace after a word, add a single space to the buffer
            *destination++ = *source;
            total_used++;
            real_length++;
            in_whitespace = 0;
        }
        source++;
    }

	 //if the last character added to the buffer was a space, remove it
    if (in_whitespace && real_length > 0) {
        destination--;
        total_used--;
        real_length--;
    }

	 //fill the remainder of the buffer with dots
    while (total_used < buffer_length) {
        *destination++ = '.';
        total_used++;
    }

    return real_length;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff + i));
    }
    putchar(']');
	 putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buffer, int length, int buff_sz) {
    int word_count = 0;
    int in_word = 0;
	 (void)buff_sz;

    for (int i = 0; i < length; i++) {
        if (buffer[i] == ' ') {
            if (in_word) {
                word_count++;
                in_word = 0; //set flag to indicate end of a word
            }
        } else {
            in_word = 1; //set flag to indicate inside a word
        }
    }

    if (in_word) {
        word_count++;
    }

    return word_count;
}

int reverse_string(char *buffer, int length, int buff_sz) {
    int left_index = 0;
    int right_index = length - 1;
	 (void)buff_sz;

    while (buffer[right_index] == '.' && right_index > left_index) {
        right_index--;
    }

	 //reverse the string by swapping characters from both ends
    while (left_index < right_index) {
        char temp = buffer[left_index];
        buffer[left_index] = buffer[right_index];
        buffer[right_index] = temp;
        left_index++;
        right_index--;
    }

    return 0;
}

int word_print(char *buffer, int length, int buff_sz) {
    int word_count = 0;
    int in_word = 0;
    int word_start = 0;
    int index = 0;
	 (void)buff_sz;

    printf("Word Print\n----------\n");

    while (index < length && buffer[index] != '.') {
        char current_char = buffer[index];

        if (current_char == ' ') {
            if (in_word) {
                int word_length = index - word_start;
                word_count++;
                printf("%d. ", word_count);
                for (int j = word_start; j < index; j++) {
                    putchar(buffer[j]);
                }
                printf("(%d)\n", word_length);
                in_word = 0; //reset flag for end of a word
            }
        } else {
            if (!in_word) {
					 //start of new word
                in_word = 1;
                word_start = index;
            }
        }
        index++;
    }

	 //if we end word, print final word
    if (in_word) {
        int word_length = index - word_start;
        word_count++;
        printf("%d. ", word_count);
        for (int j = word_start; j < index; j++) {
            putchar(buffer[j]);
        }
        printf("(%d)\n", word_length);
    }

    printf("\nNumber of words returned: %d\n", word_count);
    return word_count;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
	 /*
	 			  This is safe because or is a short-circuit operation. That means
	 			  if the first operand evaluates to true, there is no need to evaluate
	 			  the second operand. This means if argc is less than 2 it doesn't
	 			  matter if argv[1] exists, because it won't be evaluated. 
	 			  On the other hand, if argc is greater than 2 and we have to check
	 		     the second operand of our or statement, we know that argv[1] will
	 			  exist, since argc tell us we have 2 or more values.
	 */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }
	 
	 //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
	 /*
	 			  The if statement below checks that we have at least 3 arguments.
				  We need at least an executable, an option flag, and the user's string,
				  so if we have less than 3 we know that we don't have a user string.
				  Without this check, we would have an error, rather than showing the user
				  the correct format and exiting with an error code.
	 */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

	 //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a
    //          return code of 99
	 buff = (char*)malloc(BUFFER_SZ * sizeof(char));
	 if (buff == NULL) {
	 	printf("Error allocating memory for buffer.\n");
		exit(99);
	 }
   

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);       //see todos   
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }
	 		// Adding the case statements for 'r' and 'w'

	 switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len, BUFFER_SZ);  
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
					 exit(3);
            }
            printf("Word Count: %d\n", rc);
            break;
			case 'r':
				rc = reverse_string(buff, user_str_len, BUFFER_SZ);
					if (rc < 0) {
						printf("Error reversing string, rc = %d\n", rc);
						exit(3);
					}
					break;
			case 'w':
				rc = word_print(buff, user_str_len, BUFFER_SZ);
				if (rc < 0) {
					printf("Error printing words, rc = %d\n", rc);
					exit(3);
				}
				break;
			case 'x':
				printf("Not Implemented!\n");
				exit(3);
			default:
				usage(argv[0]);
				exit(1);
	}

	print_buff(buff, BUFFER_SZ);
	free(buff);
	exit(0);
}


//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
/*
				Providing both the pointer and the length as arguments 
				ensures that helper functions operate safely and correctly. 
				It prevents buffer overflows, enhances function reusability 
				with buffers of different sizes, and makes the code more 
				readable and easier to understand. This may also help with 
				debugging and maintaining the code by clearly specifying the 
				size constraints of the buffer being used.
*/


