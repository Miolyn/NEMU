#include "nemu.h"
#include <time.h>
#define random(x) (rand()%x)
TLBCache TLB;
void reset_tlb(){
    TLB.lineNum = TLB_LINE_NUM;
    srand((int)time(0));
    int i;
    if(TLB.TLBLines == NULL){
        TLB.TLBLines = (TLBLine*)malloc(TLB_LINE_NUM * sizeof(TLBLine));
    }
        printf("inti tlb\n");
    for(i = 0; i < TLB.lineNum; i++){
        TLB.TLBLines[i].valid_bit = 0;
    }
}

uint32_t read_tlb(uint32_t lnAddr, bool *success){
    uint32_t v_addr = lnAddr >> 12;
    int i;
    for(i = 0; i < TLB.lineNum; i++){
        if(TLB.TLBLines[i].valid_bit && TLB.TLBLines[i].tag == v_addr){
            *success = true;
            return TLB.TLBLines[i].addr;
        }
    }
    *success = false;
    return 0;
}

void write_tlb(uint32_t lnAddr, uint32_t addr){
    uint32_t v_addr = lnAddr >> 12;
    int i;
    for(i = 0; i < TLB.lineNum; i++){
        if(!TLB.TLBLines[i].valid_bit){
            break;
        }
    }
    if(TLB.lineNum == i){
        i = random(TLB.lineNum);
    }
    TLB.TLBLines[i].valid_bit = 1;
    TLB.TLBLines[i].tag = v_addr;
    TLB.TLBLines[i].addr = addr;
}