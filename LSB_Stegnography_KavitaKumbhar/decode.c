#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"

uint decode_lsb_to_int(char *buffer)
{
    uint data = 0;
    int i = 0;
    while (i < 32)
    {
        uint mask = (buffer[i] & 1) << (31 - i);
        data = data | mask;
        i++;
    }
    return data;
}
char decode_lsb_to_byte(char *buffer)
{
    char data = 0;
    int i = 0;
    while (i < 8)
    {
        unsigned char mask = (buffer[i] & 0x01) << (7 - i);
        data = (data | mask);
        i++;
    }
    return data;
}
Status openfiles(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_imag_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_imag_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    if(argc <=2)
    {
        return e_failure;
    }
    if (argc == 3 || argc == 4)
    {
        if (strstr(argv[2], ".bmp") == NULL)
        {
            return e_failure;
        }
        decInfo->stego_imag_fname = argv[2];
        if (argc != 4)
        {

            strcpy(decInfo->output_file_fname, "output_file");
        }
        else
        {
            strcpy(decInfo->output_file_fname, argv[3]);
        }
    }
    return e_success;
}

Status Skip_bmp_header(FILE *fptr_stego_image)
{
    printf("Info: Skip BMP Header\n");
    fseek(fptr_stego_image, 54, SEEK_SET);
    return e_success;
}

Status decode_magicstring_length(DecodeInfo *decInfo)
{
    printf("Info: Decoding magic string size \n");
    char buffer[32];
    fseek(decInfo->fptr_stego_image, 0, SEEK_CUR);
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    decInfo->magic_string_len = decode_lsb_to_int(buffer);
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Info: Decoding magic string\n");
    int i = 0;
    while (i < decInfo->magic_string_len)
    {
        fread(decInfo->imag_data, MAX_IMAGE_BUF_SIZE, 1, decInfo->fptr_stego_image);
        decInfo->magicstring[i] = decode_lsb_to_byte(decInfo->imag_data);
        i++;
    }
    decInfo->magicstring[i] = '\0';
    char user[20];
    printf("Enter the magic string : ");
    scanf(" %[^\n]", user);

    if (!strcmp(decInfo->magicstring, user))
    {
        return e_success;
    }
    else
    {
        printf("Din't match with magic string Try again \n");
        return decode_magic_string(decInfo);
    }
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    printf("Info: Decoding secret file extension size\n");
    char buffer[32];
    fseek(decInfo->fptr_stego_image, 0, SEEK_CUR);
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    decInfo->file_extn_size = decode_lsb_to_int(buffer);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("Info: Decoding secret file extension \n");
    int i = 0;
    while (i < decInfo->file_extn_size)
    {
        fread(decInfo->imag_data, MAX_IMAGE_BUF_SIZE, 1, decInfo->fptr_stego_image);
        decInfo->file_extn[i] = decode_lsb_to_byte(decInfo->imag_data);
        i++;
    }
    decInfo->file_extn[i] = '\0';
    strcat(decInfo->output_file_fname, decInfo->file_extn);

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("Info: Decoding secret file size\n");
    char buffer[32];
    fseek(decInfo->fptr_stego_image, 0, SEEK_CUR);
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    decInfo->sec_file_size = decode_lsb_to_int(buffer);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("Info: Decoding secret file data\n");
    int i = 0;
    uint sec_size = decInfo->sec_file_size;
    decInfo->output_fname = fopen(decInfo->output_file_fname, "w");
    while (i < decInfo->sec_file_size)
    {
        fread(decInfo->imag_data, MAX_IMAGE_BUF_SIZE, 1, decInfo->fptr_stego_image);
        fprintf(decInfo->output_fname, "%c", decode_lsb_to_byte(decInfo->imag_data));
        i++;
    }
    return e_success;
}

/*Status do_decoding(DecodeInfo *decInfo)
{
    if (openfiles(decInfo) == e_failure)
    {

        printf("Error : Files not found\n");
        return e_failure;
    }
    if (Skip_bmp_header(decInfo->fptr_stego_image) == e_failure)
    {

        printf("Error : BMP header data is not Skipped\n");
        return e_failure;
    }
    if (decode_magicstring_length(decInfo) == e_failure)
    {
        printf("Error : Decode magic string length failed\n");
        return e_failure;
    }
    if (decode_magic_string(decInfo) == e_failure)
    {

        printf("Error : Decode Magic string failed\n");
        return e_failure;
    }
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {

        printf("Error : Decode Secret file extn size failed \n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo) == e_failure)
    {
   
        printf("Error : Decode Secret file extn  failed \n");
        return e_failure;
    }
    if(decode_secret_file_size(decInfo) == e_failure)
    {

        printf("Error : Decode Secret file size failed \n");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo)== e_failure)
    {

        printf("Error : Decode Secret file extn size failed \n");
        return e_failure;
    }

    return e_success;
}
*/

Status do_decoding(DecodeInfo *decInfo)
{
    if (openfiles(decInfo) == e_success)
    {
         printf("Info: Done\n");
    } 
    if (Skip_bmp_header(decInfo->fptr_stego_image) == e_success)
    {
         printf("Info: Done\n");
        if (decode_magicstring_length(decInfo) == e_success)
        {
             printf("Info: Done\n");
            if (decode_magic_string(decInfo) == e_success)
            {
                 printf("Info: Done\n");
                if (decode_secret_file_extn_size(decInfo) == e_success)
                {
                     printf("Info: Done\n");
                    if (decode_secret_file_extn(decInfo) == e_success)
                    {
                         printf("Info: Done\n");
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                                printf("Info: Done\n");
                               if(decode_secret_file_data(decInfo) == e_success)
                               {
                                    printf("Info: Done\n");
                               }
                        }
    
                    }
                }
            }
        }
    }

    return e_success;
}
