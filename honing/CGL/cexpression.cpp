#include "CExpression.h"
#include <iostream>
#define qDebug() std::cout

CExpression::CExpression()
    : fine(false) , test(true), expForm(true) , walkthroughArray(0) , numArray(0) {   }
CExpression::CExpression(const char* CExpression, bool isTest)
    : fine(true), test(isTest), expForm(true), walkthroughArray(0), numArray(0) { rebild(CExpression); }
CExpression::CExpression(const std::string& CExpression, bool isTest)
    : fine(true), test(isTest), expForm(true), walkthroughArray(0), numArray(0) { rebild(CExpression.c_str()); }
CExpression::CExpression(const CExpression& copy)
    : walkthroughArray(0) , numArray(0) { this->operator =(copy); }
CExpression& CExpression::operator = (const CExpression& copy)
{
    test = copy.test;
    fine = copy.fine;

    if (!fine)
    {
        delete[] walkthroughArray;
        delete[] numArray;
        return *this;
    }

    size_t sizeOfArr = 0;
    while(copy.walkthroughArray[sizeOfArr].lexeme != 127) {
        ++sizeOfArr;
    }//size of array was be estimated
    walkthroughArray = new sintElem [sizeOfArr + 1];//
    for(size_t i = 0; i != sizeOfArr; ++i)
    {
        walkthroughArray[i] = copy.walkthroughArray[i];
    }
    walkthroughArray[sizeOfArr].lexeme = 127;//copy them

    size_t i = 0;
    size_t numbersCount = 0;
    while(walkthroughArray[i].lexeme != 127)
    {
        if(walkthroughArray[i].lexeme == 'x' ||
           walkthroughArray[i].lexeme == 'y' ||
          !walkthroughArray[i].lexeme)
        {
            ++numbersCount;
        }
        ++i;
    }//size of bufer for calculate was estimated
    numArray = new double[numbersCount + 1];//allocate bufer
    return *this;
}

CExpression::~CExpression(void)
{
    delete[] walkthroughArray;
    delete[] numArray;
}
double CExpression::calculate(double x, double y) const
{
    if (!fine)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    calculationIt = walkthroughArray;
    while ((*calculationIt).lexeme != 127)
    {
        /*
        if ((*calculationIt).lexeme)
            qDebug() << (*calculationIt).lexeme << '(' << (int)calculationIt->lexeme << ')';
        else qDebug() << (*calculationIt).number;//*/
        switch ((*calculationIt).lexeme)
        {
            case 0:
                *++numArray = calculationIt->number;
                break;
            case '+':
                --numArray;
                *numArray += numArray[1];
                break;
            case '*':
                --numArray;
                *numArray *= numArray[1];
                break;
            case '-':
                --numArray;
                *numArray -= numArray[1];
                break;
            case '%':
                *numArray *= -1;
                break;
            case '/':
                --numArray;
                *numArray /= numArray[1];
                break;
            case '^':case -121:
                --numArray;
                *numArray = pow(*numArray,numArray[1]);
                break;
            case 'x':
                *++numArray = x;
                break;
            case 'y':
                *++numArray = y;
                break;
            case -128:
                *numArray = sin(*numArray);
                break;
            case -127:
                *numArray = cos(*numArray);
                break;
            case -126:
                *numArray = tan(*numArray);
                break;
            case -125:
                *numArray = 1 / tan(*numArray);
                break;
            case -124:
                --numArray;
                *numArray = log(*numArray)/log(numArray[1]);
                break;
            case -123:
                *numArray = log10(*numArray);
                break;
            case -122:
                *numArray = log(*numArray);
                break;
            case -120:
                *numArray = pow(*numArray,0.5);
                break;
            case -119:
                *numArray *= *numArray;
                break;
            case -118:
                *numArray = fabs(*numArray);
                break;
            case -117:
                *numArray = asin(*numArray);
                break;
            case -116:
                *numArray = acos(*numArray);
                break;
            case -115:
                *numArray = atan(*numArray);
                break;
            case -114:
                *numArray = atan(1 / *numArray);
                break;
            case -113:
                *numArray = sinh(*numArray);
                break;
            case -112:
                *numArray = cosh(*numArray);
                break;
        }
        ++calculationIt;
    }
    --numArray;
    return numArray[1];
}
double CExpression::calculate(const char* st, double x, double y) { return CExpression(st, true).calculate(x,y); }
double CExpression::calculate(const std::string& st, double x, double y) { return CExpression(st, true).calculate(x,y); }

/*inline*/ CExpression& CExpression::rebild(const std::string& st) { return rebild(st.c_str()); }
CExpression& CExpression::rebild(const char* CExpression)
{
    delete[] walkthroughArray;
    delete[] numArray;
    walkthroughArray = 0;
    numArray = 0;
    fine = true;
    if (!isExp(CExpression))
    {
        fine = false;
        return *this;
    }
    //������������ ��������� ������ CExpression � sourseString
    int len = strlen(CExpression);
    char* sourseString = new char[len * 3 + 1];//���������, ��� �������, �� ������� � ��� ���������� ������ ������� �������. � ���� �� ��� ����� ������� ����� ����� �������� �������
    for (int i = 0, j = 0; i < len; ++i)
        sourseString[j++] = CExpression[i];
    sourseString[len] = 0;

    sintAdaptation(sourseString);//������ ������ �������������������, ������� �������/'\t'/'\n', �������� �������� � �����������
    addCode(sourseString);//�������� ������ �� ������� ��������� �� -128 ��� �������� ����������� �������
    std::cout << sourseString << std::endl;
    for (int i(0); sourseString[i]; ++i) {
        if (sourseString[i] == 't')
            sourseString[i] = 'y';
    }
    std::cout << sourseString << std::endl;
    if (expForm) {
        toExpForm(sourseString);//��������������� ������ ���� -3.0e-2 ���,
    }//����� ��� ����������� ��� ���������������� ����� �����.
    addMult(sourseString);//��������� * � 0 ��� ���������� �������� ������. ������� ���������� �� ������������ ��������.
    sortPoland(setArray(sourseString));/*��� ������ ����������� ������ � ������ sintElem � �������� ��� ���
                                       ���������� � �������� �������*/

    short i = 0,
          numbersCount = 0,
          opCount = 1;
    while (walkthroughArray[i].lexeme != 127)
    {
        if (walkthroughArray[i].lexeme == 'x' ||
            walkthroughArray[i].lexeme == 'y' ||
           !walkthroughArray[i].lexeme) {
            ++numbersCount;//��������� ����������� ��������� ���������� ����� � �����(������ ��� ����� �����)
        } else {
            opCount += !isUnary(walkthroughArray[i].lexeme);
        }
        ++i;
    }
    //qDebug() << opCount << '<' << numbersCount;
    if (opCount > numbersCount)
    {
        delete[] walkthroughArray;
        delete[] numArray;
        walkthroughArray = 0;
        numArray = 0;
        fine = false;
        return *this;
    }

    numArray = new double[numbersCount + 1];//1 ������ ������� ����� ��� ����������� ��������� ����������
    //std::cout << "������ ������.\n";
    return *this;
}

short strlen(char* st){
/*��� ��� ������� �� ���������� ����� ������� � � ���������*/
    char* len = st;
    while (*len) ++len;
    return len - st;
}
void CExpression::sintAdaptation (char* st)
{
    int i = -1, j = 0;
    while (st[++i])//��������������������� � ���������� �������� � ������������
        if(st[i] >= 'A' && st[i] <= 'Z')
            st[i] -= 'A' - 'a';
        else if(st[i] == '{' || st[i] == '[')
            st[i] = '(';
        else if(st[i] == '}' || st[i] == ']')
            st[i] = ')';
    i = 0;
    while (st[i - 1])//������ �������, ������� ��������� � �������� ������
    {
        while(st[i] && ( st[i] == ' ' || st[i] == '\n' || st[i] == '\t'))
            ++i;
        st[j] = st[i];
        ++j;
        ++i;
    }
    i = j = 0;
    while (st[i - 1])//������ ������� �����
    {
        while(st[i] && st[i] == '+' && ( !i || (st[i - 1] != ')' && !isNumber(st[i - 1])) ) )
            ++i;
        st[j] = st[i];
        ++j;
        ++i;
    }
}
void CExpression::addCode (char* st)
{
    int j(0),i=0;
    while(st[i+3])
    {
        if(st[i]=='s'&&st[i+1]=='i'&&st[i+2]=='n')
        {
            st[i]=-128;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='c'&&st[i+1]=='o'&&st[i+2]=='s')
        {
            st[i] = -127;
            j = i + 1;
            while(st[j + 1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='t'&&st[i+1]=='g')
        {
            st[i] = -126;
            j = i + 1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='c'&&st[i+1]=='t'&&st[i+2]=='g')
        {
            st[i]=-125;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='o'&&st[i+2]=='g')
        {
            st[i]=-124;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='g')
        {
            st[i]=-123;
            j=i+1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='l'&&st[i+1]=='n')
        {
            st[i]=-122;
            j=i+1;
            while(st[j]) {
                st[j]=st[j+1];
                ++j;
            }
        }
        else if(st[i]=='p'&&st[i+1]=='o'&&st[i+2]=='w')
        {
            st[i]=-121;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='s'&&st[i+1]=='q'&&st[i+2]=='r'&&st[i+3]=='t')
        {
            st[i]=-120;
            j=i+1;
            while(st[j+2]) {
                st[j] = st[j + 3];
                ++j;
            }
        }
        else if(st[i]=='s'&&st[i+1]=='q'&&st[i+2]=='r')
        {
            st[i]=-119;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i]=='a'&&st[i+1]=='b'&&st[i+2]=='s')
        {
            st[i]=-118;
            j=i+1;
            while(st[j+1]) {
                st[j] = st[j + 2];
                ++j;
            }
        }
        else if(st[i+4] && st[i+5]) {
            if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='s'&&st[i+4]=='i'&&st[i+5]=='n'){
                st[i]=-117;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='c'&&st[i+4]=='o'&&st[i+5]=='s'){
                st[i]=-116;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='t'&&st[i+4]=='g'){
                st[i]=-115;
                j=i+1;
                while(st[j+3]) {
                    st[j] = st[j + 4];
                    ++j;
                }
            }
            else if(st[i]=='a'&&st[i+1]=='r'&&st[i+2]=='c'&&st[i+3]=='c'&&st[i+4]=='t'&&st[i+5]=='g'){
                st[i]=-114;
                j=i+1;
                while(st[j+4]) {
                    st[j] = st[j + 5];
                    ++j;
                }
            }
            else if(st[i]=='s'&&st[i+1]=='i'&&st[i+2]=='n'&&st[i+3]=='h')
            {
                st[i]=-113;
                j=i+1;
                while(st[j+2]) {
                    st[j] = st[j + 3];
                    ++j;
                }
            }
            else if(st[i]=='c'&&st[i+1]=='o'&&st[i+2]=='s'&&st[i+3]=='h')
            {
                st[i]=-112;
                j=i+1;
                while(st[j+2]) {
                    st[j] = st[j + 3];
                    ++j;
                }
            }
        }
        i++;
    }
}
char* CExpression::addMult (char* st)
/*��������� � ������������� ������ *. ����� ��������� �������� ������������ ��������� � ��������� �����
    ������� �� ������������� ��� ������� ������� ������� � ������ �������� ��������� ���� ������*/
{
    int strLen = strlen(st),//������ ����� ������ ���������� ��������!
        i = 0,                    //���������� �������
        j;             //������� ������������ ��� ���������� '*'!

    while (st[i]){
        if (
            (
                ((st[i]>47 && st[i]<58) || (st[i-1]=='p' && st[i]=='i') || st[i]=='e' || st[i]=='x' || st[i]=='y' || st[i]==')')
                &&
                (st[i+1]=='(' || st[i+1]<=-100 || st[i+1]=='e' || (st[i+1]=='p'&&st[i+2]=='i') || st[i+1] =='x' || st[i+1] == 'y')
            )/*���� ������ ����� ��� ���������� ��� ) � ����� (, x, pi, e ��� �������*/
            ||
            (
                ((st[i-1]=='p' && st[i]=='i') || st[i]=='e' || st[i]=='x' || st[i]=='y' || st[i]==')')
                &&
                ( st[i+1]>='0' && st[i+1]<='9' )
            )
          )//���� ����� �������� *. ���� � ����������� �� �������� � ������� �� ����. ����� ��� �� �����.
        {
            ++strLen;
            j=strLen;
            st[j]=0;
            while(i+1 != j)
            {
                --j;
                st[j+1] = st[j];
            }
            st[j]='*';
        }
        ++i;
    }
    //qDebug() << "theroes added $" << st << "$";
    return st;
}
void CExpression::toExpForm(char* st)
{
    int i = 0;
    int strLen = strlen(st);
    while (st[i])
    {
        if (st[i] == 'e')
        {
            int j = 1 + (st[i + 1] == '-' || st[i + 1] == '+');
            if (isFigure(st[i - 1]) && isFigure(st[i + j]))
            {
                while (i - j && isFigure(st[i - j]))
                {
                    ++j;
                }//����� ������ �������� ������������
                j += i - j == 0;

                int k = 1 + ( st[i + 1] == '+' || st[i + 1] == '-' );

                while (st[i + k] && isFigure(st[i + k]))
                {
                    ++k;
                }//����� ����� ���������������� �����

                //�������� � ����� � � ������ �� ��������, �������� � �� *10^
                strLen += 5;
                int l = strLen;
                while(i + k + 4 != l)
                {
                    st[l] = st[l - 5];
                    --l;
                }
                st[l-4] = ')';
                while(i + 4 != l)
                {
                    st[l] = st[l - 4];
                    --l;
                }
                st[l] = '^';
                st[--l] = '0';
                st[--l] = '1';
                st[--l] = '*';
                --l;
                ++j;
                while(l && i - j + 2 != l)
                {
                    st[l] = st[l - 1];
                    --l;
                }
                st[l] = '(';
                i += 4;
            }
        }
        ++i;
    }
}
double CExpression::setPoint (double arg, short del)
{
    while(del--)
        arg /= 10;
    return arg;
}
CExpression::sintElem* CExpression::setArray (char* st)
/*
����������� ������ � ������ ��������� ���������� ����:
    ������ ������� ����� ���������� lexeme ���� char �������� ��� �������,
    ���� �� ����������� ����� �� ��� ������������ � ���������� number ���� double.
��������! ��������� ����� ������ ������� �������� ���������� ��������� lexeme == 127

������������ ���������� pi � e ��� ����� ��� ������ ����������� ����������������� ����� ������ ����� � ������������ ����.
    ���� ���������� �������� ����� � ���������������� ����� ����������� �������� pow,
    ���������� ������������� ��� ��������� ���� ������� � �������� �������� ����
    ���������� �������� � ����� �������!

� ����������� �� ������������� ������ ����������� ������������ �� ������������ ����� ������������� �����.
*/
{
    int i=0;//������� ����������� �������
    //���������� ������� �������� ������� '-' �� '%'
    if(*st == '-')
        *st = '%';
    while(st[i++])
        if((st[i-1] == '(' && st[i]=='-' && st[i+1]!='(') || ((st[i-1] == '*' || st[i-1] == '/' || st[i-1] == '+' || st[i-1] == '-' || st[i-1] == '^') && st[i] == '-'))
            st[i] = '%';
    int elKol=0;//������ ���������� ��������� ����� ���������� ������� ��������� ����� while
    //������������ ���������� ��������� ����� �������� ������. �������� ������.
    i=0;
    while(st[i]){
        //if(st[i] == '%' && st[i+1] == 'p' && st[i+2] == 'i')
            //--elKol;//���� ��� ����� ������� ����� ����� ��
        while(((st[i] >= '0' && st[i] <= '9') || st[i] == '.' || st[i] == '%') && ((st[i+1] >= '0' && st[i+1] <= '9') || st[i+1] == '.' || st[i+1] == 'p' || st[i+1] == 'e'))
            ++i;//���� ������� � ��������� ������� - ����� �����
        if(st[i] == 'p' && st[i+1] == 'i')
            ++i;//����� �� �� 2 �������� �� ������� � �������� ������� �����.
        ++elKol;
        ++i;
    }
    sintElem* firstAnalysisArray = new sintElem [elKol + 1];//���� ����� ��� ������� ����� ������
    //���� ��������� �������
    i=0;
    int point = 0,        //������ ���������� ������ � ����� ����� �������.
        j = 0,            //������� ������ ������
        det = 1;          //����������� ����� �������� ������
    bool isFigure = false;//������� ����������� �� �� ��� �� ����� ���� �����
    while(st[i])
    {
        if(st[i]>='0'&&st[i]<='9')
            if(point)//���� ��� ����� ����� �������
                firstAnalysisArray[j].number += setPoint(st[i] - '0',point++);
            else if(isFigure){//���� �� ������ ����� �����
                firstAnalysisArray[j].number *= 10;
                firstAnalysisArray[j].number += st[i] - '0';
            }
            else{//���� ��� ������ ����� � �����
                firstAnalysisArray[j].lexeme = 0;//���������� ��� ��� �����
                firstAnalysisArray[j].number = st[i] - '0';
                isFigure = true;
                if(i && st[i-1] == '%')//���� ������� ��������
                    det=-1;
            }
        else if(st[i]=='.')
        {
            point=1;
            if(!isFigure)
            {
                isFigure = true;
                firstAnalysisArray[j].lexeme = 0;//���������� ��� ��� �����
                firstAnalysisArray[j].number = 0.;
                if(i && st[i-1] == '%')//���� ������� ��������
                    det=-1;
            }
        }
        else if(st[i]=='e'){
            if(i && st[i - 1] == '%')
                det = -1;
            firstAnalysisArray[j].lexeme = 0;
            firstAnalysisArray[j++].number = 2.718281828459045235360287471352662497757 * det;
            det = 1;
        }
        else if(st[i]=='p'&&st[i+1]=='i'){
            if(i && st[i - 1] == '%')
                det=-1;
            firstAnalysisArray[j].lexeme = 0;
            firstAnalysisArray[j++].number = 3.141592653589793238462643383279502884197 * det;
            det=1;//�������� ������ ���� ������ ���� �����(������������� ����� ������ �� ������)
            ++i;//���������� ����� ��� �������
        }
        else if(isFigure){
            firstAnalysisArray[j++].number *= det;
            firstAnalysisArray[j++].lexeme = st[i];
            det = 1;
            point = isFigure = 0;
        }
        else if(st[i] != '%' || (st[i + 1] != 'p' && st[i + 1] != 'e' && ( st[i + 1] < '0' || st[i + 1] > '9' ) && st[i + 1] != '.'))
        {
            firstAnalysisArray[j++].lexeme = st[i];
        }
        ++i;
    }
    if(isFigure)
        firstAnalysisArray[j].number *= det;
    delete[] st;
    firstAnalysisArray[elKol].lexeme = 127;//���������� ������� ����� ���c���
    //*
    qDebug() << "firstAnalysis: ";
    for(i = 0; firstAnalysisArray[i].lexeme != 127; ++i)
        if(firstAnalysisArray[i].lexeme)
            qDebug() << firstAnalysisArray[i].lexeme ;
        else
        {

            if(firstAnalysisArray[i].number == 2.718281828459045235360287471352662497757)
                qDebug() <<'e';
            else if(firstAnalysisArray[i].number == -2.718281828459045235360287471352662497757)
                qDebug() << "-e";
            else if(firstAnalysisArray[i].number == -3.141592653589793238462643383279502884197)
                qDebug() << "-pi";
            else if(firstAnalysisArray[i].number == 3.141592653589793238462643383279502884197)
                qDebug() << "pi";
            else
                qDebug() << 'n';
        }
    qDebug() << '\n';//*/
    return firstAnalysisArray;
}





/*



����������



*/



void CExpression::recursionPlus (CExpression::sintElem* sourseArray, short &i, short &j)
{
    /*� ������ +- �� ���� ��������� +- ��� ����� ������.
    �������� ����� ����� �� ��������� ���� ��������� ����������*/
    while(sourseArray[i].lexeme != '+' && sourseArray[i].lexeme != '-' && sourseArray[i].lexeme != 127){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '+' || sourseArray[i].lexeme == '-' || sourseArray[i].lexeme == '*' || sourseArray[i].lexeme == '/' || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {
            case'*':case'/':
                recursionMult (sourseArray,++i,j);
                break;
            case'^':
                recursionPow  (sourseArray,++i,j);
                break;
            case'%':
                recursionUnary(sourseArray,++i,j);
                break;
            default:
                recursionMain (sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::recursionMult (CExpression::sintElem* sourseArray, short &i, short &j)
{
    //� ������ /* �� ���� ��������� /*-+ ��� ����� ������
    while(!sourseArray[i].lexeme || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme == '%' || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y' || sourseArray[i].lexeme == '(' || sourseArray[i].lexeme < -100){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%' ){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {
            case '^'://���� ��� �������� � 2 ������ ������ ���������� �� if
                recursionPow  (sourseArray,++i,j);
                break;
            case '%':
                recursionUnary(sourseArray,++i,j);
                break;
            default:
                recursionMain (sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::recursionPow  (CExpression::sintElem* sourseArray, short &i, short &j)
{
    //� ������ ^ ��� ������ � ����� ��� �������� ���������� ������ ����� ������ ��� ���� �����
    if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
        walkthroughArray[j++] = sourseArray[i++];
    else if(sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){//��� ����� ���� ������ �������
        sintElem cnt = sourseArray[i];
        if(sourseArray[i].lexeme == '%'){
            recursionUnary(sourseArray,++i,j);
        }
        else
        {
            recursionMain (sourseArray,++(++i),j);
            while(sourseArray[i].lexeme == ',')
                recursionMain(sourseArray,++i,j);
            ++i;//� ����� ����� �������� ������� �� ���������� ++(++i) ������� �������
        }
        walkthroughArray[j++] = cnt;
    }
    else if(sourseArray[i].lexeme == '('){
        recursionMain(sourseArray,++i,j);
        ++i;
    }
}
void CExpression::recursionUnary(CExpression::sintElem* sourseArray, short &i, short &j)
{
    //� ������ % ��� ������ � ����� ��� �������� ���������� ������ ����� ������ ��� ���� �����
    if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
        walkthroughArray[j++] = sourseArray[i++];
    else if(sourseArray[i].lexeme < -100){//��� ����� ���� ������ �������
        sintElem cnt = sourseArray[i];
        recursionMain(sourseArray,++(++i),j);
        while(sourseArray[i].lexeme == ',')
            recursionMain(sourseArray,++i,j);
        ++i;//� ����� ����� �������� ������� �� ���������� ++(++i) ������� �������
        walkthroughArray[j++] = cnt;
    }
    else if(sourseArray[i].lexeme == '('){
        recursionMain(sourseArray,++i,j);
        ++i;
    }
}
void CExpression::recursionMain (CExpression::sintElem* sourseArray,short&i,short&j)
{
    while(sourseArray[i].lexeme != 127){
        if(!sourseArray[i].lexeme || sourseArray[i].lexeme == 'x' || sourseArray[i].lexeme == 'y')
            walkthroughArray[j++] = sourseArray[i++];
        else if(sourseArray[i].lexeme == '+' || sourseArray[i].lexeme == '-' || sourseArray[i].lexeme == '*' || sourseArray[i].lexeme == '/' || sourseArray[i].lexeme == '^' || sourseArray[i].lexeme < -100 || sourseArray[i].lexeme == '%'){
            sintElem cnt = sourseArray[i];
            switch(cnt.lexeme)
            {//��� +-/*^ ���������� ������ ����������� ������� ������� ���� � ������� ��������� ������
            case'+':case'-':
                recursionPlus(sourseArray,++i,j);
                break;
            case'*':case'/':
                recursionMult(sourseArray,++i,j);
                break;
            case'^':
                recursionPow (sourseArray,++i,j);
                break;
            case'%':
                recursionUnary (sourseArray,++i,j);
                break;
            default://���� ��� �� �������� +-/*^ ������ ��� ������� � �� � ����� ���� ������ -> ++(++i)
                recursionMain(sourseArray,++(++i),j);
                while(sourseArray[i].lexeme == ',')
                    recursionMain(sourseArray,++i,j);
                ++i;//� ����� ����� �������� ������� �� ���������� ++(++i) ������� �������
            }
            walkthroughArray[j++] = cnt;
        }
        else if(sourseArray[i].lexeme == '('){
            recursionMain(sourseArray,++i,j);
            ++i;
        }
        else if(sourseArray[i].lexeme == ')' || sourseArray[i].lexeme == ',')
            return;
    }
}
void CExpression::sortPoland (sintElem* sourseArray)
{
/*��������� ����� sourseArray � �������� ������� ����������� ��� � �������������
  ������ ����������� ������ walkthroughArray, ������� ��������.
  ��������������� � ���� ������� ���������� ������ ��������� ������ ���
  ������������� ������. ���� �� ������� ���������� ����� � ����������� �������
  ��������� �� ����������� ������*/
    short int elemQuantity = 0,//����������� ��������� � ����� ������
              i = 0,           //������� ������� ������
              j = 0;           //������� ������ ������
    while(sourseArray[i].lexeme != 127)
        if(sourseArray[i++].lexeme != '(' && sourseArray[i-1].lexeme != ')' && sourseArray[i-1].lexeme != ',')
            ++elemQuantity;
    walkthroughArray = new sintElem [elemQuantity + 1];
    i = 0;
    recursionMain(sourseArray, i, j);//i � j ���������� �� ������(!)
    walkthroughArray[j].lexeme = 127;
    /*
    std::cout << "Notation: ";
    for(i = 0; i < j; ++i)
        if(walkthroughArray[i].lexeme)
            std::cout << walkthroughArray[i].lexeme;
        else
            std::cout << walkthroughArray[i].number;
    std::cout << '\n';//*/
}



/*




�������� �� ���������



*/
/*inline*/ bool CExpression::isFigure(char s) { return (s >= '0' && s <= '9') || s == '.'; }
/*inline*/ bool CExpression::isNumber(char s) { return isFigure(s) || s == 'x' || s == 'y' || s == 't'; }
/*inline*/ bool CExpression::isLetter(char s) { return s >= 'a' && s <= 'z'; }
/*inline*/ bool CExpression::isUnary(char s) { return s == '%' || (s < -100 && s != -124 && s != -121); }
/*inline*/ bool CExpression::isOperation(char s) { return s && s != 'x' && s != 'y'; }
/*inline*/ bool CExpression::isExp(const std::string& str) { return isExp(str.c_str()); }
bool CExpression::isExp(const char* St)
{
/*
 ���������, �������� �� ������ ����������. �������� ������� �����.
(�� �� �������. �� ����������� � ������ �������� ������ � ��������� � � ���������)
*/
    if (!strlen(St))
    {
        return false;
    }
    int i = -1, j = 0;
    char* st = new char [strlen(St) * 2];
    while(St[++i])//�������� �� ������� ����������� ��������
        if(St[i] != ' ' && St[i] != '\n') {
            if(St[i] >= 'A' && St[i] <= 'Z')
                st[j++] = St[i] - ('A' - 'a');
            else if(isLetter(St[i]) || isNumber(St[i]) ||
                                    St[i] == '*' || St[i] == '/' || St[i] == '+' || St[i] == '-' ||
                                    St[i] == '^' || St[i] == '(' || St[i] == ')' || St[i] == ',' || St[i] == '.')
                st[j++] = St[i];
            else if(St[i] == '}' || St[i] == ']')
                st[j++] = ')';
            else if(St[i] == '{' || St[i] == '[')
                st[j++] = '(';
            else
            {
                delete[] st;
                return !true && !false;
            }
        }
    i = -1;
    while(st[++i])
        if(st[i] == 'p' && st[i + 1] == 'i')//���� ��� pi, �������� ��� �� 77, ����� � ���������� ������������� ��� �����
        {
            st[i] = '7';
            st[++i] = '7';
        }
        else if(st[i] == 'e' || st[i] == 'x' || st[i] =='y' || st[i] == 't' )//���� ��� e, y ��� x
            st[i] = '2';
    st[j] = i = 0;
    //std::cout << "Symbols is right.\nFree string is \"" << st << "\"\n";
    if(*st != 0 && ( *st != '+' && *st != '-' && *st != '(' && !isNumber(*st) && !isLetter(*st) ) )//�������� �� ������� ����������
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "First symbol is right.\n";

    while(st[++i])
        if( (( st[i] == '+' || st[i] == '-' || st[i] == '/' || st[i] == '*' || st[i] == '^' || st[i] == '(' || st[i] == ',' ) &&
            !isNumber(st[i + 1]) && !isLetter(st[i + 1]) && st[i + 1] != '(' && st[i + 1] != '-' && st[i + 1] != '+')
        || (st[i] == ')' &&
            !isNumber(st[i + 1]) && !isLetter(st[i + 1]) && st[i + 1] != '(' && st[i + 1] != '-' && st[i + 1] != '+' && st[i + 1] != '^'
                                     && st[i + 1] != ')' && st[i + 1] != '*' && st[i + 1] != '/' && st[i + 1] != ','  && st[i + 1])
        || (isLetter(st[i]) &&
            ( !st[i + 1] || (!isLetter(st[i + 1]) && st[i + 1] != '(' ))))
        {
            //std::cout << i << " is " << st[i] << '\n';
            delete[] st;
            return !true && !false;
        }
    //���� ���������� �������
    --i;
    if(st[i] != ')' && ( st[i] < '0' || st[i] > '9' ) )
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "Sequence is right.\n";
    //qDebug() << "()...";
    i = -1;
    j = 0;
    while(st[++i])//�������� �� ��������
    {
        if(st[i] == '(')
            ++j;
        else if(st[i] == ')')
            --j;
        if( j < 0 )
        {
            delete[] st;
            return !true && !false;
        }
    }
    if(j)
    {
        delete[] st;
        return !true && !false;
    }
    //std::cout << "Brackets is right.\n";
    //qDebug() << "funk...";
    addCode(st);
    i = -1;
    while(st[++i])
    {
        if(isLetter(st[i]) || (i && st[i - 1] < -100 && st[i] < -100))
        {//���� ���� ������������ ������� - ������� �� ��������� �� ����������� ��� ��� ������� ������
            //std::cout << st << std::endl;
            delete[] st;
            return !true && !false;
        }
    }

    //qDebug() << "true";
    delete[] st;
    return true;
}


/*

 ����������� ���������

*/
CExpression::sintElem CExpression::doIt(char op, sintElem a)
{
/*
���������� ������� ��������
*/
    //std::cout << op << a.number << std::endl;
    sintElem ret;
    ret.lexeme = 0;
    switch (op)
    {
        case '%':
            ret.number = 0 - a.number;
            break;
        case -128:
            ret.number = sin(a.number);
            break;
        case -127:
            ret.number = cos(a.number);
            break;
        case -126:
            ret.number = tan(a.number);
            break;
        case -125:
            ret.number = 1 / tan(a.number);
            break;
        case -123:
            ret.number = log10(a.number);
            break;
        case -122:
            ret.number = log(a.number);
            break;
        case -120:
            ret.number = pow(a.number,0.5);
            break;
        case -119:
            ret.number = a.number * a.number;
            break;
        case -118:
            ret.number = *numArray>0 ? a.number : 0 - a.number;
            break;
        case -117:
            ret.number = asin(a.number);
            break;
        case -116:
            ret.number = acos(a.number);
            break;
        case -115:
            ret.number = atan(a.number);
            break;
        case -114:
            ret.number = atan(1 / a.number);
            break;
        case -113:
            ret.number = sinh(a.number);
            break;
        case -112:
            ret.number = cosh(a.number);
            break;
    }
    return ret;
}
CExpression::sintElem CExpression::doIt(char op, sintElem a, sintElem b)
{
/*
���������� �������� ��������
*/
    //std::cout << a.number << op << b.number << std::endl;
    sintElem ret;
    ret.lexeme = 0;
    switch (op)
    {
        case '+':
            ret.number = a.number + b.number;
            break;
        case '*':
            ret.number = a.number * b.number;
            break;
        case '-':
            ret.number = a.number - b.number;
            break;
        case '/':
            ret.number = a.number / b.number;
            break;
        case '^':case -121:
            ret.number = pow(a.number, b.number);
            break;
        case -124:
            ret.number = log(a.number)/log(b.number);
            break;
    }
    return ret;
}
unsigned CExpression::operatorCount(sintElem* op)
{

    unsigned k = 1;
    signed j = 0;
    while(k)//�������� �� ���������� ������ ������� ������� �� �����, ���� �� ������������ ������� �����������
    {//�� �������. . . ���� ��� ��������, �������� 1 ���� �� ��������, ���� �� �������� (�����) ������� 1.
        k += isOperation(op[-++j].lexeme) ?  !isUnary(op[-j].lexeme) : -1;//�� ����� ��� ������� ��� �� ������� ��������
    }
    return j;
}
/*std::ostream& operator << (std::ostream &cout, CExpression::sintElem el)
{
    if(el.lexeme)
        cout << el.lexeme;
    else
        cout << el.number;
    return cout;
}*/
void CExpression::delElements(sintElem* from, unsigned how)
{
/*
������� how ��������� �� ������� from, ��� ��������
*/
    for(unsigned i = 0; from[i + how - 1].lexeme != 127; ++i)
    {
        from[i] = from[i + how];
    }
}
CExpression& CExpression::optimization()
{
/*
�������� ������ ���������� ���������. �� ��������� �������������� �������.
*/
    if (!fine)
    {
        return *this;
    }
    unsigned counter = 0;
    while(walkthroughArray[counter].lexeme != 127)
        ++counter;
    sintElem* temp = new sintElem[counter + 1];

    unsigned i = 0, j = 0;
    while(walkthroughArray[i].lexeme != 127)
    {
        if(!walkthroughArray[i].lexeme || walkthroughArray[i].lexeme == 'x' || walkthroughArray[i].lexeme == 'y')
            temp[j++] = walkthroughArray[i];
        else
            if(isUnary(walkthroughArray[i].lexeme))
                if( !temp[j - 1].lexeme )
                    temp[j - 1] = doIt(walkthroughArray[i].lexeme, temp[j - 1]);
                else
                    temp[j++] = walkthroughArray[i];
            else
                if( !temp[j - 1].lexeme && !temp[j - 2].lexeme ) {
                    --j;
                    temp[j - 1] = doIt(walkthroughArray[i].lexeme, temp[j - 1], temp[j]);
                } else
                    temp[j++] = walkthroughArray[i];
        ++i;
    }
    temp[j].lexeme = 127;
    /*������������� ��������:
* 1, 0
+ 0
/ 1
- 0
^ 1

� ���� *, ��� � ��� ��������? ��� ��������� �������� ����������� ����� ��� ��� ���������, �. �. ����� ���� 1 x 3 ^ * ��� �� 1*�^3, ��� ������ ���� x^3,
��� ������ ��� � ������ �������� �����-�� ���� �� ���������� ���� �� ����� � ����� �������� x ��� �� ������ ���, ����� � ����� �� ������ ������ �������
������ ������������� �������, ��������� ��� ������� �� ��� �������, ������� ���������.
*/

    i = j = 0;
    if(temp[1].lexeme != 127)
        ++(++i);//���� � �������� ���������, ��� ������ ������� ��� ��� ������ �������� �����.
    while(temp[i].lexeme != 127)
    {
        switch(temp[i].lexeme)
        {
        case '*':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)//������ ������� �� ����� ��������� �����
            {
                delElements(temp + i - 1, 2);//������� 1 � *
                --(--i);
            }
            else if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                temp[i].lexeme = 0;
                temp[i].number = 0;//����� �� �������� ��� �������� ���������� 0 ������ *
                j = operatorCount(temp + i - 1);//�������� ������ �������, ������� ��� � *.
                delElements(temp + i - j - 1, j + 1);
                i -= j + 1;
            }
            else//����� ������ �������� �� ��� �������� � ������ �������
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 1)
                {//������� 1 � ���� ���������.
                    delElements(temp + i, 1);
                    delElements(temp + i - j - 1, 1);
                    --(--i);
                }
                else if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i - j, j + 1);//������� �� ����� ����
                    i -= j + 1;
                }
            }
            break;
        case '+': case '-':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i, 1);
                    delElements(temp + i - j - 1, 1);
                    --(--i);
                }
            }
            break;
        case '/':
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            if(!temp[i - 1].lexeme && temp[i - 1].number == 0)
            {
                temp[i].lexeme = 0;
                temp[i].number = 1;
                j = operatorCount(temp + i - 1);
                delElements(temp + i - j - 1, j + 1);
                i -= j + 1;
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && temp[i - j - 1].number == 0)
                {
                    delElements(temp + i - j, j + 1);
                    i -= j + 1;
                }
            }
            break;
        case '^': case -121:
            if(!temp[i - 1].lexeme && temp[i - 1].number == 1)
            {
                delElements(temp + i - 1, 2);
                --(--i);
            }
            else
            {
                j = operatorCount(temp + i);
                if(!temp[i - j - 1].lexeme && ( temp[i - j - 1].number == 0 || temp[i - j - 1].number == 1 ) )
                {
                    delElements(temp + i - j, j + 1);
                    i -= j + 1;
                }
            }
            break;
        }
        ++i;
    }

    delete[] walkthroughArray;
    walkthroughArray = temp;

    /*
    std::cout << "���������������� �������:\n";
    for(i = 0; walkthroughArray[i].lexeme != 127; ++i)
        if(walkthroughArray[i].lexeme)
            std::cout << walkthroughArray[i].lexeme << std::endl;
        else
            std::cout << walkthroughArray[i].number << std::endl;
    //*/

    return *this;
}



/*

�������� �� ���������� ������ ���������?

*/
bool CExpression::isConst()
{/*
 ��������������, ��� ������� ����������� ��� ����� ����� � ������� � ��� �� ����.
 � ����� ��������� ��� ������ ������ � ������� (�������� ������ ���������
 ����������� � � � � ���������). �� ����� ��� � �� �������.
  ���� ��� ���������� ��������� � ����� ��� ����������� � �� �������� ������ ���� ����� �� true
*/
    optimization();
    return fine && walkthroughArray[0].lexeme == 0 && walkthroughArray[1].lexeme == 127;
}

bool CExpression::isConstFast() const
{// ���� ������ ��������. . .
    for (int i = 0; walkthroughArray[i].lexeme != 127; ++i)
    {
        if (walkthroughArray[i].lexeme == 'x' || walkthroughArray[i].lexeme == 'y')
        {
            return false;
        }
    }
    return true;
}
