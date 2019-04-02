#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniz.h"


/*
These plotting support files use "zlib" compression. After decompression you get an ASCII file that is more or less "human readable". 

The compressed file starts with a 59 bytes long header that has nothing to do with this type of compression - it makes these files more or less proprietary. The header contains some identification strings - that is the easy part. The only obscure things remain bytes 48..55 and 56..59. They seem to be numbers related to the content (CRC?). To decompress, this whole header can be skipped. To test this, just crop the first 59 bytes from the file with an editor and use a zlib tool to decompress the "decapitated" file. 

To compress, a header should be added, containing the strings like "PIAFILEVERSION..." and the numbers have to be written in it. But what numbers? I have found that AutoCAD seems to read and interpret the files without problems if the numbers are large enough - it just takes a little longer than with the calculated numbers. In my experimental tool I use this (hex) sequence FF FF FF FF FF FF FF 00 FF FF FF 00. 

Here is an on line CTB viewer, made by Ian, inspired by the above info: http://www.xcad.ch/tests/ctbviewer.php/ 
*/

char * load_file(char *path, long *fsize){
	FILE *file;
	
	*fsize = 0;
	file = fopen(path, "rb");
	if(file == NULL){
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	*fsize = ftell(file); /* size of file*/
	fseek(file, 0, SEEK_SET);  //same as rewind(f);
	//printf("file size = %d\n", fsize);
	
	char *buf = malloc(*fsize + 1);
	if (!buf){
		*fsize = 0;
		fclose(file);
		return NULL;
	}
	fread(buf, *fsize, 1, file);
	fclose(file);
	buf[*fsize] = 0;
	return buf;
}

int main(int argc, char *argv[]){
	long comp_len = 0;
	char *comp = load_file("test.ctb", &comp_len);
	
	unsigned char *dest = calloc(5000000, sizeof(char));
	long dest_len = 5000000;
	int res = uncompress(dest, &dest_len, (const unsigned char *)comp + 60, (mz_ulong)comp_len - 60);
	
	if(res == MZ_OK){
		FILE *fp;
		fp = fopen( "file.txt" , "w" );
		fwrite(dest , 1 , dest_len, fp );

		fclose(fp);
	}
	
	free(comp);
	free(dest);
	
	return 0;
}