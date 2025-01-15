#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096 // 4 KB buffer for processing

void print_usage(const char *program_name) {
    printf("Usage: %s -o P.bin A.bin B.bin C.bin D.bin\n", program_name);
    printf("  -o : Specify output parity file.\n");
    printf("  A.bin, B.bin, C.bin, D.bin : Input files to create parity.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 7 || strcmp(argv[1], "-o") != 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *parity_file = argv[2];
    const char *input_files[4] = { argv[3], argv[4], argv[5], argv[6] };

    FILE *fp_in[4] = {NULL, NULL, NULL, NULL};
    FILE *fp_out = NULL;

    // Open input files
    for (int i = 0; i < 4; i++) {
        fp_in[i] = fopen(input_files[i], "rb");
        if (!fp_in[i]) {
            perror(input_files[i]);
            goto cleanup;
        }
    }

    // Open parity file for writing
    fp_out = fopen(parity_file, "wb");
    if (!fp_out) {
        perror(parity_file);
        goto cleanup;
    }

    unsigned char buffer[4][BUFFER_SIZE];
    unsigned char parity_buffer[BUFFER_SIZE];
    size_t bytes_read[4];
    int eof_reached = 0;

    while (!eof_reached) {
        eof_reached = 1;
        memset(parity_buffer, 0, BUFFER_SIZE); // Reset parity buffer

        // Read chunks from all input files
        for (int i = 0; i < 4; i++) {
            bytes_read[i] = fread(buffer[i], 1, BUFFER_SIZE, fp_in[i]);
            if (bytes_read[i] > 0) {
                eof_reached = 0;
            }
        }

        // XOR the buffers to create parity
        for (size_t i = 0; i < BUFFER_SIZE; i++) {
            for (int j = 0; j < 4; j++) {
                if (i < bytes_read[j]) {
                    parity_buffer[i] ^= buffer[j][i];
                }
            }
        }

        // Write the parity buffer to the output file
        fwrite(parity_buffer, 1, BUFFER_SIZE, fp_out);
    }

    printf("Parity file '%s' successfully created.\n", parity_file);

cleanup:
    // Close all files
    for (int i = 0; i < 4; i++) {
        if (fp_in[i]) fclose(fp_in[i]);
    }
    if (fp_out) fclose(fp_out);

    return EXIT_SUCCESS;
}
