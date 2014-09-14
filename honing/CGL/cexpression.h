#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>

class CExpression
{
    struct sintElem
    {
    /*
     �������� ��������� ������. ���� ������� �������, ���� �����. ����� ����������� ���������
     ����� ������ ������ � ������������������ � ����� ����� ������ ��������� ( � ��������,
     ���������� ��������).
    */
        char lexeme;
        double number;
    };

    bool fine;//�������� �� ������ ��������� ����������
    bool test;//����� �� ����������� ��������� �� ������������ ��� ������������
    bool expForm;//���� 1 �� ��������� ����� � ����������������� ����. �����
            //������������ �� � ���� ������������/����� ��� �����. �� ��������� 1

    sintElem* walkthroughArray;/*������ � ������� �������� ���������� ����������� ������.
                               ��������� ����� ������ ������� �������� walkthroughArray[i].lexeme == 127*/

    //��� mutable: http://alenacpp.blogspot.com/2005/10/mutable-constcast.html
    mutable double* numArray;//����� ������ ��� ����� calculate
    mutable sintElem* calculationIt;/*���� �������� ��� �������� � calculation � �������� �������*/
//��. ������ � ������� ������ �� ��������.


    //������� ������������. ��������� ���������� ������� � ���� �������.
    /*������� ���������� � ����� ArifmometrSortSigment.
    �������� ������� ��������� ��� ���������� � �������� �������*/
    void      sortPoland    (sintElem*              );
    void      recursionMain (sintElem*,short&,short&);
    void      recursionPlus (sintElem*,short&,short&);
    void      recursionMult (sintElem*,short&,short&);
    void      recursionPow  (sintElem*,short&,short&);
    void      recursionUnary(sintElem*,short&,short&);
    /*������� ���������� � ����� ArifmometrPreparationSigment.
    �������� ������� ����������� ��� ���������� ������ � ���������� � �������� �������*/
    void      sintAdaptation   (char*                  );
    double    setPoint         (double, short          );
    static char* addMult       (char*                  );
    sintElem* setArray         (char*                  );
    static void addCode        (char*                  );
    void toExpForm             (char*                  );

//������� ��� �������� ������ ��� ��������� � ��� ������� �����������.
    static inline bool isNumber(char);//0-9/./x/y
    static inline bool isFigure(char);//0-9/.
    static inline bool isLetter(char);//�������� ��������� �����-��?
    static inline bool isUnary(char);//������� ��������. 1 ���� �������, 0 ���� ��������.
    static inline bool isOperation(char);//���� �� 0 � �� 'x' � �� 'y'
    sintElem doIt(char,sintElem);//��������� �������� char ��� ���������
    sintElem doIt(char,sintElem,sintElem);//���
    unsigned operatorCount(sintElem*);//
    void delElements(sintElem*, unsigned);

public:
/*
��� ������� ������� ������ ���������� ���������� ������ �� ������
��� �������� ��� �������.
*/
    CExpression();//��������� ��������� ��������, �������� ������ � true
    inline CExpression(const CExpression& exp);//�������� operator =
    inline CExpression(char const* exp, bool isTest = true);//�������� rebild
    inline CExpression(const std::string& exp, bool isTtest = true);//�������� rebild
    ~CExpression(void);
    CExpression& operator = (const CExpression& exp);
    inline CExpression& rebild(const std::string& st);//������ � ������������
    CExpression& rebild(const char* exp);//������ �������� ������� ���������, ���������� ������ �� ������ exp.
    double calculate(double x = 0., double y = 0.) const;
    //����� ��������� ������. ������ ��� ����������� ������������!
    static double calculate(const char* st, double x = 0., double y = 0.);
    static double calculate(const std::string& st, double x = 0., double y = 0.);

    static inline bool isExp(const std::string&);
    static bool isExp(const char*);


    CExpression& setTest(bool t) { test = t; return *this; }
    CExpression& setExpForm(bool t) { expForm = t; return *this; }
    bool isExpFor() const { return expForm; }
    bool isTest() const { return test; }
    bool isFine() const { return fine; }
    operator bool () const { return fine; }

    CExpression& optimization ();
    bool isConst();
    bool isConstFast() const;
};


#endif // CEXPRESSION_H
