// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp) {
	if (mp < 0) {
		throw std::invalid_argument("Максимальная мощность множества не может быть отрицательной");
	}
	MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField) {
	MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf) {
	MaxPower = bf.GetLength();
}

TSet::operator TBitField() {
	TBitField result = BitField;
	return result;
}

int TSet::GetMaxPower(void) const { // получить макс. к-во эл-тов
	int power = MaxPower;
	return power;
}

int TSet::IsMember(const int Elem) const { // элемент множества?
	if (Elem < 0 || Elem >= MaxPower) {
		return 0;
	}
	int result = BitField.GetBit(Elem);
	return result;
}

void TSet::InsElem(const int Elem) { // включение элемента множества
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Включаемый элемент находится вне допустимых границ универса");
	}
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) { // исключение элемента множества
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Включаемый элемент находится вне допустимых границ универса");
	}
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) { // присваивание
	if (this != &s) {
		MaxPower = s.MaxPower;
		BitField = s.BitField;
	}
	return *this;
}

int TSet::operator==(const TSet& s) const { // сравнение
	if (MaxPower != s.MaxPower) {
		return 0;
	}

	if (BitField == s.BitField) {
		return 1;
	}
	else {
		return 0;
	}
}

int TSet::operator!=(const TSet& s) const { // сравнение
	if (*this == s) {
		return 0;
	}
	else {
		return 1;
	}
}

TSet TSet::operator+(const TSet& s) { // объединение
	int maxPower = MaxPower;
	if (s.MaxPower > MaxPower) {
		maxPower = s.MaxPower;
	}
	TSet result(maxPower);
	result.BitField = BitField | s.BitField;
	return result;
}

TSet TSet::operator+(const int Elem) { // объединение с элементом
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Элемент для объединения находится вне границ универса");
	}
	TSet result(*this);
	result.InsElem(Elem);
	return result;
}

TSet TSet::operator-(const int Elem) { // разность с элементом
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Элемент для разности находится вне границ универса");
	}
	TSet result(*this);
	result.DelElem(Elem);
	return result;
}

TSet TSet::operator*(const TSet& s) { // пересечение
	int maxPower = MaxPower;
	if (s.MaxPower > MaxPower) {
		maxPower = s.MaxPower;
	}
	TSet result(maxPower);
	result.BitField = BitField & s.BitField;
	return result;
}

TSet TSet::operator~(void) { // дополнение
	TSet result(MaxPower);
	result.BitField = ~BitField;
	return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) { // ввод
	istr >> s.BitField;
	s.MaxPower = s.BitField.GetLength();
	return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) { // вывод
	ostr << "{ ";
	int first = 1;

	for (int i = 0; i < s.MaxPower; i++) {
		if (s.IsMember(i) == 1) {
			if (first == 0) {
				ostr << ", ";
			}
			ostr << i;
			first = 0;
		}
	}
	ostr << " }";
	return ostr;
}
