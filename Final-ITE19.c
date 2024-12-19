#include <stdio.h>  // Include standard input-output header
#include <stdlib.h> // Include standard library header
#include <string.h> // Include string manipulation header

// Map Roman numeral characters to their respective values
int getRomanCharValue(char numeral) {
    switch (numeral) {
        case 'I': return 1;   // Value for 'I'
        case 'V': return 5;   // Value for 'V'
        case 'X': return 10;  // Value for 'X'
        case 'L': return 50;  // Value for 'L'
        case 'C': return 100; // Value for 'C'
        case 'D': return 500; // Value for 'D'
        case 'M': return 1000; // Value for 'M'
        default: return -1;    // Invalid numeral
    }
}

// Convert Roman numeral string to its decimal equivalent
int convertRomanToInteger(const char *romanString) {
    int decimalResult = 0; // Initialize result to zero

    for (int i = 0; romanString[i] != '\0'; i++) {
        int currentValue = getRomanCharValue(romanString[i]);   // Get current Roman numeral value
        int nextValue = getRomanCharValue(romanString[i + 1]); // Get next Roman numeral value

        if (currentValue < nextValue) {
            decimalResult -= currentValue; // Subtract if current value is smaller than next
        } else {
            decimalResult += currentValue; // Add otherwise
        }
    }

    return decimalResult; // Return the final decimal result
}

// Helper function to concatenate strings
void appendWord(char *buffer, const char *word) {
    strcat(buffer, word);  // Append word to buffer
    strcat(buffer, " "); // Add a space after the word
}

// Convert an integer to its word representation
void convertIntegerToWords(int number, char *wordBuffer) {
    // Arrays for word equivalents of numbers
    static const char *unitWords[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    static const char *teenWords[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    static const char *tensWords[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    static const char *scaleWords[] = {"", "Thousand", "Million", "Billion"};

    if (number == 0) {
        strcpy(wordBuffer, "Zero"); // Handle zero explicitly
        return;
    }

    char tempBuffer[1024] = ""; // Temporary buffer for building the result
    int scaleIndex = 0;          // Scale index (thousands, millions, etc.)

    while (number > 0) {
        int part = number % 1000;  // Extract the last three digits
        char partBuffer[256] = ""; // Temporary buffer for the current part

        if (part > 0) {
            if (part / 100 > 0) {
                appendWord(partBuffer, unitWords[part / 100]); // Add hundreds place
                appendWord(partBuffer, "Hundred");            // Add "Hundred"
            }
            part %= 100; // Reduce to tens and units

            if (part >= 10 && part <= 19) {
                appendWord(partBuffer, teenWords[part - 10]); // Handle teens
            } else {
                if (part / 10 > 0) {
                    appendWord(partBuffer, tensWords[part / 10]); // Add tens place
                }
                part %= 10; // Reduce to units
                if (part > 0) {
                    appendWord(partBuffer, unitWords[part]); // Add units place
                }
            }

            if (scaleWords[scaleIndex][0] != '\0') {
                appendWord(partBuffer, scaleWords[scaleIndex]); // Add scale (thousand, million, etc.)
            }
        }

        char temp[1024];
        strcpy(temp, partBuffer);   // Copy current part
        strcat(temp, tempBuffer);  // Append to result buffer
        strcpy(tempBuffer, temp);  // Update result buffer

        number /= 1000; // Move to the next scale
        scaleIndex++;   // Increment scale index
    }

    strcpy(wordBuffer, tempBuffer); // Copy result to output buffer
}

// Parse and evaluate expressions involving Roman numerals
int calculateExpression(const char *expressionLine) {
    char romanNum1[100], romanNum2[100], operatorChar; // Buffers for operands and operator
    sscanf(expressionLine, "%s %c %s", romanNum1, &operatorChar, romanNum2); // Parse expression

    int firstValue = convertRomanToInteger(romanNum1); // Convert first Roman numeral to decimal
    int secondValue = convertRomanToInteger(romanNum2); // Convert second Roman numeral to decimal

    switch (operatorChar) {
        case '+': return firstValue + secondValue; // Handle addition
        case '-': return firstValue - secondValue; // Handle subtraction
        case '*': return firstValue * secondValue; // Handle multiplication
        default: return 0;                         // Invalid operator
    }
}

// Process expressions from a file
void handleFileProcessing(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");  // Open input file for reading
    FILE *outputFile = fopen(outputFilename, "w"); // Open output file for writing

    if (!inputFile || !outputFile) {
        fprintf(stderr, "Error: Unable to open file(s).\n"); // Error message for file issues
        return;
    }

    char expressionBuffer[256]; // Buffer for reading each line

    while (fgets(expressionBuffer, sizeof(expressionBuffer), inputFile)) {
        expressionBuffer[strcspn(expressionBuffer, "\n")] = '\0'; // Remove newline character

        int result = calculateExpression(expressionBuffer);  // Evaluate the expression
        char resultInWords[1024];                           // Buffer for result in words
        convertIntegerToWords(result, resultInWords);       // Convert result to words

        // Write the results to the output file
        fprintf(outputFile, "Expression: %s\nDecimal Result: %d\nWord Form: %s\n\n", expressionBuffer, result, resultInWords);
    }

    fclose(inputFile);  // Close input file
    fclose(outputFile); // Close output file
    printf("Processing complete. Check '%s' for results.\n", outputFilename); // Completion message
}

int main() {
    const char inputFile[] = "input.txt";   // Name of the input file
    const char outputFile[] = "output.txt"; // Name of the output file

    handleFileProcessing(inputFile, outputFile); // Process the input file

    return 0; // Exit program
}
