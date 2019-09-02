#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* AAC原始码流（又称为“裸流”）是由一个一个的ADTS frame组成的。
* 其中每个ADTS frame之间通过syncword（同步字）进行分隔。同步字为0xFFF（二进制“111111111111”）。
* AAC码流解析的步骤就是首先从码流中搜索0x0FFF，分离出ADTS frame；然后再分析ADTS frame的首部各个字段。
*/

int getADTSframe(unsigned char *buffer, int buf_size, unsigned char *data, int *data_size) {
    int size = 0;

    if (!buffer || !data || !data_size) {
        return -1;
    }

    while (1) {
        if (buf_size < 7) {
            return -1;
        }
        //Sync words
        if ((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0)) {
            size |= ((buffer[3] & 0x03) << 11);     //high 2 bit
            size |= buffer[4] << 3;                //middle 8 bit
            size |= ((buffer[5] & 0xe0) >> 5);        //low 3bit
            break;
        }
        --buf_size;
        ++buffer;
    }

    if (buf_size < size) {
        return 1;
    }

    memcpy(data, buffer, size);
    *data_size = size;

    return 0;
}

int simplest_aac_parser(char *url) {
    int data_size = 0;
    int size = 0;
    int cnt = 0;
    int offset = 0;

    //FILE *myout=fopen("output_log.txt","wb+");
    FILE *myout = stdout;

    unsigned char *aacframe = (unsigned char *) malloc(1024 * 5);
    unsigned char *aacbuffer = (unsigned char *) malloc(1024 * 1024);

    FILE *ifile = fopen(url, "rb");
    if (!ifile) {
        printf("Open file error");
        return -1;
    }

    printf("-----+- ADTS Frame Table -+------+\n");
    printf(" NUM | Profile | Frequency| Size |\n");
    printf("-----+---------+----------+------+\n");

    while (!feof(ifile)) {
        data_size = fread(aacbuffer + offset, 1, 1024 * 1024 - offset, ifile);
        unsigned char *input_data = aacbuffer;

        while (1) {
            int ret = getADTSframe(input_data, data_size, aacframe, &size);
            if (ret == -1) {
                break;
            } else if (ret == 1) {
                memcpy(aacbuffer, input_data, data_size);
                offset = data_size;
                break;
            }

            char profile_str[10] = {0};
            char frequence_str[10] = {0};

            unsigned char profile = aacframe[2] & 0xC0;
            profile = profile >> 6;
            switch (profile) {
                case 0:
                    sprintf(profile_str, "Main");
                    break;
                case 1:
                    sprintf(profile_str, "LC");
                    break;
                case 2:
                    sprintf(profile_str, "SSR");
                    break;
                default:
                    sprintf(profile_str, "unknown");
                    break;
            }

            unsigned char sampling_frequency_index = aacframe[2] & 0x3C;
            sampling_frequency_index = sampling_frequency_index >> 2;
            switch (sampling_frequency_index) {
                case 0:
                    sprintf(frequence_str, "96000Hz");
                    break;
                case 1:
                    sprintf(frequence_str, "88200Hz");
                    break;
                case 2:
                    sprintf(frequence_str, "64000Hz");
                    break;
                case 3:
                    sprintf(frequence_str, "48000Hz");
                    break;
                case 4:
                    sprintf(frequence_str, "44100Hz");
                    break;
                case 5:
                    sprintf(frequence_str, "32000Hz");
                    break;
                case 6:
                    sprintf(frequence_str, "24000Hz");
                    break;
                case 7:
                    sprintf(frequence_str, "22050Hz");
                    break;
                case 8:
                    sprintf(frequence_str, "16000Hz");
                    break;
                case 9:
                    sprintf(frequence_str, "12000Hz");
                    break;
                case 10:
                    sprintf(frequence_str, "11025Hz");
                    break;
                case 11:
                    sprintf(frequence_str, "8000Hz");
                    break;
                default:
                    sprintf(frequence_str, "unknown");
                    break;
            }


            fprintf(myout, "%5d| %8s|  %8s| %5d|\n", cnt, profile_str, frequence_str, size);
            data_size -= size;
            input_data += size;
            cnt++;
        }

    }
    fclose(ifile);
    free(aacbuffer);
    free(aacframe);

    return 0;
}

// 代码中的size是ADTS数据的长度，这个长度的值存储在ADTS头文件7个字节（58位）中的第31-43位中，共13位，
// 因为一个字节占8位，所以这13位的数据分布在三个字节中，也就是第四个字节的最后两位、第五个字节的八位、第六个字节的前三位