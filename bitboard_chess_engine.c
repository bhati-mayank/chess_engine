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
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

//enumeration of pieces
enum { white, black, both};

// bishop and rook
enum {rook, bishop}; //0-1



// castling rights binary encoding

/*

    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side

   examples

   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side

*/

enum { wk = 1, wq = 2, bk = 4, bq = 8 };

// encode pieces
enum { P, N, B, R, Q, K, p, n, b, r, q, k};

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


// ASCII pieces
char ascii_pieces[12] = "PNBRQKpnbrqk";

// unicode pieces
char *unicode_pieces[12] = {"♙","♘","♗","♖","♕","♔","♟︎","♞","♝","♜"," ♛","♚" };

// convert ASCII character pieces to encoded constants
int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,

    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k,
};

                /**********************************\
                 ==================================
                
                            Chess board
                
                ==================================
                \**********************************/

/*
                            WHITE PIECES


        Pawns                  Knights              Bishops
        
  8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  1 1 1 1 1 1 1 1    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 1 0 0 0 0 1 0    1  0 0 1 0 0 1 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


         Rooks                 Queens                 King

 8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0    8  0 0 0 0 0 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  1 0 0 0 0 0 0 1    1  0 0 0 1 0 0 0 0    1  0 0 0 0 1 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


                            BLACK PIECES


        Pawns                  Knights              Bishops
        
  8  0 0 0 0 0 0 0 0    8  0 1 0 0 0 0 1 0    8  0 0 1 0 0 1 0 0
  7  1 1 1 1 1 1 1 1    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h


         Rooks                 Queens                 King

 8  1 0 0 0 0 0 0 1    8  0 0 0 1 0 0 0 0    8  0 0 0 0 1 0 0 0
  7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0    7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0    2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0    1  0 0 0 0 0 0 0 0

     a b c d e f g h       a b c d e f g h       a b c d e f g h



                             OCCUPANCIES


     White occupancy       Black occupancy       All occupancies

  8  0 0 0 0 0 0 0 0    8  1 1 1 1 1 1 1 1    8  1 1 1 1 1 1 1 1
  7  0 0 0 0 0 0 0 0    7  1 1 1 1 1 1 1 1    7  1 1 1 1 1 1 1 1
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  1 1 1 1 1 1 1 1    2  0 0 0 0 0 0 0 0    2  1 1 1 1 1 1 1 1
  1  1 1 1 1 1 1 1 1    1  0 0 0 0 0 0 0 0    1  1 1 1 1 1 1 1 1


  
                            ALL TOGETHER

                        8  ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜
                        7  ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎
                        6  .  .  .  .  .  .  .  .  
                        5  .  .  .  .  .  .  .  .
                        4  .  .  .  .  .  .  .  .
                        3  .  .  .  .  .  .  .  .
                        2  ♙ ♙ ♙ ♙ ♙ ♙  ♙♙
                        1  ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖

                           a b c d e f g h

*/

// piece bitboards
U64 bitboards[12];

// define occupancy bit boards -->white black and all
U64 occupancies[3];

// side to move
int side;

// enpassant square
int enpassant = no_sq;

// castling rights
int castle;



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
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << square))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << square))
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

//print board
void print_board()
{
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {


        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            //print ranks
            if(!file){
                printf(" %d   ", 8 - rank);
            }

            // define piece variable
            int piece = -1;

            // loop over all piece bitboards
            for( int bb_piece = P; bb_piece <= k; bb_piece++ ){

                if(get_bit(bitboards[bb_piece], square))
                    piece = bb_piece;
            }

            // print piece
            #ifdef WIN64
                printf(" %c ", (piece == -1) ? '.' : ascii_pieces[piece]);

            #else
                printf(" %s ", (piece == -1) ? "." : unicode_pieces[piece]);

            #endif

        }

        printf("\n");
    }

    // print files
    printf("\n      a  b  c  d  e  f  g  h\n\n");

    // print side to move
    printf("      Side      :     %s\n", !side ? "white" : "black" );


    //print enpassant square
    printf("      Enpassant :     %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");

    // print castling rights
    printf("      Castling  :     %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
                                            (castle & wq) ? 'Q' : '-',
                                            (castle & bk) ? 'k' : '-',
                                            (castle & bq) ? 'q' : '-');
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

// rook magic numbers
U64 rook_magic_numbers[64] = {
0x8a80104000800020ULL,0x140002000100040ULL,0x2801880a0017001ULL,0x100081001000420ULL,
0x200020010080420ULL,0x3001c0002010008ULL,0x8480008002000100ULL,0x2080088004402900ULL,
0x800098204000ULL,0x2024401000200040ULL,0x100802000801000ULL,0x120800800801000ULL,
0x208808088000400ULL,0x2802200800400ULL,0x2200800100020080ULL,0x801000060821100ULL,
0x80044006422000ULL,0x100808020004000ULL,0x12108a0010204200ULL,0x140848010000802ULL,
0x481828014002800ULL,0x8094004002004100ULL,0x4010040010010802ULL,0x20008806104ULL,
0x100400080208000ULL,0x2040002120081000ULL,0x21200680100081ULL,0x20100080080080ULL,
0x2000a00200410ULL,0x20080800400ULL,0x80088400100102ULL,0x80004600042881ULL,
0x4040008040800020ULL,0x440003000200801ULL,0x4200011004500ULL,0x188020010100100ULL,
0x14800401802800ULL,0x2080040080800200ULL,0x124080204001001ULL,0x200046502000484ULL,
0x480400080088020ULL,0x1000422010034000ULL,0x30200100110040ULL,0x100021010009ULL,
0x2002080100110004ULL,0x202008004008002ULL,0x20020004010100ULL,0x2048440040820001ULL,
0x101002200408200ULL,0x40802000401080ULL,0x4008142004410100ULL,0x2060820c0120200ULL,
0x1001004080100ULL,0x20c020080040080ULL,0x2935610830022400ULL,0x44440041009200ULL,
0x280001040802101ULL,0x2100190040002085ULL,0x80c0084100102001ULL,0x4024081001000421ULL,
0x20030a0244872ULL,0x12001008414402ULL,0x2006104900a0804ULL,0x1004081002402ULL,
};


//bishop magic numbers
U64 bishop_magic_numbers[64] = {
0x40040844404084ULL,0x2004208a004208ULL,0x10190041080202ULL,0x108060845042010ULL,
0x581104180800210ULL,0x2112080446200010ULL,0x1080820820060210ULL,0x3c0808410220200ULL,
0x4050404440404ULL,0x21001420088ULL,0x24d0080801082102ULL,0x1020a0a020400ULL,
0x40308200402ULL,0x4011002100800ULL,0x401484104104005ULL,0x801010402020200ULL,
0x400210c3880100ULL,0x404022024108200ULL,0x810018200204102ULL,0x4002801a02003ULL,
0x85040820080400ULL,0x810102c808880400ULL,0xe900410884800ULL,0x8002020480840102ULL,
0x220200865090201ULL,0x2010100a02021202ULL,0x152048408022401ULL,0x20080002081110ULL,
0x4001001021004000ULL,0x800040400a011002ULL,0xe4004081011002ULL,0x1c004001012080ULL,
0x8004200962a00220ULL,0x8422100208500202ULL,0x2000402200300c08ULL,0x8646020080080080ULL,
0x80020a0200100808ULL,0x2010004880111000ULL,0x623000a080011400ULL,0x42008c0340209202ULL,
0x209188240001000ULL,0x400408a884001800ULL,0x110400a6080400ULL,0x1840060a44020800ULL,
0x90080104000041ULL,0x201011000808101ULL,0x1a2208080504f080ULL,0x8012020600211212ULL,
0x500861011240000ULL,0x180806108200800ULL,0x4000020e01040044ULL,0x300000261044000aULL,
0x802241102020002ULL,0x20906061210001ULL,0x5a84841004010310ULL,0x4010801011c04ULL,
0xa010109502200ULL,0x4a02012000ULL,0x500201010098b028ULL,0x8040002811040900ULL,
0x28000010020204ULL,0x6000020202d0240ULL,0x8918844842082200ULL,0x4010011029020020ULL,
};

//===========================================//
//===========================================//


/*  PAWN ATTACKS TABLE  [side to move][square]  */
U64 pawn_attacks[2][64];


/*  KNIGHTS ATTACK TABLE [square]*/
U64 knight_attacks[64];


/*  KING ATTACK TABLE [square] */
U64 king_attacks[64];

/* bishop attack masks*/
U64 bishop_masks[64];

/* rook attack masks*/
U64 rook_masks[64];

/*bishop attacks table [square][occupancy]*/
U64 bishop_attacks[64][512];

/*rok attacks table [square][occupancy]*/
U64 rook_attacks[64][4096];


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
        for( index = 0, fail = 0; !fail && index < occupancy_indicies; index++){

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


// init slider piece's attack tables
void init_sliders_attacks(int bishop){

    //loop over 64 board squares
    for(int square = 0; square < 64; square++){

        //init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square]   = mask_rook_attacks(square);

        //init curretn mask
        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        //init relevant occupancy bit count
        int relevant_bits_count = count_bits(attack_mask);

        //init occupancy indices
        int occupancy_indicies = ( 1 << relevant_bits_count ); 

        //loop over occupancy indices
        for(int index = 0; index < occupancy_indicies; index++){

            //bishop
            if(bishop){
                //init current occupancy variation
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                //init magic index
                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

                // init bishop attacks
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }

            //rook
            else{
                //init current occupancy variation
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                //init magic index
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

                // init bishop attacks
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

//get bishop attacks
static inline U64 get_bishop_attacks(int square, U64 occupancy){

    // get bishop attacks  assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= ( 64 - bishop_relevant_bits[square]);

    return bishop_attacks[square][occupancy];
}

//get rook attacks
static inline U64 get_rook_attacks(int square, U64 occupancy){

    // get bishop attacks  assuming current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= ( 64 - rook_relevant_bits[square]);

    return rook_attacks[square][occupancy];
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

    //init slider piece attacks
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

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

    set_bit(bitboards[P], e2);
    set_bit(bitboards[P], h2);
    set_bit(bitboards[P], b2);
    set_bit(bitboards[P], d2);

    set_bit(bitboards[N], b1);
    set_bit(bitboards[N], g1);

    //init side
    side = black;

    //init expassant
    enpassant = e3;


    //print_bitboard(bitboards[P]);

    print_board();

    return 0;

} 