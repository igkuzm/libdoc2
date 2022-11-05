/**
 * File              : test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 03.11.2022
 * Last Modified Date: 05.11.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "ole2.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "property_set.h"
#include "cprecode.h"
#include <iconv.h>


static char* unicode_decode_iconv(const char *s, size_t len, iconv_t ic) {
    char* outbuf = 0;

    if(s && len && ic)
    {
        size_t outlenleft = len;
        int outlen = len;
        size_t inlenleft = len;
        const char* src_ptr = s;
        char* out_ptr = 0;

        size_t st; 
        outbuf = malloc(outlen + 1);

		if(outbuf)
        {
            out_ptr = outbuf;
            while(inlenleft)
            {
                st = iconv(ic, (char **)&src_ptr, &inlenleft, (char **)&out_ptr,(size_t *) &outlenleft);
                if(st == (size_t)(-1))
                {
                    if(errno == E2BIG)
                    {
                        size_t diff = out_ptr - outbuf;
                        outlen += inlenleft;
                        outlenleft += inlenleft;
                        outbuf = realloc(outbuf, outlen + 1);
                        if(!outbuf)
                        {
                            break;
                        }
                        out_ptr = outbuf + diff;
                    }
                    else
                    {
                        free(outbuf), outbuf = NULL;
                        break;
                    }
                }
            }
        }
        outlen -= outlenleft;

        if(outbuf)
        {
            outbuf[outlen] = 0;
        }
    }
    return outbuf;
}

int prop_cb(void * user_data, uint32_t propid, uint32_t dwType, uint32_t * value){
	char * str = NULL;
	
	/*printf("longVal=%llx\n", *(uint64_t *)value);*/
	/*printf("s[%u]=%s\n", *(uint32_t  *)value, (char *)value + 4);*/

	if (dwType == 30){
		char buf[BUFSIZ];
		strncpy(buf, (char*)value + 4, BUFSIZ);
		//str = cprecode(buf, CPRECODE_CP1251, CPRECODE_UTF8);
		//str = buf;
		iconv_t ic = iconv_open("UTF-8", "CP1251");
		str = unicode_decode_iconv(buf, strlen(buf), ic);
	}
	else if (dwType == 2){
		char buf[BUFSIZ];
		sprintf(buf, "%d", *(uint16_t*)value);
		str = buf;
	}
	else if (dwType == 3){
		char buf[BUFSIZ];
		sprintf(buf, "%d", *(uint32_t*)value);
		str = buf;
	}

	printf("PROP id: %d, type: %d, value: %s\n", propid, dwType, str);
	return 0;
};

int main(int argc, char *argv[])
{
	printf("START TEST...\n");

	ole2_t ole2 = ole2_open("1.doc");
	if (!ole2)
		printf("error wcbff init\n");

	ole2_dir_t dir = ole2_get_dir(ole2, "\005SummaryInformation");
	if (!dir)
		printf("NOT FOUND\n");

	printf("DIR TYPE: %d\n", dir->dir._mse);

	printf("DIR NAME: %s\n", ole2_dir_name(dir));

	printf("SIZE: %u\n", dir->dir._ulSize);
	printf("START: %x\n", dir->dir._sectStart);

	FILE * si = ole2_dir_stream(dir);
	property_set_get(si, NULL, prop_cb);
	
	getchar();
	printf("press any key to finish\n");
	fclose(si);
	return 0;
}
