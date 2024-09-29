#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 10

typedef struct _DecodeInfo
{
    /*magic String*/
    uint magic_string_len;
    char magicstring[20];

    /* Decoded file Info */
    char *stego_imag_fname;
    FILE *fptr_stego_image;
    uint file_extn_size;
    uint sec_extn_size;
    uint sec_file_size;
    char file_extn[MAX_FILE_SUFFIX];

    /* Stego Image Info */
    char output_file_fname[30];
    FILE *output_fname;
    char imag_data[MAX_IMAGE_BUF_SIZE];

}DecodeInfo;

/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Perform the decode operation*/
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status openfiles(DecodeInfo *decInfo);

/* Skip bmp image header */
Status Skip_bmp_header(FILE *fptr_stego_image);

/* Store Magic String */
Status decode_magicstring_length(DecodeInfo * decInfo);
Status decode_magic_string( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn_size(DecodeInfo * decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
uint decode_lsb_to_int(char * buffer);

/* Encode a byte into LSB of image data array */
char decode_lsb_to_byte(char *buffer);

#endif
