#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/* Check operation type */
OperationType check_operation_type(int argc, char *argv[]);
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    OperationType t = check_operation_type(argc, argv);
    if (t == e_encode)
    {
        if (read_and_validate_encode_args(argc, argv, &encInfo) == e_failure)
        {
           printf("Encoding : ./a.out -e <.bmp file> <.any valid extension file> <.bmp file>[output file]\n");
            return 0;
        }
        if (do_encoding(&encInfo) == e_failure)
        {
            printf("Encoding failed..........\n");
            return 0;
        }
        else
        {
            printf("## Encoding Done successfully ##\n");
        }
    }
    else if (t == e_decode)
    {
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_failure)
        {
            printf("decoding : ./a.out -d <.bmp file> <.any valid extension file> [output file]\n");
            return 0;
        }
        if (do_decoding(&decInfo) == e_failure)
        {
            printf("decodingfailed..........\n");
            return 0;
        }
        else
        {
             printf("## Decoding Done successfully ##\n");
        }
    }
    else
    {
        printf("Encoding : ./a.out -e <.bmp file> <.any valid extension file> <.bmp file>[output file]\nDecoding : ./a.out -d <.bmp file> <.extension file>[output file]\n");
        printf("Error: execution terminated\n");
    }

    return 0;
}
OperationType check_operation_type(int argc, char *argv[])
{
    if (argc <= 1)
    {
        return e_unsupported;
    }
    if (!strcmp(argv[1], "-e"))
    {
        return e_encode;
    }
    else if (!strcmp(argv[1], "-d"))
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}