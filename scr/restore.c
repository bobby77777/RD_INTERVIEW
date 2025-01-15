#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096 // 4 KB buffer for processing

void print_usage(const char *program_name) {
    printf("Usage: %s -o MISSING_FILE P.bin EXISTING1.bin EXISTING2.bin EXISTING3.bin\n", program_name);
    printf("  -o : Specify the output file to restore (the missing file).\n");
    printf("  P.bin : Parity file.\n");
    printf("  EXISTING1.bin, EXISTING2.bin, EXISTING3.bin : Remaining input files.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 7 || strcmp(argv[1], "-o") != 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *missing_file = argv[2];
    const char *parity_file = argv[3];
    const char *existing_files[3] = { argv[4], argv[5], argv[6] };

    FILE *fp_in[4] = {NULL, NULL, NULL, NULL};
    FILE *fp_out = NULL;

    // Open parity file
    fp_in[0] = fopen(parity_file, "rb");
    if (!fp_in[0]) {
        perror(parity_file);
        goto cleanup;
    }

    // Open existing input files
    for (int i = 0; i < 3; i++) {
        fp_in[i + 1] = fopen(existing_files[i], "rb");
        if (!fp_in[i + 1]) {
            perror(existing_files[i]);
            goto cleanup;
        }
    }

    // Open the missing file for writing
    fp_out = fopen(missing_file, "wb");
    if (!fp_out) {
        perror(missing_file);
        goto cleanup;
    }

    unsigned char buffer[4][BUFFER_SIZE];
    unsigned char restored_buffer[BUFFER_SIZE];
    size_t bytes_read[4];
    int eof_reached = 0;

    while (!eof_reached) {
        eof_reached = 1;
        memset(restored_buffer, 0, BUFFER_SIZE); // Reset the restored buffer

        // Read chunks from all input files (parity + 3 remaining)
        for (int i = 0; i < 4; i++) {
            if (fp_in[i]) {
                bytes_read[i] = fread(buffer[i], 1, BUFFER_SIZE, fp_in[i]);
                if (bytes_read[i] > 0) {
                    eof_reached = 0;
                }
            } else {
                bytes_read[i] = 0;
            }
        }

        // XOR the buffers to reconstruct the missing file
        for (size_t i = 0; i < BUFFER_SIZE; i++) {
            restored_buffer[i] = buffer[0][i]; // Start with the parity
            for (int j = 1; j < 4; j++) {
                if (i < bytes_read[j]) {
                    restored_buffer[i] ^= buffer[j][i];
                }
            }
        }

        // Write the restored buffer to the output file
        fwrite(restored_buffer, 1, BUFFER_SIZE, fp_out);
    }

    printf("File '%s' successfully restored.\n", missing_file);

cleanup:
    // Close all files
    for (int i = 0; i < 4; i++) {
        if (fp_in[i]) fclose(fp_in[i]);
    }
    if (fp_out) fclose(fp_out);

    return EXIT_SUCCESS;
}
