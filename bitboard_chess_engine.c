/* AN ATTEMPT TO BUILD A CHESS ENGINE BY ME */




//SYSTEM HEADERS
#include<stdio.h>
#include<string.h>

//bitboard data type defining
#define U64 unsigned long long

//board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

//enumeration of pieces
enum { white, black };

// bishop and rook
enum {rook, bishop}; //0-1

const char *square_to_coordinates[] = {

    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};


/****************************************************
 * =================================================
 *             
 *                  RANDOM NUMBERS
 * 
 * =================================================
 * ***************************************************
 */

 // pseudo randon number state
unsigned int random_state = 1804289383;

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number(){

    //get current state
    unsigned int number = random_state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    //update random number state
    random_state = number;

    //return the random number
    return number;
}

//generate 64-bit pseudo legal numbers
U64 get_random_U64_number(){

    //define 4 random numbers
    U64 n1,n2,n3,n4;

    //init random numbers slicing 16bits form MSB side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    //return random number
    return n1 | ( n2 << 16 ) | (n3 << 32) | (n4 << 48);
}

//generate magic number candidate
U64 generate_magic_number(){

    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}


//get/set/pop macros
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

// count bits within a bitboard
static inline int count_bits(U64 bitboard){

    //bit counter
    int count = 0;

    //consecutively reset least significant bit 
    while(bitboard){

        //resetting the least significant bit
        bitboard = bitboard & (bitboard -1);

        //increment count
            count++;
    }

    //return the bit counter 
    return count;
}


//get least significant 1st bit index
static inline int get_ls1st_bit_index(U64 bitboard){

    //make sure bitboard is not zero
    if(bitboard){
        
        //count trailing bits first
        return count_bits((bitboard & -bitboard) -1);
    }

    else 
        return -1; //as no 1bit present
}


//printing the bitboard
void print_bitboard(U64 bitboard){

    printf("\n");

    //loop over board ranks
    for(int rank = 0; rank < 8; rank++){
        //loop over files
        for(int file = 0;  file < 8; file++){
            //convert rank and file into square index
            int square  = rank*8 + file;

            //print ranks
            if(!file){
                printf(" %d   ", 8 - rank);
            }
            
            //print bit state either 1 or 0
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }

        //print new line every rank
        printf("\n");
    }

    //print board files
    printf("\n      a  b  c  d  e  f  g  h\n");


    //print bitboard as unsigned decimal number
    printf("\n\nBitboard : %llud\n\n", bitboard);
}


/*
//=======================================================//

                         ATTACKS
       
//=======================================================//     
*/

/*
        not A file

 8    0  1  1  1  1  1  1  1 
 7    0  1  1  1  1  1  1  1 
 6    0  1  1  1  1  1  1  1 
 5    0  1  1  1  1  1  1  1 
 4    0  1  1  1  1  1  1  1 
 3    0  1  1  1  1  1  1  1 
 2    0  1  1  1  1  1  1  1 
 1    0  1  1  1  1  1  1  1 

      a  b  c  d  e  f  g  h


---------------------------------

        not H file 

 8    1  1  1  1  1  1  1  0 
 7    1  1  1  1  1  1  1  0 
 6    1  1  1  1  1  1  1  0 
 5    1  1  1  1  1  1  1  0 
 4    1  1  1  1  1  1  1  0 
 3    1  1  1  1  1  1  1  0 
 2    1  1  1  1  1  1  1  0 
 1    1  1  1  1  1  1  1  0 

      a  b  c  d  e  f  g  h

----------------------------------

        not HG file


 8    1  1  1  1  1  1  0  0 
 7    1  1  1  1  1  1  0  0 
 6    1  1  1  1  1  1  0  0 
 5    1  1  1  1  1  1  0  0 
 4    1  1  1  1  1  1  0  0 
 3    1  1  1  1  1  1  0  0 
 2    1  1  1  1  1  1  0  0 
 1    1  1  1  1  1  1  0  0 

      a  b  c  d  e  f  g  h

----------------------------------

        not AB file


 8    0  0  1  1  1  1  1  1 
 7    0  0  1  1  1  1  1  1 
 6    0  0  1  1  1  1  1  1 
 5    0  0  1  1  1  1  1  1 
 4    0  0  1  1  1  1  1  1 
 3    0  0  1  1  1  1  1  1 
 2    0  0  1  1  1  1  1  1 
 1    0  0  1  1  1  1  1  1 

      a  b  c  d  e  f  g  h

*/

//not A file cosntant
const U64 not_a_file = 18374403900871474942ULL;


//not a H file constant
const U64 not_h_file = 9187201950435737471ULL;

//not a HG constant
const U64 not_hg_file = 4557430888798830399ULL;

//not AB file const
const U64 not_ab_file = 18229723555195321596ULL;


//relevant occupancy bit count for every square on board

  //bishop
const int bishop_relevant_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        6, 5, 5, 5, 5, 5, 5, 6 
};

  //rook
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12 
};


//rook magic numbers
U64 rook_magic_numbers[64] = {
    0x2000380201000008ULL,
0x9002041009000030ULL,
0x1000430100100082ULL,
0x808008040002020ULL,
0x801a0105000424ULL,
0x12000018200000a0ULL,
0x2000020800220040ULL,
0x40801910d4004ULL,
0x1020210000040180ULL,
0x1915500080810013ULL,
0x2800000882600ULL,
0x28140010110aULL,
0xc0018c1000262001ULL,
0xa001000200060ULL,
0xc00000e00a000500ULL,
0x4050008210040800ULL,
0x25c080804200101ULL,
0x14a0000c00001004ULL,
0x202008011001182ULL,
0x88d288c004000ULL,
0xa520042000120000ULL,
0x80280304a109080ULL,
0x841801400120030ULL,
0x800080000100001ULL,
0x3200a40580200001ULL,
0x400020500c00200ULL,
0x2020020410000010ULL,
0x1000082400000400ULL,
0x800402808004000ULL,
0x40000300a102020ULL,
0x80042002904000ULL,
0x181080480000041ULL,
0x540004900470302ULL,
0x2019004000c382ULL,
0x1040010001880800ULL,
0x9000200289008000ULL,
0x131042080000150ULL,
0x220800040c04800ULL,
0x8c02100001200ULL,
0x2022188000040120ULL,
0x20020891201800ULL,
0x608022088440411ULL,
0x8061840001108005ULL,
0x870001a040ULL,
0x7000000009018ULL,
0x1001064a2250000ULL,
0x2000820300180020ULL,
0x2040c09000060020ULL,
0x2008000004069210ULL,
0x8b08001220009ULL,
0x400032040004ULL,
0x20006200040280c0ULL,
0x20004403000010ULL,
0x8000c001020c0001ULL,
0x12040810902240ULL,
0x400c802211420200ULL,
0x1a10004004004ULL,
0xa09540042500000ULL,
0x540008008069ULL,
0x1080c804200ULL,
0xa000081241180380ULL,
0x800880000402445ULL,
0x8040035200202040ULL,
0x400080002440020ULL
};

//bishop magic numbers
U64 bishop_magic_numbers[64] = {
    

0x8000900140840092ULL,
0x804840004c900ULL,
0x4018a4000100200ULL,
0x2008028800020000ULL,
0x4003108c18304c34ULL,
0x2c08401405280000ULL,
0x100780600010ULL,
0xc408004a001040ULL,
0x800420008018010ULL,
0x1800012898000018ULL,
0x62040981c0001032ULL,
0x102120030402100ULL,
0x10182840048180ULL,
0x50405800000840ULL,
0x4300085008ULL,
0x11008402150a0200ULL,
0x840011004044280ULL,
0x704840810202ULL,
0x8000020e080001ULL,
0x8808008200808401ULL,
0x44c20212800000ULL,
0x18000d090041800ULL,
0x10e020504211200ULL,
0x200021018440400ULL,
0xaa0001210064048ULL,
0x241102000008124ULL,
0x100002c12180204ULL,
0x880200102070ULL,
0x40200052008011ULL,
0x430000d0ULL,
0x8024c0005408060fULL,
0x400002000c50060ULL,
0x422008180c1020ULL,
0x2308102a00414400ULL,
0x2000082020004206ULL,
0x8004000c2024090ULL,
0x6006501801ULL,
0x88880004685010ULL,
0x10000802040003c0ULL,
0x1010430800024000ULL,
0x410041240190800ULL,
0x1804000b00810100ULL,
0x800002808000284ULL,
0x648041000ULL,
0x80010802081044ULL,
0x8080024009001042ULL,
0x3000dc020500020ULL,
0x80042000e00004ULL,
0x220802468032200ULL,
0x10858020080a0ULL,
0x2080604201818000ULL,
0x8002100214000ULL,
0x8040000821802000ULL,
0xa0001200000c020ULL,
0x100090004e800ULL,
0x28890000282040ULL,
0x40502920100000ULL,
0x8020293001200840ULL,
0x2605400040003000ULL,
0x2200002002800446ULL,
0x200806200a008ULL,
0x801400600042100ULL,
0x2900290080208400ULL,
0x5000020058ULL
};


//===========================================//
//===========================================//


/*  PAWN ATTACKS TABLE  [side to move][square]  */
U64 pawn_attacks[2][64];


/*  KNIGHTS ATTACK TABLE [square]*/
U64 knight_attacks[64];


/*  KING ATTACK TABLE [square] */
U64 king_attacks[64];


//generate pawn attacks (curr_square, side to move)
U64  mask_pawn_attacks(int side, int square){

    // result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set pices on board
    set_bit(bitboard, square);

    //white pawns
    if(!side){
        
        //generate pawn  attacks for white (right shift by offsets)
        if((bitboard >> 7)& not_a_file)
            attacks = attacks | (bitboard >> 7);

        if((bitboard >> 9)& not_h_file)
            attacks = attacks | (bitboard >> 9);
    }
    
    //black pawn
    else{

        //generate pawn  attacks for black(left shift by offsets)
        if((bitboard << 7)& not_h_file)
            attacks = attacks | (bitboard << 7);

        if((bitboard << 9)& not_a_file)
            attacks = attacks | (bitboard << 9);
    }

    //return attack map
    return attacks;
}


//generate knight attacks 
U64 mask_knight_attacks(int square){

    //result attack bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set pieces on board
    set_bit(bitboard, square);

    //generate knight attacks --> 17, 15, 6, 10
    if((bitboard >> 17) & not_h_file ) attacks |= (bitboard >> 17);
    if((bitboard << 17) & not_a_file ) attacks |= (bitboard << 17);
    if((bitboard >> 15) & not_a_file ) attacks |= (bitboard >> 15);
    if((bitboard << 15) & not_h_file ) attacks |= (bitboard << 15);
    if((bitboard << 06) & not_hg_file) attacks |= (bitboard << 06);
    if((bitboard >> 06) & not_ab_file) attacks |= (bitboard >> 06);
    if((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);

    //return attacks map
    return attacks;
}


//generate king attacks
U64 mask_king_attacks(int square){

    //result attack bit board
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //generating king attacks --> 1, 7, 9, 8
    if((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    attacks |= (bitboard << 8);

    if((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    attacks |= (bitboard >> 8);

    //return attack map
    return attacks;

}


// mask bishop attacks
U64 mask_bishop_attacks(int square){

    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target rank and files
    int tr = square/ 8;
    int tf = square% 8;

    //mask relevent bishop occupancy bits
    for(r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++,f++)
        attacks |= (1ULL << (r*8 + f));

    for(r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--,f++)
        attacks |= (1ULL << (r*8 + f));


    for(r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++,f--)
        attacks |= (1ULL << (r*8 + f));


    for(r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--,f--)
        attacks |= (1ULL << (r*8 + f));


    

    //return attack map
    return attacks;
}


//mask rook attacks
U64 mask_rook_attacks(int square){

    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target rank and files
    int tr = square/ 8;
    int tf = square% 8;

    //mask relevent rook occupancy bits
    for(r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));

    for(r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));

    for(f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));

    for(f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    //return attack map
    return attacks;
}


// generate  bishop attacks on  the fly
U64 bishop_attacks_on_the_fly(int square, U64 block){

    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target rank and files
    int tr = square/ 8;
    int tf = square% 8;

    //generate bishop attacks
    for(r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++,f++){

        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f)) & block) break;
    }

    for(r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--,f++){

        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f)) & block) break;
    }


    for(r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++,f--){

        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f)) & block) break;
    }

    for(r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--,f--){

        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f)) & block) break;
    }

    //return attack map
    return attacks;
}


//generate  rook attacks on the fly
U64 rook_attacks_on_the_fly(int square, U64 block){

    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target rank and files
    int tr = square/ 8;
    int tf = square% 8;

    //generate rook attacks
    for(r = tr + 1; r <= 7; r++){

        attacks |= (1ULL << (r * 8 + tf));
        if((1ULL << (r * 8 + tf)) & block) break;
    }

    for(r = tr - 1; r >= 0; r--){

        attacks |= (1ULL << (r * 8 + tf));
        if((1ULL << (r * 8 + tf)) & block) break;

    }

    for(f = tf + 1; f <= 7; f++){

        attacks |= (1ULL << (tr * 8 + f));
        if((1ULL << (tr * 8 + f)) & block) break;
    }

    for(f = tf - 1; f >= 0; f--){

        attacks |= (1ULL << (tr * 8 + f));
        if((1ULL << (tr * 8 + f)) & block) break;
    }

    //return attack map
    return attacks;
}


//initialize leaper pieces attacks
void init_leapers_attacks(){

    //loop over 64 board squares
    for(int square = 0; square < 64; square++){

        //init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        //init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        //init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}


//set occupancies
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask){

    // occupancy map
    U64 occupancy = 0ULL;

    //loop over the range of bits within attack mask
    for(int count = 0; count < bits_in_mask; count++){

        //get least signi 1st bit index of attack mask
        int square = get_ls1st_bit_index(attack_mask);

        //pop least signi bit in attack map
        pop_bit(attack_mask, square);

        //make sure occupancy is on board
        if(index & (1 << count))
            //popolate occupancy map
            occupancy |= (1ULL << square);
    }

    //return occupancy map
    return occupancy;
}



//======================================//
/*
                MAGICS
*/
//======================================//

// find appropriate magic number
U64 find_magic_number(int square, int relevant_bits, int bishop){

    // init occupancies
    U64 occupancies[4096];

    // init attack tables 
    U64 attacks[4096];

    // init used attacks
    U64 used_attacks[4096];

    //init attack mask for current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // init occpancy indices
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indices
    for(int index = 0; index < occupancy_indicies; index++){

        // init occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        //init attacks
        attacks[index] = bishop ? bishop_attacks_on_the_fly(square, occupancies[index]) 
                            : rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers loop
    for( int random_count = 0; random_count < 100000000; random_count++){
         
        //generate magic number candidate
        U64 magic_number = generate_magic_number();

        //skip inappropriate magic numbers
        if(count_bits((attack_mask*magic_number) & 0xFF00000000000000 )< 6 ) continue;

        // init used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index and fail flag
        int index, fail;

        // test magic index loop
        for( index = 0, fail = 0; !fail && index << occupancy_indicies; index++){

            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if(used_attacks[magic_index] == 0ULL)
                //init used attacks
                used_attacks[magic_index] = attacks[index];

            // otherwise
            else if(used_attacks[magic_index] != attacks[index])
                //magic index doesnt work
                fail = 1;
        }

        // if magic nummbre works 
        if(!fail)
            //return it
            return magic_number;
    } 

    // if magic number doesnt work
    printf(" Magic number fails!");
    return 0ULL;
}

// init magic number 
void init_magic_numbers(){

    //loop over 64 board squaeres
    for(int square = 0; square < 64; square++)
        //init rook magic numbers
        rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);

    //loop over 64 board squaeres
    for(int square = 0; square < 64; square++)
        //init rook magic numbers
        bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
}




//======================================//
/*
            INIT ALL
*/
//======================================//

// init all variables
void init_all(){

    //init leaper pieces attacks
    init_leapers_attacks();

    //init magic numbers
    //init_magic_numbers();
}


//======================================//
/*
            MAIN DRIVER
*/
//======================================//




int main(){

    //init all
    init_all();

    

    return 0;

} 