#include "cparse.h"
#include <cstring>
#include <ios>

CParse::CParse(char *string, bool isOwner)
    : d(string)
    , owner(isOwner)
{
}

CParse::CParse(char *string)
    : d(string)
    , owner(false)
{
}

CParse::CParse(const char *string)
    : d(new char[strlen(string) + 1])
    , owner(true)
{
    strcpy(d, string);
}

CParse::~CParse()
{
    if (owner)
        delete[] d;
}

CParse CParse::parseFile(const std::string& filename)
{
    CParse out(0, true);

    size_t length;

    std::ifstream is;
    is.open(filename.c_str(), std::ios::binary );
    if (is.bad()) {
        out.d = 0;
        return out;
    }

    is.seekg (0, std::ios::end);
    length = is.tellg();
    is.seekg (0, std::ios::beg);

    out.d = new char [length];

    // read data as a block:
    is.read (out.d,length);
    is.close();
    return out;
}

CParse::CParse(const CParse& p) {
    d = p.d;
    owner = p.owner;
    p.owner = false;
}

int CParse::integer()
{
    while (*d != '-' && *d != '+' && ( *d < '0' || *d > '9')) {
        ++d;
    }

    int num = 0;
    bool flag = *d == '-';
    *d += flag;

    while (*d >= '0' && *d <= '9')
    {
        num *= 10;
        num += *d - '0';
        ++d;
    }

    return flag ? -num : num;
}
double CParse::real()
{
    while (*d != '-' && *d != '+' && ( *d < '0' || *d > '9'))
    {
        ++d;
    }

    bool flag = ( *d == '-' );
    if (flag)
    {
        ++d;
    }

    double num(*d - '0');
    ++d;

    for (double k = .1; *++d >= '0' && *d <= '9'; k /= 10)
    {
        num += k * (*d - '0');
    }
    if (*d != 'e' && *d != 'E')
    {
        return flag ? -num : num;
    }

    double k(*++d == '-' ? .1 : 10.);
    ++d;

    for (int e = (*d - '0') * 10 - '0' + d[1]; e; --e)
        num *= k;
    ++(++d);

    return flag ? -num : num;
}
float CParse::fixFloat8()
{
    int sign(1), expV(0);
    float expK(10.0f), pointK(1.0f), num(0.0f);
    bool exp(false);
    for (int i = 0; i != 8; ++d, ++i)
    {
        switch (*d)
        {
        case '-':
            if (i == 0)
            {
                sign = -1;
            }
            else
            {
                exp = true;
                expK = 0.1f;
            }
            break;
        case '+':
            if (i != 1)
            {
                exp = true;
            }
            break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            if (exp)
            {
                expV *= 10;
                expV += *d - '0';
            }
            else if (pointK == 1.0f)
            {
                num *= 10.f;
                num += *d - '0';
            }
            else
            {
                num += (*d - '0') * pointK;
                pointK /= 10.0f;
            }
            break;
        case '.':
            pointK = 0.1f;
            break;
        }
    }
    if (expV)
    {
        while (expV)
        {
            num *= expK;
            --expV;
        }
    }
    return num * sign;
}
std::string CParse::string()
{
    char* start(d);

    while (*d != '\n')
        ++d;

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i)
    {
        ret[i] = start[i];
    }
    ++d;

    return ret;
}
std::string CParse::word()
{
    while( *d == ' ' || *d == '\t' || *d == '\n')
        ++d;
    char* start = d;

    while ( *d != ' ' && *d != '\t' && *d != '\n')
        ++d;

    std::string ret(d - start, '\0');
    for (int i(0); start + i != d; ++i)
    {
        ret[i] = start[i];
    }
    ++d;

    return ret;
}
time_t CParse::time()
{
    if(*d == 'N' || *d == 'n')//Если заменено на None
    {
        while(*d != '\n') ++d;
        ++d;
        return 0;
    }
    time_t days = *d * 10 - '0' * 11 + d[1];
    d += 3;
    time_t months;
    switch(*d)
    {
    case 'J':
        months = d[1] == 'a' ? 0 : d[2] == 'l' ? 6 : 5;
        break;
    case 'F':
        months = 1;
        break;
    case 'M':
        months = d[2] == 'y' ? 4 : 2;
        break;
    case 'A':
        months = d[1] == 'p' ? 3 : 7 ;
        break;
    case 'S':
        months = 8;
        break;
    case 'O':
        months = 9;
        break;
    case 'N':
        months = 10;
        break;
    case 'D':
        months = 11;
        break;
    default:
        return 0;
    }
    d +=4;
    time_t years = *d * 10 - '0' * 11 + d[1];
    d += 3;
    time_t hours = *d * 10 - '0' * 11 + d[1];
    d += 3;
    time_t minutes = *d * 10 - '0' * 11 + d[1];
    d += 3;
    time_t seconds = *d * 10 - '0' * 11 + d[1];
    d += 5;

    //Учёт високосных годов и длинн месяцев
    days += months / 2 + ( months % 2 ) + ( months == 10 || months == 8 ) + ( months > 1 ? -2 : 0 ) + years / 4 ;

    //Секунд до московского(!) милениума + остальное время в секундах
    return 946670400 + seconds + minutes * 60 + hours * 3600 + days * 86400 + months * 2592000 + years * 31536000;
    //т.е следует понимать, что разбор верен только в московской часовой зоне.
}
void CParse::skipRow()
{
    if (!d)
    {
        return;
    }
    while (*d && *d != '\n')
    {
        ++d;
    }
    if (*d == '\n')
    {
        ++d;
    }
}

void CParse::skipTo(const char* dest)
{
    if (!d)
    {
        return;
    }
    bool founding(false);
    int i(0);
    while (!founding)
    {
        i = 0;
        founding = true;
        while (founding && dest[i])
        {
            founding = d[i] == dest[i];
            ++i;
        }
        ++d;
    }
    --d;
}

bool CParse::testPrew(const char *dest)
{
    if (!d)
    {
        return false;
    }
    if (!*dest)
    {
        return *d != '\0';
    }
    bool ret;
    int i(0);
    do
    {
        ret = d[i] == dest[i];
        ++i;
    }
    while (ret && d[i] && dest[i]);
    return ret;
}
