// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // get the scaling factor, entered as string and convert to float
    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // create an array to hold a copy of the header i
    // t's 44 bytes long and we can use uint8
    // uint8_t header[HEADER_SIZE];

    // use fread to read the header
    // need to loop through all the bytes in the header and store them
    // so I'll need some memory to do that
    // but I think fread is smarter than that and that I don't have to do the looping myself
    uint8_t *header = malloc(HEADER_SIZE * sizeof(uint8_t));

    // ptr is the address of the first byte of memory into which to read the data
    // size in bytes of the type of data to read
    // nmemb number of those types to read at once
    // stream pointer to a file returned by fopen
    int fread_return;
    fread_return = fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    // printf("read %i items\n", fread_return);

    // now write the header to the new file
    // do I have to make the file separately? no that's already been done above by fopen
    int frwirte_return;
    frwirte_return = fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);
    // printf("wrote %i items\n", frwirte_return);
    free(header);

    // TODO: Read samples from input file and write updated data to output file
    // now we need a buffer to store what we're reading in after the header
    // I think we're going to go by "sample" as a unit since we don't know how long
    // the file is. documentation of fwrite looks like it has some promising syntax.

    // get some memory for the buffer, this is where I'll store each sample
    // each sample is 16 bits (or 2 bytes)
    int16_t *buffer = malloc(sizeof(int16_t));

    // do for the first one
    // int samples_read;
    // samples_read = fread(buffer, sizeof(int16_t), 1, input);
    // printf("read %i samples\n", samples_read);

    // int samples_wrote;
    // samples_wrote = fwrite(buffer, sizeof(int16_t), 1, output);
    // printf("wrote %i samples\n", samples_wrote);

    while (fread(buffer, sizeof(int16_t), 1, input))
    {
        // before writing, need to get the value that is being stored
        // in the buffer
        *buffer = *buffer * factor;
        fwrite(buffer, sizeof(int16_t), 1, output);
    }

    free(buffer);

    // Close files
    fclose(input);
    fclose(output);
}
