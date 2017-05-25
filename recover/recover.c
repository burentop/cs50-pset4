#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // get file name
    char *infile = argv[1];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // keep track of how many jpegs found
    int foundJpegs = -1;
    FILE *img;
    
    // buffer to hold data
    unsigned char buffer[512];
    
    while (fread(&buffer, 512, 1, inptr) == 1)
    {
        unsigned char fourth = buffer[3];
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (fourth == 0xe0 || fourth == 0xe1 || fourth == 0xe2 || fourth == 0xe3 ||
             fourth == 0xe4 || fourth == 0xe5 || fourth == 0xe6 || fourth == 0xe7 ||
             fourth == 0xe8 || fourth == 0xe9 || fourth == 0xea || fourth == 0xeb ||
             fourth == 0xec || fourth == 0xed || fourth == 0xee || fourth == 0xef))
        {
            if (foundJpegs == -1)
            {
                foundJpegs++;
                char str[8];
                sprintf(str, "%03i.jpg", foundJpegs);
                img = fopen(str, "w");
                fwrite(&buffer, 512, 1, img);
            }
            else if (foundJpegs > -1)
            {
                fclose(img);
                foundJpegs++;
                char str[8];
                sprintf(str, "%03i.jpg", foundJpegs);
                img = fopen(str, "w");
                fwrite(&buffer, 512, 1, img);
            }
        }
        else if (foundJpegs > -1)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }
    
    fclose(inptr);
    
    // success
    return 0;
}