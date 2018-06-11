#include "aaa.h"

char * readline(FILE* f)
{
    static char line[100];
    char c;
    int len = 0;

    while((c = fgetc(f) ) != EOF && c != '\n' && c != '\r')
    {
        line[len++] = c;
    }
	line[len] = '\0';
    return line;
}

int get_word_from_buff(char * buff, char * word)
{
	char c;
	int i = 0, len, j=0;
	
	while(buff[i] == ' ')
		i++;
	while((buff[i] >= 'a' && buff[i] <= 'f') || 
		(buff[i] >= 'A' && buff[i] <= 'F') || 
		(buff[i] >= '0' && buff[i] <= '9')){
		word[j++] = buff[i++];
	}
	word[j] = '\0';
	return i;
}

char get_halfbyte(unsigned char * buff)
{

}


short get_address(unsigned char * buff)
{

}

int xtoi(char * buff)
{
	int val = 0;
	int i=0, a = 0;

	while(1){
		switch(buff[i++]){
		case '0':
			a = 0;
			break;
		case '1':
			a = 1;
			break;
		case '2':
			a = 2;
			break;
		case '3':
			a = 3;
			break;
		case '4':
			a = 4;
			break;
		case '5':
			a = 5;
			break;
		case '6':
			a = 6;
			break;
		case '7':
			a = 7;
			break;
		case '8':
			a = 8;
			break;
		case '9':
			a = 9;
			break;
		case 'a':
		case 'A':
			a = 10;
			break;
		case 'b':
		case 'B':
			a = 11;
			break;
		case 'c':
		case 'C':
			a = 12;
			break;
		case 'd':
		case 'D':
			a = 13;
			break;
		case 'e':
		case 'E':
			a = 14;
			break;
		case 'f':
		case 'F':
			a = 15;
			break;
		default:
			a = -1;
			break;
		}
		if(a  >= 0){
			val <<= 4;
			val += a;
		}else{
			break;
		}
	}
	return val;
}

int find_sectors(FILE *src, FILE * tag)
{
	char *buff;
	char wp[8];
	char bak[100], dt, stf[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	int blen, wlen, data, pos, datalen = 0, wpos;
	
	int sectors = 0, sector_start = 0;
	int address;

	long adw;

	fseek(src, 0, 0);
	fseek(tag, 0, 0);

	while(1){
		buff = readline(src);
		blen = strlen(buff);
		if(blen <= 0 || (blen ==1 && buff[0] == 'q')){
			fseek(tag, sector_start + 8, SEEK_SET);
			fwrite(&datalen, 2, 1, tag);
			printf("len=%04x\r\n", datalen);
			break;
		}else{
//			strcpy(bak, buff);

			if(buff[0] == '@'){
				////////////////////////
				// last sector length
				////////////////////////
				int alglen;

				adw = ftell(tag);
				fseek(tag, sector_start + 8, SEEK_SET);
				fwrite(&datalen, 2, 1, tag);
				if(sectors)
					printf("len=%04x\r\n", datalen);
				//////////////////////////////////
				// current sector start address
				//////////////////////////////////
				sector_start += (datalen + 16);		//align by 16 byte
				if(sector_start & 0x0f){
					sector_start &= ~0x0f;
					sector_start += 0x10;
				}

				pos = get_word_from_buff(buff + 1, wp);
				address = xtoi(wp);
				if(address < 0xffff){
					fseek(tag, sector_start, SEEK_SET);
					adw = ftell(tag);
					fwrite("SECT", 4, 1, tag);
					fwrite(&sectors, 2, 1, tag);
					fwrite(&address, 2, 1, tag);
					printf("Sector %02d Start %04x\t\t", sectors, address);
					fseek(tag, sector_start + 16, SEEK_SET);
					sectors++;
					datalen = 0;
					
				}
			}else{
				pos = 0;
				while(1){
					wlen = get_word_from_buff(buff, wp);
					if(!strlen(wp))
						break;
					buff += wlen;
					data = xtoi(wp);
					dt = data & 0xff;
					adw = ftell(tag);
					fwrite(&dt, 1, 1, tag);
					datalen++;
				}
				
			}
			
		}
	}

	fseek(tag, 0, 0);
	//sectors --;
	if(sectors > 0){
		fwrite(&sectors, 2, 1, tag);
	}
	return sectors;
}
