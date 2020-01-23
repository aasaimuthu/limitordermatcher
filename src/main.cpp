#include <iostream>
#include "InputOrderProcessor.h"

using namespace std;

int main(int argc, char* argv[])
{
    InputOrderProcessor inputprocessor;
    if (argc >= 2)
    {
        string strFileName = argv[1];
        cout << " InputFileName:" << strFileName << endl;
        ifstream ifs(strFileName.c_str(), ios::in);
        if (!ifs.is_open())
        {
            cout << "failed to open the file : " << strFileName << endl;
            return 0;
        }
        string line;
        while (getline(ifs, line))
        {
            if (line.empty() || '#' == line[0])
            continue;
            stringvector tokens;
            InputOrderProcessor::StringTokenize(line, " ", tokens);
            inputprocessor.ProcessOrderInput(tokens);
        }
    }
    else
    {
        while (1)
        {
            cout << " Please enter the order details with ===space demiliter=== in stdin" << endl;
            string userinput;
            getline(cin, userinput);
            cout << " UserInput Received:"  << userinput << endl;
            stringvector tokens;
            InputOrderProcessor::StringTokenize(userinput, " ", tokens);
            inputprocessor.ProcessOrderInput(tokens);
        }
    }
}
