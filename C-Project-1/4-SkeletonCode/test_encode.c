#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"decode.h"


int main(int argc, char *argv[])
{

	EncodeInfo encInfo;
	DecodeInfo decInfo;
	
        int res = check_operation_type(argv);
	if(res == e_encode)
	{
		printf("Selected encoding\n");
		if(argc == 4 || argc == 5)
		{
			if(read_and_validate_encode_args(argv,&encInfo) == e_success)
			{
				printf("Read and validation is done\n");

				if(do_encoding(&encInfo) == e_success)
				{
					printf("Do Encoding is Successful\n");
				}
				else
				{
					printf("Do Encoding is failed\n");
				}	
			}

			else
			{
				printf("Read and validation is Failed\n");
			}
		}
		else
		{
			printf("Please pass the Arguments\n");
		}

	}

	else if(res == e_decode)
	{
		printf("Selected decoding\n");

		if(argc == 3 || argc == 4)
		{
			if(read_and_validate_decode_args(argv,&decInfo) == e_success)
			{
				printf("Read and Validate decode args is Successfull\n");
				if(do_decoding(&decInfo) == e_success)
				{
					printf("Do Decoding is sucessfull\n");
				}
				else
				{
					printf("Do Decoding is failed\n");
				}	
			}
			else
			{
				printf("Read and Validate Decode Arguments is failed\n");
			}
		}

		else
		{
			printf("Please Pass the arguments\n");
		}

	}

	else 
	{
		printf("Error : For Encoding : ./a.out -e beautiful.bmp secret.txt\n");

		printf("Error : For decoding : ./a.out -d output.bmp\n");
	}

    return 0;
}


OperationType check_operation_type (char *argv[])
{
	if(argv[1] != NULL)
	{
		
		if(!strcmp (argv[1] ,"-e" ))
		{
			return e_encode;
		}
		else if(!strcmp (argv[1],"-d"))
		{
			return e_decode;
		}
		else
		{
			return e_unsupported;
		}
	}
	else
	{
		return e_unsupported;
	}
}


