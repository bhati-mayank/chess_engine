/* AN ATTEMPT TO BUILD A CHESS ENGINE BY ME */




//SYSTEM HEADERS
#include<stdio.h>

//bitboard data type defining
#define U64 unsigned long long


//get/set/pop macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))



//printing the bitboard
void print_bitboard(U64 bitboard){
    //loop over board ranks
    for(int rank = 0; rank < 8; rank++){
        //loop over files
        for(int file = 0;  file < 8; file++){
            //convert rank and file into square index
            int square  = rank*8 + file;

            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }

        //print new line every rank
        printf("\n");
    }
}

//main driver
int main(){
    U64 bitboard = 0ULL;
    print_bitboard(bitboard);

    return 0;   
} 