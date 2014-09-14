/*
����� ������������� ����������� ������� ������ ������������ (double) �����,
� ���������� �����. ������ ����� �������� � ������, ������ ��� � ������ ���
�������� ����� �� ���������� ��������. ��� � ������ ����� ���� ��������.
�������������� ���������.
����� ��������, ��� ����������� ����� � ����� ������� ������ �� 1 ������
��� ����������� ����� �������� � ��������� �� begin �� end. ����� ���������
�������� � ������������, �� ������� ������ ����� ������ ������������� ������.
*/
#ifndef CINTERVAL_H
#define CINTERVAL_H
#include <iostream>
#include <limits>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <cmath>

class CInterval
{
    size_t s;
    double st;
    double min;
public:
    CInterval();
    CInterval(double begin, double step, double end);
    CInterval(double begin, double step, size_t size);
    CInterval(double begin, double step, int size);
    static CInterval interval(double begin, double end, size_t size);
    inline double first() const { return min; }
    inline double last() const { return min + st * (s - 1); }
    inline double step() const { return st; }
    inline bool isEmpty() const { return static_cast<bool>(s); }
    inline size_t size() const { return s; }

    inline double operator[] (int i) const { return min + st * i; }
    inline bool operator !=(const CInterval& v) const { return v.s != s || v.min != min || v.st != st; }
    inline bool operator ==(const CInterval& v) const { return v.s == s && v.min == min && v.st == st; }

    friend std::ostream& operator << (std::ostream& out, const CInterval& i);
    friend std::istream& operator >> (std::istream& in, CInterval& i);
    friend QTextStream& operator << (QTextStream& out, const CInterval& i);
    friend QTextStream& operator >> (QTextStream& in, CInterval& i);
    friend QDataStream& operator << (QDataStream& out, const CInterval& i);
    friend QDataStream& operator >> (QDataStream& out, CInterval& i);
    friend QDebug operator << (QDebug out, const CInterval& i);
};

std::ostream& operator << (std::ostream& out, const CInterval& i);
std::istream& operator >> (std::istream& in, CInterval& i);
QTextStream& operator << (QTextStream& out, const CInterval& i);
QTextStream& operator >> (QTextStream& in, CInterval& i);
QDataStream& operator << (QDataStream& out, const CInterval& i);
QDataStream& operator >> (QDataStream& out, CInterval& i);
QDebug operator << (QDebug out, const CInterval& i);

#endif // CINTERVAL_H
