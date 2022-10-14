// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		this->BitLen = len;
		this->MemLen = (len) / (sizeof(TELEM) * 8) + 1;
		this->pMem = new TELEM[MemLen];
		memset(this->pMem, 0, MemLen * sizeof(TELEM));
	}
	else
	{
		throw exception("length can`t be negative or 0");
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	this->BitLen = bf.BitLen;
	this->MemLen = bf.MemLen;
	this->pMem = new TELEM[MemLen];
	memcpy(this->pMem, bf.pMem, this->MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] this->pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	size_t indBit = n % (sizeof(TELEM) * 8);
	TELEM mask = 1;
	return (mask << indBit);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < this->BitLen)
	{
		size_t indByte = this->GetMemIndex(n);
		this->pMem[indByte] |= this->GetMemMask(n);
	}
	else
	{
		throw exception("wrong index");
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < this->BitLen)
	{
		size_t indByte = this->GetMemIndex(n);
		this->pMem[indByte] &= ~this->GetMemMask(n);
	}
	else
	{
		throw exception("wrong index");
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < this->BitLen)
	{
		size_t indByte = this->GetMemIndex(n);
		size_t indBit = n % (sizeof(TELEM) * 8);
		TELEM tmp = this->pMem[indByte];
		tmp &= (1 << indBit);

		if (tmp != 0)
			return 1;
		else
			return 0;
	}
	else
	{
		throw exception("wrong index!");
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	delete[] this->pMem;
	this->BitLen = bf.BitLen;
	this->MemLen = bf.MemLen;
	this->pMem = new TELEM[MemLen];
	memcpy(this->pMem, bf.pMem, this->MemLen * sizeof(TELEM));

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	size_t iterNum = this->MemLen * sizeof(TELEM) / sizeof(size_t);
	if (iterNum > 0)
	{
		for (size_t i = 0; i < iterNum; i += sizeof(size_t) / sizeof(TELEM))
			if (*(size_t*)&this->pMem[i] != *(size_t*)&bf.pMem[i])
				return 0;
	}
	for (size_t i = iterNum * sizeof(size_t) / sizeof(TELEM); i < this->MemLen; i++)
		if (this->pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf)
		return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField res(max(this->BitLen, bf.BitLen));
	size_t iterNum = min(this->MemLen, bf.MemLen) * sizeof(TELEM) / sizeof(size_t);
	if (iterNum > 0)
	{
		for (size_t i = 0; i < iterNum; i += sizeof(size_t) / sizeof(TELEM))
			*(size_t*)&res.pMem[i] = *(size_t*)&this->pMem[i] | *(size_t*)&bf.pMem[i];
	}
	for (size_t i = iterNum * sizeof(size_t) / sizeof(TELEM); i < min(this->MemLen, bf.MemLen); i++)
		res.pMem[i] = this->pMem[i] | bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField res(max(this->BitLen, bf.BitLen));
	size_t iterNum = min(this->MemLen, bf.MemLen) * sizeof(TELEM) / sizeof(size_t);
	if (iterNum > 0)
	{
		for (size_t i = 0; i < iterNum; i += sizeof(size_t) / sizeof(TELEM))
			*(size_t*)&res.pMem[i] = *(size_t*)&this->pMem[i] & *(size_t*)&bf.pMem[i];
	}
	for (size_t i = iterNum * sizeof(size_t) / sizeof(TELEM); i < min(this->MemLen, bf.MemLen); i++)
		res.pMem[i] = this->pMem[i] & bf.pMem[i];

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	size_t iterNum = this->MemLen * sizeof(TELEM) / sizeof(size_t);
	if (iterNum > 0)
	{
		for (size_t i = 0; i < iterNum; i += sizeof(size_t) / sizeof(TELEM))
			*(size_t*)&this->pMem[i] = ~*(size_t*)&this->pMem[i];
	}
	size_t i = iterNum * sizeof(size_t) / sizeof(TELEM);
	for (; i < this->MemLen; i++)
		this->pMem[i] = ~this->pMem[i];
	// зануление неиспользуемых битов битовой маской
	pMem[i - 1] &= ~((~(TELEM)0) << ((sizeof(TELEM) * 8) - (this->MemLen * sizeof(TELEM) * 8 - this->BitLen))); 
	
	return *this;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	unsigned int tmp;
	for (size_t i = 0; i < bf.GetLength(); i++)
	{
		cout << "bit[" << i << "] = ";
		do
		{
			cin >> tmp;
		} while (tmp != 1 && tmp != 0);

		if (tmp == 1)
			bf.SetBit(i);
	}

	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (size_t i = 0; i < bf.GetLength(); i++)
		ostr << bf.GetBit(i);
	return ostr;
}
