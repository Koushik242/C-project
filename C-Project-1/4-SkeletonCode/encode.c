#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>


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
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

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

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
	if(strstr (argv[2],".bmp") != NULL)
	{
		if (!strcmp(strstr(argv[2],".bmp"), ".bmp"))
		{
			encInfo->src_image_fname = argv[2];
		}
		else
		{
			printf("argv[2] is not a .bmp extension\n");
			return e_failure;
		}
	}
	else
	{
		printf("argv[2] is not a .bmp extension\n");
		return e_failure;
	}

	if(strstr (argv[3],"."))
	{
		encInfo->secret_fname = argv[3];
	}
	else
	{
		printf("argv[3] is not a file\n");
		return e_failure;
	}

	if(argv[4] != NULL)
	{
		if(strstr(argv[4],".bmp") != NULL)
		{
			if(!strcmp(strstr (argv[4],".bmp"),".bmp"))
			{
				encInfo->stego_image_fname = argv[4];
			}
			else
			{
				printf("argv[4] is not a .bmp file\n");
				return e_failure;
			}
		}
		else
		{
			printf("argv[4] is not a .bmp file\n");
			return e_failure;
		}
	}
	else
	{
		encInfo->stego_image_fname = "output.bmp";
	}
	return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
	if(open_files(encInfo) == e_success)
	{
		printf("File opening successfull\n");
		
		printf("Enter Magic String \n");
		scanf("%s",encInfo->magic_string);


		if(check_capacity(encInfo) == e_success)
		{
			printf("Check Capacity is successfull\n");
			
			 if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
			 {
				 printf("Header Copied is successfull\n");

				if(encode_secret_file_size(encInfo->magic_string_len,encInfo) == e_success)
				{
					printf("Encoding Secret file size is successfull\n");
				
				 if(encode_magic_string(encInfo->magic_string,encInfo) == e_success)
				 {
					 printf("Enoding magic string is successfull\n");

		
					 if(encode_secret_file_size(encInfo->secret_extn_size,encInfo) == e_success)
					 {
						 printf("Encoding Secret file size is successfull\n");

						if( encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
						{
							printf("Encoding Secret file extension is successfull\n");

					 		if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
							{
								printf("Encoding Secret file size is successfull\n");

								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("Encoding Secret file data is successfull\n");

								    if(copy_remaining_img_data(encInfo-> fptr_src_image,encInfo->fptr_stego_image) == e_success)
									{
										printf("Copying remaining image data is successfull\n");
									}
									else
									{
										printf("Copying remaining image data is failed\n");
										return e_failure;
									}
								}
								else
								{
									printf("Encode Secret file data is failed\n");
									return e_failure;
								}
							}
							else
							{
								printf("Encoding Secret file data size is failed\n");
								return e_failure;
							}

						}
						else
						{
							printf("Encoding secret file extension is failed\n");
							return e_failure;
						}

					 }
					 else
					 {
						 printf("Encoding Secret file size is failed\n");
						 return e_failure;
					 }
				 }
				 else
				 {
					 printf("Encoding magic string is failed\n");
					 return e_failure;
				 }

			 
			}
			else
			{
				printf("Encoding magic string length is failed\n");
				return e_failure;
			}
			 }
				
			 else
			 {
				 printf("Copying Header is failed\n");
				 return e_failure;
			 }

		}
		else
		{
			printf("Check Capacity is failed\n");
			return e_failure;
		}

	}
	else
	{
		printf("Opening Files is failed\n");
		return e_failure;
	}
	return e_success;

}

Status check_capacity (EncodeInfo *encInfo )
{
	encInfo -> image_capacity = get_image_size_for_bmp (encInfo -> fptr_src_image);


	encInfo->magic_string_len = strlen(encInfo->magic_string);
    
	
	strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname,"."));

	
	encInfo->secret_extn_size = strlen(strstr(encInfo->secret_fname,"."));
	

	encInfo->size_secret_file = get_file_size(encInfo -> fptr_secret);

	if(encInfo->image_capacity > 54 + 32 + (encInfo->magic_string_len * 8) + 32 + (encInfo -> secret_extn_size * 8) + 32 +(encInfo -> size_secret_file * 8))
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

uint get_file_size(FILE *fptr)
{
	fseek(fptr,0,SEEK_END);
	return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	char bmp_header[54];

	fseek(fptr_src_image, 0, SEEK_SET);
	fread(bmp_header, 1,54, fptr_src_image);
	fwrite(bmp_header, 1,54, fptr_dest_image);

	if(54 == get_file_size(fptr_dest_image))
	{

		return e_success;
	}
	else
	{
		return e_failure;
	}
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{

	
	if(encode_data_to_image(encInfo->magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		printf("Encoding Data to image is sucessfull\n");
		return e_success;
	}
	else
	{
		printf("Encoding Data to image is failed\n");
		return e_failure;
	}
}


Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	
	char image_buffer[8];

	for(int i=0; i<size; i++)
	{
		fread(image_buffer,1,8,fptr_src_image);
		encode_byte_to_lsb(data[i],image_buffer);
		fwrite(image_buffer,1,8,fptr_stego_image);
	}
	return e_success;

}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
	int index = 0;
	for(int i = 7 ; i >= 0 ; i--)
	{
		image_buffer[index] = (image_buffer[index] & ~1) | ((data & (1<<i)) >> i);
		index++;
	}
	return e_success;

}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
	char ext_buffer[32];
	int index=0;

        fread(ext_buffer,1,32,encInfo->fptr_src_image);

	for(int i=31 ; i>=0 ; i--)
	{
		ext_buffer[index] = (ext_buffer[index] & ~1) | ((file_size & (1<<i)) >> i);
		index++;
	}
	fwrite(ext_buffer,1,32,encInfo->fptr_stego_image);
	return e_success;

}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	
	if(encode_data_to_image(encInfo->extn_secret_file,encInfo->secret_extn_size,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char data_buff[encInfo->size_secret_file];
	fseek(encInfo->fptr_secret, 0, SEEK_SET); 

	
	fread(data_buff,1,encInfo->size_secret_file,encInfo->fptr_secret);
	
	if(encode_data_to_image(data_buff,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	int output_file_size = get_file_size(fptr_dest);
	int src_file_size = get_file_size(fptr_src);
	int size = src_file_size-output_file_size;

	fseek(fptr_src,output_file_size,SEEK_SET);

	char rem_buff[size];
	fread(rem_buff,1,size,fptr_src);
	fwrite(rem_buff,1,size,fptr_dest);

	return e_success;
}
	



