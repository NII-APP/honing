#ifndef CPARSE_H
#define CPARSE_H

#include <fstream>
#include <string>


class CParse
{
protected:
    char* d;
    mutable bool owner;
    CParse(char *string, bool isOwner);
    CParse(const CParse&);
public:
    CParse(char *string = 0);
    CParse(const char *string);
    ~CParse();
    static CParse parseFile(const std::string& filename);
    inline char *data() { return d; }



    int integer();
    double real();
    std::string string();
    std::string word();
    time_t time();
    float fixFloat8();

    void skipRow();
    void skipTo(const char* dest);
    inline void skipTo(const std::string& dest) { skipTo(dest.c_str()); }
    bool testPrew(const char* dest);
    inline bool testPrew(const std::string& dest) { return testPrew(dest.c_str()); }
};

#endif // CPARSE_H
