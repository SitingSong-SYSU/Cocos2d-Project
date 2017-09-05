#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using std::string;
using std::vector;
class Global
{
public:
    Global();
    ~Global();
    static string gameSessionId;
	static string userName;
    static string score;
	static int heartNum;
    static string toString(vector<char> *buffer);
    static string getSessionIdFromHeader(string header);
};

#endif
