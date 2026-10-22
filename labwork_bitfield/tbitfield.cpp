// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <stdexcept>
#include <iostream>

const int BitsInElem = sizeof(TELEM) * 8;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("Отрицательная длина битового поля");
    }
    BitLen = len;
    MemLen = (BitLen + BitsInElem - 1) / BitsInElem;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        memset(pMem, 0, MemLen * sizeof(TELEM));
    }
    else {
        pMem = nullptr;
    }
}

TBitField::TBitField(const TBitField& bf) {  // конструктор копирования
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField() {
    if (pMem != nullptr) {
        delete[] pMem;
        pMem = nullptr;
    }
}

int TBitField::GetMemIndex(const int n) const { // индекс Мем для бита n
    if (n < 0 || n >= BitLen) {
        throw std::invalid_argument("Недопустимое Значение Индекса");
    }
    int index = n / (8 * sizeof(TELEM));
    return index;
}

TELEM TBitField::GetMemMask(const int n) const { // битовая маска для бита n
    if (n < 0 || n >= BitLen) // если индекс выходит за границы поля
    {
        throw std::invalid_argument("Недопустимое значение индекса");
    }
    TELEM bitmask = (TELEM)1 << (n % (8 * sizeof(TELEM)));
    return bitmask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const { // получить длину (к-во битов)
    return BitLen;
}

void TBitField::SetBit(const int n) { // установить бит
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) { // очистить бит
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] & ~mask;
}

int TBitField::GetBit(const int n) const { // получить значение бита
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    if ((pMem[index] & mask) != 0) {
        return 1;
    }
    else {
        return 0;
    };
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) { // присваивание
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            if (pMem != nullptr) {
                delete[] pMem;
            }
            MemLen = bf.MemLen;
            if (MemLen > 0) {
                pMem = new TELEM[MemLen];
            }
            else {
                pMem = nullptr;
            }
        }
        BitLen = bf.BitLen;
        if (MemLen > 0) {
            memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const { // сравнение
    if (BitLen != bf.BitLen) {
        return 0;
    }
    int result = memcmp(pMem, bf.pMem, MemLen * sizeof(TELEM));
    if (result == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int TBitField::operator!=(const TBitField& bf) const { // сравнение
    if (*this == bf) {
        return 0;
    }
    else {
        return 1;
    }
}

TBitField TBitField::operator|(const TBitField& bf) { // операция "или"
    int maxLen = BitLen;
    if (bf.BitLen > BitLen) {
        maxLen = bf.BitLen;
    }
    TBitField result(maxLen);
    memcpy(result.pMem, pMem, MemLen * sizeof(TELEM));
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] |= bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) { // операция "и"
    int maxLen = BitLen;
    if (bf.BitLen > BitLen) {
        maxLen = bf.BitLen;
    }
    TBitField result(maxLen);
    int minMemLen = MemLen;
    if (bf.MemLen < MemLen) {
        minMemLen = bf.MemLen;
    }
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) { // отрицание
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    if (BitLen % BitsInElem != 0) {
        TELEM mask = (1 << (BitLen % BitsInElem)) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) { // ввод
    int i = 0;
    char ch;
    memset(bf.pMem, 0, bf.MemLen * sizeof(TELEM));

    while (istr >> ch) {
        if (ch == '1') {
            if (i < bf.BitLen) {
                bf.SetBit(i);
            }
            i++;
        }
        else if (ch == '0') {
            if (i < bf.BitLen) {
                bf.ClrBit(i);
            }
            i++;
        }
        else {
            istr.putback(ch);
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) { // вывод
    int length = bf.GetLength();
    for (int i = 0; i < length; i++) {
        if (bf.GetBit(i) == 1) {
            ostr << '1';
        }
        else {
            ostr << '0';
        }
    }
    return ostr;
}
