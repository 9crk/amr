#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedef.h>
#include <enc_if.h>
#include <dec_if.h>

#define AMR_MAGIC_NUMBER "#!AMR-WB\n"
extern const UWord8 block_size[];

int main(int argc,char* argv[]) {
    
    int dtx = 0;
    WB_enc_if_state enstate;
    E_IF_init(&enstate);
    
    WB_dec_if_state destate;
    D_IF_init(&destate);
    int req_mode = atoi(argv[2]);
    FILE* pcm_orig = fopen(argv[1], "rb");
    FILE* amrwb = fopen("cclzhouhua.amrwb", "wb");
    FILE* pcm_back = fopen("cclzhouhua.back", "wb");
    if (NULL == pcm_orig) exit(1);
    if (NULL == amrwb) exit(1);
    if (NULL == pcm_back) exit(1);
    fwrite(AMR_MAGIC_NUMBER, sizeof(char), strlen(AMR_MAGIC_NUMBER), amrwb);
    printf("ok\r\n");
    while(!feof(pcm_orig)){
        unsigned char serial_data[61];
        short speech[320];
        int len = fread(&speech,640,1,pcm_orig);
        int byte_counter = E_IF_encode(&enstate, req_mode, speech, serial_data, dtx);
        fwrite(serial_data, sizeof(char), byte_counter, amrwb);
        /*printf("test times: %d, bytes: %d\n", i, byte_counter);*/
        Word16 dec_mode = (serial_data[0] >> 3) & 0x0F;
        int read_size = block_size[dec_mode];
        D_IF_decode(&destate, serial_data, speech, 0);
        fwrite(speech, sizeof(short int), 320, pcm_back);
        /*printf("test times: %d, bytes: %d\n", i, read_size);*/
    }
    fclose(pcm_orig);
    fclose(amrwb);
    fclose(pcm_back);
    return 0;
}
