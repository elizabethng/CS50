#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    // Check command-line arguments
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER hd;
    fread(&hd, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(hd) == 0)
    {
        fclose(input);
        printf("Unsupported file format.\n");
        return 2;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 3;
    }

    // Write header to file
    // TODO #6
    fwrite(&hd, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(hd);

    // Write reversed audio to file
    // TODO #8
    // ignore multiple channels for now
    // confused about what a block vs sample are

    // size of header should be 44
    // printf("Size of header is %lu bytes\n", sizeof(hd));

    // check ftell after reading in header
    // long i_loc = ftell(input);
    // long o_loc = ftell(output);
    // printf("Inpupt loc: %lu; Output loc: %lu\n", i_loc, o_loc);

    BYTE buffer[block_size];

    // intialize at start of last block
    fseek(input, -block_size, SEEK_END);

    // as long as we are not in the header, work backwards and save blocks
    while (ftell(input) >= sizeof(hd))
    {
        fread(buffer, block_size, 1, input);
        fwrite(buffer, block_size, 1, output);
        fseek(input, -2 * block_size, SEEK_CUR);

        // i_loc = ftell(input);
        // o_loc = ftell(output);
        // printf("Inpupt loc: %lu; Output loc: %lu\n", i_loc, o_loc);
    }

    // while (fread(&buffer, block_size, 1, input))
    // {
    // post_header = ftell(input);
    // printf("Current input pointer: %lu\n", post_header);

    // for now, copy sound to a temp variable so we know how big it is?
    // fwrite(&buffer, block_size, 1, output);
    //}

    // narrative of an approach, just assume one "block" of code
    // use fseek to start at the end of the file
    // copy the current (last) value to the buffer
    // the increment backward 1 block and repeat
    // until you get to 45th byte, which will be the last block
    // fseek(input, -(block_size * i), SEEK_END); // move backward in block-sized chunks

    // int file_end = fseek(input, 0, SEEK_END);
    // printf("End of file: %lu\n", post_header);

    // Close files
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    // "it suffices to show the “WAVE” marker characters are found in the
    // format member of the WAVHEADER struct"
    // printf("header format %s\n", header.format);

    // loop through all elements of header.format to check that they equal WAVE
    char *w = "WAVE";
    // printf("header format %s\n", w);

    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != w[i])
            return 0;
    }

    return 1;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    // Calculate as number of channels multiplied by bytes per sample
    // this is the bits per sample * number of channels becauase audio for
    // channels are "interleaved"
    // "Samples are the individual chunks of bits which make up the audio
    //  you hear. With more bits per sample, an audio file can have greater
    //  clarity (at the cost of more memory used!)."
    int block;
    block = header.numChannels * (header.bitsPerSample / 8);

    // printf("block size %i bytes\n", block);

    // printf("chunkID %s\n", header.chunkID); //  BYTE   chunkID[4];
    // printf("chunkSizeID %i\n", header.chunkSize); // DWORD  chunkSize;
    // printf("format %s\n", header.format); // BYTE   format[4];
    // printf("subchunk1ID %s\n", header.subchunk1ID); // BYTE   subchunk1ID[4];
    // printf("subchunk1IDSize %i\n", header.subchunk1Size); // DWORD  subchunk1Size;
    // printf("audioFormat %i\n", header.audioFormat); // WORD   audioFormat;
    // printf("numChannels %i\n", header.numChannels); // WORD   numChannels;
    // printf("sampleRate %i\n", header.sampleRate); // DWORD  sampleRate;
    // printf("byteRate %i\n", header.byteRate); // DWORD  byteRate;
    // printf("blockAlign %i\n", header.blockAlign); // WORD   blockAlign;
    // printf("bitsPerSample %i\n", header.bitsPerSample); // WORD   bitsPerSample;
    // printf("subchunk2ID %s\n", header.subchunk2ID); // BYTE   subchunk2ID[4];
    // printf("subchunk2Size %i\n", header.subchunk2Size); // DWORD  subchunk2Size;
    return block;
}