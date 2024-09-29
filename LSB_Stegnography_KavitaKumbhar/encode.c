#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

Status encode_int_to_lsb(int data, char *imag_buffer)
{
    int j = 0;
    for (int i = 31; i >= 0, j < 32; i--)
    {
        unsigned data_bit = (unsigned)(data & (1 << i)) >> i;
        imag_buffer[j] = imag_buffer[j] & 0xfe;
        imag_buffer[j] = imag_buffer[j] | data_bit;
        j++;
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *imag_buffer)
{
    int j = 0;
    for (int i = 7; i >= 0, j < 8; i--)
    {
        unsigned char data_bit = (unsigned)(data & (1 << i)) >> i;

        imag_buffer[j] = imag_buffer[j] & 0xfe;
        imag_buffer[j] = imag_buffer[j] | data_bit;
        j++;
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    rewind(fptr_image);
    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    printf("Info: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    /* Check Number of Arguments are 4 or 5 */
    if (argc == 4 || argc == 5)
    {
        /*check for valid source file. It must be of .bmp extn */
        if (strstr(argv[2], ".bmp") == NULL)
        {
            return e_failure;
        }
        char *tempstr = strchr(argv[3], '.');
        if (tempstr == NULL)
        {
            return e_failure;
        }
        else
        {
            if (strlen(tempstr) < 1)
            {
                return e_failure;
            }
        }
        if (argc != 5)
        {
            encInfo->stego_image_fname = "stego.bmp";
        }
        else
        {
            if (strstr(argv[4], ".bmp"))
            {
                encInfo->stego_image_fname = argv[4];
            }
            else
            {
                printf("INFO: Output file not mentioned.Creating stego.bmp as default\n");
                encInfo->stego_image_fname = "stego.bmp";
            }
        }

        encInfo->src_image_fname = argv[2];
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, tempstr);

        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("Error : Files are not opened\n");
        return e_failure;
    }
    if (read_magic_string(encInfo) == e_failure)
    {
        printf("Error : magic string\n");
        return e_failure;
    }
    if (check_capacity(encInfo) == e_failure)
    {
        printf("Error : BMP file size is less than total number of bytes encoding Excluding header\nCheck capacity failed\n");
        return e_failure;
    }
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error : Failed to copy BMP file Header\n");
        return e_failure;
    }

    if (encode_magicstring_length(encInfo->magic_string_len, encInfo) == e_failure)
    {

        printf("Encoding magic string length failed\n");
        return e_failure;
    }
    if (encode_magic_string(encInfo->magic_string, encInfo) == e_failure)
    {
        printf("Error : Encode magic String failed\n");
        return e_failure;
    }
    if (encode_secret_file_extn_size(encInfo) == e_failure)
    {

        printf("Error : Encoding secret file extension size failed\n");
        return e_failure;
    }
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {

        printf("Error : Encoding secret file extension Failed\n");
        return e_failure;
    }
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {

        printf("Error : Encoding secret file size Failed\n");
        return e_failure;
    }
    if (encode_secret_file_data(encInfo) == e_failure)
    {

        printf("Error : Encoding secret file data Failed\n");
        return e_failure;
    }
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error : Copy remaining data Failed\n");
        return e_failure;
    }

    return e_success;
}
*/

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_success)
    {
        printf("Info: Done\n");
    }
    if (read_magic_string(encInfo) == e_success)
    {
        printf("Info: Done\n");
    }
    printf("Info: ## Encoding Procedure Started ## \n");
    if (check_capacity(encInfo) == e_success)
    {
        printf("Info: Done. found ok\n");
        if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        {
            printf("Info: Done\n");
            if (encode_magicstring_length(encInfo->magic_string_len,encInfo) == e_success)
            {
                printf("Info: Done\n");

                if (encode_magic_string(encInfo->magic_string, encInfo) == e_success)
                {
                    printf("Info: Done\n");
                    if (encode_secret_file_extn_size(encInfo) == e_success)
                    {
                        printf("Info: Done\n");
                        if (encode_secret_file_extn(encInfo->secret_fname, encInfo) == e_success)
                        {
                            printf("Info: Done\n");
                            if(encode_secret_file_size(encInfo -> size_secret_file,encInfo)== e_success)
                            {
                                printf("Info: Done\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                     printf("Info: Done\n");
                                     if(copy_remaining_img_data(encInfo ->fptr_src_image,encInfo -> fptr_stego_image)== e_success)
                                     {
                                        printf("Info: Done\n");

                                        fclose(encInfo -> fptr_stego_image);
                                        fclose(encInfo -> fptr_src_image);
                                        fclose(encInfo -> fptr_secret);
                                     }
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    return e_success;
}


Status read_magic_string(EncodeInfo *encInfo)
{
    printf("Info: Reading Magic string: ");
    scanf(" %s", encInfo->magic_string);
    encInfo->magic_string_len = strlen(encInfo->magic_string);
    if (encInfo->magic_string_len > 10)
    {
        printf("Error : Magic string length should not exceed 10 charecters\n");
        printf("Read again :\n");
        return read_magic_string(encInfo);
    }
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    printf("Info: Checkeing Capacity\n");
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);
    if (encInfo->image_capacity > ((32 * 3 + 16 + (encInfo->size_secret_file * 8) + 54)))
    {
        printf("Info: opened output %s file\n",encInfo->stego_image_fname);
        /*Open output bmp file*/
        // Stego Image file
        encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
        // Do Error handling
        if (encInfo->fptr_stego_image == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

            return e_failure;
        }
        return e_success;
    }
    else
        return e_failure;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("Info: Copying Image Header\n");
    char buffer[54];
  //  fseek(fptr_src_image, 0, SEEK_SET);
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);

    return e_success;
}

Status encode_magicstring_length(int magicstring_length, EncodeInfo *encInfo)
{
    printf("Info: Encoding magic string length\n");
    char buffer[32];
    fseek(encInfo->fptr_src_image, 0, SEEK_CUR);
    fread(buffer, sizeof(buffer), 1, encInfo->fptr_src_image);
    encode_int_to_lsb(magicstring_length, buffer);
    fwrite(buffer, sizeof(buffer), 1, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("Info: Encoding magic string\n");
    int index = 0;
    while (magic_string[index])
    {
        fread(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[index], encInfo->image_data);
        fwrite(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_stego_image);
        index++;
    }
    return e_success;
}
Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    printf("Info: Encoding seccret file extention size\n");
    char buffer[32];
    fread(buffer, sizeof(buffer), 1, encInfo->fptr_src_image);
    encode_int_to_lsb(strlen(encInfo->extn_secret_file), buffer);
    fwrite(buffer, sizeof(buffer), 1, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(const char *file, EncodeInfo *encInfo)
{
    printf("Info: Encoding seccret file extention\n");
    int index = 0;
    while (encInfo->extn_secret_file[index])
    {
        fread(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(encInfo->extn_secret_file[index], encInfo->image_data);
        fwrite(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_stego_image);
        index++;
    }
    return e_success;
}
Status encode_secret_file_size(uint file_size, EncodeInfo *encInfo)
{
    printf("Info: Encoding seccret file size\n");
    char buffer[32];
    fread(buffer, sizeof(buffer), 1, encInfo->fptr_src_image);
    encode_int_to_lsb(file_size, buffer);
    fwrite(buffer, sizeof(buffer), 1, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Info: Encoding seccret file data\n");
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    char str[encInfo->size_secret_file];
    // fscanf(encInfo ->fptr_secret,"%[^-1]",str);
    fread(str, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    int i = 0;
    while (i < encInfo->size_secret_file)
    {
        fread(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(str[i], encInfo->image_data);
        fwrite(encInfo->image_data, MAX_IMAGE_BUF_SIZE, 1, encInfo->fptr_stego_image);
        i++;
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("Info: Copying Left over Data\n");
    char ch;
    ferror(fptr_src);
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
