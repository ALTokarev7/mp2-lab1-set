// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    this->MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    this->MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    this->MaxPower = this->BitField.GetLength();
}

TSet::operator TBitField()
{
    TBitField temp(this->BitField);
    return temp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return this->BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    this->BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    this->BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s)
        return *this;
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (this->MaxPower == s.MaxPower)
        if (this->BitField == s.BitField)
            return 1;
    return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (this->MaxPower == s.MaxPower)
        if (this->BitField == s.BitField)
            return 0;
    return 1;
}

TSet TSet::operator+(const TSet& s) // объединение
{   
    TSet res(max(this->MaxPower, s.MaxPower));
    res.BitField = this->BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(*this);
    res.BitField.SetBit(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res(*this);
    res.BitField.ClrBit(Elem);
    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{   
    TSet res(max(this->MaxPower, s.MaxPower));
    res.BitField = this->BitField & s.BitField;
    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res(this->MaxPower);
    res.BitField = ~this->BitField;
    return res;
}

// перегрузка ввода/вывода
//Enter numer of element, then enter ',' to continue or '.' to stop 
istream& operator>>(istream& istr, TSet& s) // ввод
{
    char sym = ',';
    size_t tmp;
    while (sym != '.')
    {
        istr >> tmp >> sym;
        if (tmp < s.MaxPower && tmp >= 0)
            s.InsElem(tmp);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    size_t i = 0;
    while(!s.IsMember(i))
        i++;
    ostr << i;
    for (++i; i < s.GetMaxPower(); i++)
        if (s.IsMember(i))
            ostr <<", "<< i;
    ostr << "." << endl;

    return ostr;
}
