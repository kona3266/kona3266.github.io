/**************************
@author: Hsien Loong Lee
-------------------------*/

# include <stdio.h>

int InRow[81], InCol[81], InBlock[81];

const int BLANK = 0;
const int ONES = 0x3fe;    // Binary 1111111110, ten bits, start with 9 consecutive number one and ends with zero.

int Entry[81];
int Block[9], Row[9], Col[9];

int SeqPtr = 0;
int Sequence[81];

int Count = 0;
int LevelCount[81];

void p9_print(int *array) {
    int i;

    for (i = 0; i < 81; i++) {
        printf("%d ", array[i]);
        if ((i + 1) % 9 == 0) {
            printf("\n");
        }
    }
}

void SwapSeqEntries(int s1, int s2) {
    int temp = Sequence[s2];
    Sequence[s2] = Sequence[s1];
    Sequence[s1] = temp;
}

void PrintArray() {
    int i, j, valbit, val, Square;
    char ch;

    Square = 0;

    for (i = 0; i < 9; i ++) {
        if (i % 3 == 0) putc('\n', stdout);
        for (j = 0; j < 9; j++) {
            if (j % 3 == 0) putc(' ', stdout);
            valbit = Entry[Square++];
            if (valbit == 0) {
                ch = '-';
            }else {
                for (val = 1; val <= 9; val++) {
                    if (valbit == (1 << val)) {
                        ch = '0' + val;
                        break;
                    }
                }
            }
            putc(ch, stdout);
        }
        putc('\n', stdout);
    }
}

void InitEntry(int i, int j, int val) {
    int Square = 9 * i + j;
    int valbit = 1 << val;
    int SeqPtr2;

    Entry[Square] = valbit;    // eg. valbit 10 stand for number 2, a unified form is 00000 00100
    Block[InBlock[Square]] &= ~valbit;  // Block is initialized with 1111111110
    //If a cell in block area is filled with valbit, the corresbonding bit of block val will be assigned 0.
    //If all cells in the block is filled, the block val will be 00000 00000 
    Col[InCol[Square]] &= ~valbit;
    Row[InRow[Square]] &= ~valbit;

    SeqPtr2 = SeqPtr;
    while (SeqPtr2 < 81 && Sequence[SeqPtr2] != Square) {
        SeqPtr2++;
    }

    SwapSeqEntries(SeqPtr, SeqPtr2);
    SeqPtr++;
}

void ConsoleInput() {
    int i, j;
    char InputString[80];

    for (i = 0; i < 9; i++) {
        printf("Row[%d] :", i + 1);
        scanf("%s", InputString);

        for (j = 0; j < 9; j ++) {
            char ch = InputString[j];
            if (ch >= '1' && ch <= '9') {
                InitEntry(i, j, ch - '0');
            }
        }
    }
    PrintArray();
    p9_print(Sequence);
}

int NextSeq(int S) {
    int S2, Square, Possibles, BitCount;
    int T, MinBitCount = 100;

    for (T = S; T < 81; T++) {
        Square = Sequence[T];
        Possibles = Block[InBlock[Square]] & Row[InRow[Square]] & Col[InCol[Square]];
        BitCount = 0;
        while (Possibles) {
            Possibles &= ~(Possibles & -Possibles);
            BitCount++;
        }

        if (BitCount < MinBitCount) {
            MinBitCount = BitCount;
            S2 = T;
        }
    }

    return S2;
}

void Succeed() {
    PrintArray();
}

void Place(int S) {
    LevelCount[S]++;
    Count++;

    if (S >= 81) {
        Succeed();
        return;
    }

    int S2 = NextSeq(S);                         // minimise the fanout
    SwapSeqEntries(S, S2);

    int Square = Sequence[S];

    int BlockIndex = InBlock[Square], 
    RowIndex = InRow[Square],
    ColIndex = InCol[Square];

    int Possibles = Block[BlockIndex] & Row[RowIndex] & Col[ColIndex];
    // the number of 1 bit in Possibles means the number of cell in BLOCK/ROW/COL left to fill
    while (Possibles) {
        int valbit = Possibles & (-Possibles);  // get lowest 1 bit in Possibles, it mean use the minmal number which is not used.
        Possibles &= ~valbit;                   // eliminate lowest 1 bit in Possibles
        Entry[Square] = valbit;
        Block[BlockIndex] &= ~valbit;
        Row[RowIndex] &= ~valbit;
        Col[ColIndex] &= ~valbit;

        Place(S + 1);

        Entry[Square] = BLANK;                       // revert operation
        Block[BlockIndex] |= valbit;
        Row[RowIndex] |= valbit;
        Col[ColIndex] |= valbit;
    }

    SwapSeqEntries(S, S2);
}


int main(int arg, char *argv[]) {
    int i, j, Square;

    for (i = 0; i <9; i++) {
        for (j = 0; j < 9; j++) {
            Square = 9 * i + j;
            InRow[Square] = i;
            InCol[Square] = j;
            InBlock[Square] = (i / 3) * 3 + j / 3;
        }
    }

    // p9_print(InRow);
    // p9_print(InCol);
    // p9_print(InBlock);

    for (Square = 0; Square < 81; Square++) {
        Sequence[Square] = Square;
        Entry[Square] = BLANK;
        LevelCount[Square] = 0;
    }

    for (i = 0; i < 9; i++) {
        Block[i] = Row[i] = Col[i] = ONES;
    }

    ConsoleInput();
    Place(SeqPtr);

    return 0;
}