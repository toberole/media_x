#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * 分离PCM16LE双声道音频采样数据的左声道和右声道
 * 16LE: “16”代表采样位数是16bit。由于1Byte=8bit，所以一个声道的一个采样值占用2Byte。
 * “LE”代表Little Endian，代表2 Byte采样值的存储方式为高位存在高地址中。
 */
int simplest_pcm16le_split(char *url) {
    FILE *fp = fopen(url, "rb+");

    FILE *fp1 = fopen("output_l.pcm", "wb+");
    FILE *fp2 = fopen("output_r.pcm", "wb+");

    unsigned char *sample = (unsigned char *) malloc(4);

    while (!feof(fp)) {
        fread(sample, 1, 4, fp);
        // L
        fwrite(sample, 1, 2, fp1);
        // R
        fwrite(sample + 2, 1, 2, fp2);
    }

    free(sample);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    return 0;
}

/*
 * 将PCM16LE双声道音频采样数据中左声道的音量降一半
 * 读出左声道的2 Byte的取样值之后，将其当成了C语言中的一个short类型的变量。
 * 将该数值除以2之后写回到了PCM文件中
 */
/**
 * Halve volume of Left channel of 16LE PCM file
 * @param url  Location of PCM file.
 */
int simplest_pcm16le_halfvolumeleft(char *url) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("output_halfleft.pcm", "wb+");

    int cnt = 0;

    unsigned char *sample = (unsigned char *) malloc(4);

    while (!feof(fp)) {
        short *samplenum = NULL;
        fread(sample, 1, 4, fp);

        samplenum = (short *) sample;
        *samplenum = *samplenum / 2;
        //L
        fwrite(sample, 1, 2, fp1);
        //R
        fwrite(sample + 2, 1, 2, fp1);

        cnt++;
    }
    printf("Sample Cnt:%d\n", cnt);

    free(sample);
    fclose(fp);
    fclose(fp1);
    return 0;
}

/**
 * 将PCM16LE双声道音频采样数据的声音速度提高一倍
 * Re-sample to double the speed of 16LE PCM file
 * @param url  Location of PCM file.
 *
 * 本程序只采样了每个声道奇数点的样值。处理完成后，原本22秒左右的音频变成了11秒左右。音频的播放速度提高了2倍，音频的音调也变高了很多。
 */
int simplest_pcm16le_doublespeed(char *url) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("output_doublespeed.pcm", "wb+");

    int cnt = 0;

    unsigned char *sample = (unsigned char *) malloc(4);

    while (!feof(fp)) {

        fread(sample, 1, 4, fp);

        if (cnt % 2 != 0) {
            //L
            fwrite(sample, 1, 2, fp1);
            //R
            fwrite(sample + 2, 1, 2, fp1);
        }
        cnt++;
    }
    printf("Sample Cnt:%d\n", cnt);

    free(sample);
    fclose(fp);
    fclose(fp1);
    return 0;
}

/**
 * 将PCM16LE双声道音频采样数据转换为PCM8音频采样数据
 * Convert PCM-16 data to PCM-8 data.
 * @param url  Location of PCM file.
 *
 * PCM16LE格式的采样数据的取值范围是-32768到32767，而PCM8格式的采样数据的取值范围是0到255。所以PCM16LE转换到PCM8需要经过两个步骤：第一步是将-32768到32767的16bit有符号数值转换为-128到127的8bit有符号数值，第二步是将-128到127的8bit有符号数值转换为0到255的8bit无符号数值。在本程序中，16bit采样数据是通过short类型变量存储的，而8bit采样数据是通过unsigned char类型存储的。
 */
int simplest_pcm16le_to_pcm8(char *url) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("output_8.pcm", "wb+");

    int cnt = 0;

    unsigned char *sample = (unsigned char *) malloc(4);

    while (!feof(fp)) {

        short *samplenum16 = NULL;
        char samplenum8 = 0;
        unsigned char samplenum8_u = 0;
        fread(sample, 1, 4, fp);
        //(-32768-32767)
        samplenum16 = (short *) sample;
        samplenum8 = (*samplenum16) >> 8;
        //(0-255)
        samplenum8_u = samplenum8 + 128;
        //L
        fwrite(&samplenum8_u, 1, 1, fp1);

        samplenum16 = (short *) (sample + 2);
        samplenum8 = (*samplenum16) >> 8;
        samplenum8_u = samplenum8 + 128;
        //R
        fwrite(&samplenum8_u, 1, 1, fp1);
        cnt++;
    }
    printf("Sample Cnt:%d\n", cnt);

    free(sample);
    fclose(fp);
    fclose(fp1);
    return 0;
}

/**
 * 将从PCM16LE单声道音频采样数据中截取一部分数据
 * Cut a 16LE PCM single channel file.
 * @param url        Location of PCM file.
 * @param start_num  start point
 * @param dur_num    how much point to cut
 *
 * 本程序可以从PCM数据中选取一段采样值保存下来，并且输出这些采样值的数值。
 * 把单声道PCM16LE格式的“drum.pcm”中从2360点开始的120点的数据保存成output_cut.pcm文件
 */
int simplest_pcm16le_cut_singlechannel(char *url, int start_num, int dur_num) {
    FILE *fp = fopen(url, "rb+");
    FILE *fp1 = fopen("output_cut.pcm", "wb+");
    FILE *fp_stat = fopen("output_cut.txt", "wb+");

    unsigned char *sample = (unsigned char *) malloc(2);

    int cnt = 0;
    while (!feof(fp)) {
        fread(sample, 1, 2, fp);
        if (cnt > start_num && cnt <= (start_num + dur_num)) {
            fwrite(sample, 1, 2, fp1);

            short samplenum = sample[1];
            samplenum = samplenum * 256;
            samplenum = samplenum + sample[0];

            fprintf(fp_stat, "%6d,", samplenum);
            if (cnt % 10 == 0)
                fprintf(fp_stat, "\n", samplenum);
        }
        cnt++;
    }

    free(sample);
    fclose(fp);
    fclose(fp1);
    fclose(fp_stat);
    return 0;
}

/*
 * 将PCM16LE双声道音频采样数据转换为WAVE格式音频数据
 * WAVE格式音频（扩展名为“.wav”）是Windows系统中最常见的一种音频。
 * 该格式的实质就是在PCM文件的前面加了一个文件头。
 * 本程序的函数就可以通过在PCM文件前面加一个WAVE文件头从而封装为WAVE格式音频。
 */
/*
 * WAVE文件是一种RIFF格式的文件。其基本块名称是“WAVE”，其中包含了两个子块“fmt”和“data”。
 * 从编程的角度简单说来就是由WAVE_HEADER、WAVE_FMT、WAVE_DATA、采样数据共4个部分组成。
 * 它的结构:WAVE_HEADER、WAVE_FMT、WAVE_DATA、PCM数据
 */
/**
 * Convert PCM16LE raw data to WAVE format
 * @param pcmpath      Input PCM file.
 * @param channels     Channel number of PCM file.
 * @param sample_rate  Sample rate of PCM file.
 * @param wavepath     Output WAVE file.
 */
int
simplest_pcm16le_to_wave(const char *pcmpath, int channels, int sample_rate, const char *wavepath) {

    typedef struct WAVE_HEADER {
        char fccID[4];
        unsigned long dwSize;
        char fccType[4];
    } WAVE_HEADER;

    typedef struct WAVE_FMT {
        char fccID[4];
        unsigned long dwSize;
        unsigned short wFormatTag;
        unsigned short wChannels;
        unsigned long dwSamplesPerSec;
        unsigned long dwAvgBytesPerSec;
        unsigned short wBlockAlign;
        unsigned short uiBitsPerSample;
    } WAVE_FMT;

    typedef struct WAVE_DATA {
        char fccID[4];
        unsigned long dwSize;
    } WAVE_DATA;


    if (channels == 0 || sample_rate == 0) {
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 16;

    WAVE_HEADER pcmHEADER;
    WAVE_FMT pcmFMT;
    WAVE_DATA pcmDATA;

    unsigned short m_pcmData;
    FILE *fp, *fpout;

    fp = fopen(pcmpath, "rb");
    if (fp == NULL) {
        printf("open pcm file error\n");
        return -1;
    }
    fpout = fopen(wavepath, "wb+");
    if (fpout == NULL) {
        printf("create wav file error\n");
        return -1;
    }
    //WAVE_HEADER
    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
    fseek(fpout, sizeof(WAVE_HEADER), 1);
    //WAVE_FMT
    pcmFMT.dwSamplesPerSec = sample_rate;
    pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec * sizeof(m_pcmData);
    pcmFMT.uiBitsPerSample = bits;
    memcpy(pcmFMT.fccID, "fmt ", strlen("fmt "));
    pcmFMT.dwSize = 16;
    pcmFMT.wBlockAlign = 2;
    pcmFMT.wChannels = channels;
    pcmFMT.wFormatTag = 1;

    fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fpout);

    //WAVE_DATA;
    memcpy(pcmDATA.fccID, "data", strlen("data"));
    pcmDATA.dwSize = 0;
    fseek(fpout, sizeof(WAVE_DATA), SEEK_CUR);

    fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    while (!feof(fp)) {
        pcmDATA.dwSize += 2;
        fwrite(&m_pcmData, sizeof(unsigned short), 1, fpout);
        fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    }

    pcmHEADER.dwSize = 44 + pcmDATA.dwSize;

    rewind(fpout);
    fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fpout);
    fseek(fpout, sizeof(WAVE_FMT), SEEK_CUR);
    fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fpout);

    fclose(fp);
    fclose(fpout);

    return 0;
}












