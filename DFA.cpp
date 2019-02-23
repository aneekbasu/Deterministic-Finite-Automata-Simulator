#include "stdafx.h"

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
using namespace rapidjson;
bool bVerbose = false;
bool bOutputToFile = false;
string myOutputToFile = "";

void WriteLog(string content)
{
    if (bVerbose)
        cout << content << endl;
    if (bOutputToFile)
    {
        ofstream outFile(myOutputToFile.c_str(),std::ofstream::out | std::ofstream::app);
        if (outFile)
        {
            outFile << content;
            outFile.close();

        }
    }
}
string ReadContentsByLines(string fileName)
{
    std::string contents("");
    std::ifstream inFile(fileName.c_str(), std::ios::in | std::ios::binary);
    if (inFile)
    {
        inFile.seekg(0, std::ios::end);
        contents.resize(inFile.tellg());
        inFile.seekg(0, std::ios::beg);
        inFile.read(&contents[0], contents.size());
        inFile.close();
    }
    WriteLog(contents);
    return contents;
}
string GetNewState(string curr_State, string next_Symbol, Value &myTransition)
{
    for (SizeType i = 0; i < myTransition.Size(); i++)
    {
        string valNext_symbol = (myTransition[i])["next_symbol"].GetString();
        string valCurr_state = (myTransition[i])["current_state"].GetString();
        if((valNext_symbol.compare(next_Symbol) == 0) && (valCurr_state.compare(curr_State) == 0))
        {
            string valNew_state = (myTransition[i])["new_state"].GetString();
            //if(bVerbose)
              //  cout<<"Current State: "<< curr_State << " Symbol: " << next_Symbol << " --> New State: "<< valNew_state <<endl;
            string log;
            log.append("Current State: ").append(curr_State).append(" Symbol: ").append(next_Symbol).append(" --> New State: ").append(valNew_state).append("\n");
            WriteLog(log);
            return valNew_state;
        }
    }
    //cout<<"Values:"<<curr_State<<", "<<next_Symbol<<endl;
    //return "";
}
void CoreCalculation(string myInputs, set<char> nextSymbolSet, string startState, list<string> finalStates, Value &myTransition)
{
    /*if (!myInputs.empty() && myInputs[myInputs.size() - 1] == '\r')
    	myInputs.erase(myInputs.size() - 1);
    else
    {
    	cout << "Invalid Input!!" << endl;
    	return;
    }*/

    int count = myInputs.length();

    list<char> myArrayInputs(myInputs.begin(), myInputs.end());
    set<char> myArrayInputSet(myArrayInputs.begin(), myArrayInputs.end());

    set<char> myResultSet;
    set_difference(myArrayInputSet.begin(), myArrayInputSet.end(), nextSymbolSet.begin(), nextSymbolSet.end(), inserter(myResultSet, myResultSet.end()));
    string log;
    if (myResultSet.size() > 0){
        //cout << myInputs <<", ---> NOT ACCEPT, "<< myResultSet.size()<<" invalid char input:"<< endl;
        log.append(myInputs).append(", ---> NOT ACCEPT, ").append(to_string(myResultSet.size())).append(" invalid char input:\n");
        WriteLog(log);
    }
    else
    {
        string myCurrStartState = startState;

        for (auto const &i : myArrayInputs)
        {
            myCurrStartState = GetNewState(myCurrStartState, string(1,i), myTransition);
        }
        log = "";
        list<string>::iterator findIter = find(finalStates.begin(), finalStates.end(), myCurrStartState);
        if(findIter != finalStates.end())
        {
            log.append(myInputs).append(", ---> ACCEPT\n");
            WriteLog(log);
        }
        else
        {
            log.append(myInputs).append(", ---> NOT ACCEPT\n");
            WriteLog(log);
        }
    }
}

int main(int argc, char *argv[])
{
    int count;
    string myDFAInputFilePath = "";
    bool bInputStringFile = false;
    string myInputStringFile = "";
    string myStopSignal = "stop";
    //std::vector<std::string> args(argv, argv + argc);
    list<string> args(argv, argv + argc);

    auto myValue = std::find(args.begin(), args.end(), "-v");
    if (myValue != args.end())
    {
        bVerbose = true;
        cout << "Verbose mode ON !" << endl;
    }
    myValue = std::find(args.begin(), args.end(), "-h");
    if (myValue != args.end())
    {
        cout << "-h print usage" <<endl
             <<"­d \t <dfafile>  DFA definition file" <<endl
             <<"-v \t verbose mode, display machine definition, transitions etc."<<endl
             <<"-i \t Input filename with all symbols to feed DFA"<<endl
             <<"-o \t Output filename to store DFA program output"<<endl;
        return 0;
    }
    myValue = std::find(args.begin(), args.end(), "-d");
    if ((myValue != args.end()) && (std::next(myValue, 1) != args.end()))
    {
        myValue = std::next(myValue, 1);
        if (myValue != args.end())
            myDFAInputFilePath = *myValue;
        cout << "File name: " << myDFAInputFilePath.c_str() << endl;
    }
    else
    {
        cout << "No DFA file present !" << endl;
        return 0;
    }
    myValue = std::find(args.begin(), args.end(), "-i");
    if ((myValue != args.end()) && (std::next(myValue, 1) != args.end()))
    {
        myValue = std::next(myValue, 1);
        myInputStringFile = *myValue;
        bInputStringFile = true;
        cout << "Input string file name: " << myInputStringFile.c_str() << endl;
    }
    else
    {
        cout << "No Input string file present !" << endl;
    }
    myValue = std::find(args.begin(), args.end(), "-o");
    if ((myValue != args.end()) && (std::next(myValue, 1) != args.end()))
    {
        myValue = std::next(myValue, 1);
        myOutputToFile = *myValue;
        bOutputToFile = true;
        cout << "Output file name: " << myOutputToFile.c_str() << endl;
    }
    else
    {
        cout << "No output file supplied !" << endl;
    }

    string myContents = ReadContentsByLines(myDFAInputFilePath);
    if (myContents.empty())
    {
        cout << "File read failed !!" << endl;
        return 0;
    }
    Document d;
    d.Parse(myContents.c_str());
    Value &start_state = d["start_state"];
    string myStart_State = start_state.GetString();
    Value &final_states = d["final_states"];
    //Get all the final_states in list
    list<string> myFinalStates;
    for (SizeType i = 0; i < final_states.Size(); i++)
    {
        string val = final_states[i].GetString();
        myFinalStates.push_back(val);
    }
    //string myFinal_State = final_states.GetString();
    Value &transition = d["transition"];

    //Get all the next sysmbols in list, so we can quickly check if any input char is invalid
    //get all the unique char symbol & put all the unique symbols to single string
    list<string> myNextSymbols;
    string myTempString;
    for (SizeType i = 0; i < transition.Size(); i++)
    {
        string val = (transition[i])["next_symbol"].GetString();
        if (std::find(myNextSymbols.begin(), myNextSymbols.end(), val) == myNextSymbols.end())
        {
            myNextSymbols.push_back(val);
            myTempString.append(val.c_str());
        }
    }
    //get list of char from list of string
    list<char> myTempChars(myTempString.begin(), myTempString.end());
    //create set from list of char
    set<char> myNextSymbolSet(myTempChars.begin(), myTempChars.end());

    string myInput = "";
    if (bInputStringFile == false) //process string from the console inputs
    {
        while (true)
        {
            cout << "Enter DFA input string or type stop to exit" << endl;
            cin >> myInput;
            if (myInput.compare(myStopSignal) == 0)
                break;
            else
                CoreCalculation(myInput, myNextSymbolSet, myStart_State, myFinalStates, transition);
        }
    }
    else //process string from the input file
    {
        ifstream myStringFile(myInputStringFile);
        if (!myStringFile)
        {
            cout << "Error opening DFA input file:" << myInputStringFile << "Error No:" << errno << endl;
        }
        else
        {
            cout << "Starting processing DFA input string from file:" << myInputStringFile << endl;
            string line("");
            while (getline(myStringFile, line))
            {
                //cout<<"line="<<line<<"filename="<<myInputStringFile<<endl;
                CoreCalculation(line, myNextSymbolSet, myStart_State, myFinalStates, transition);
            }
            cout << "Done processing DFA input string from file:" << myInputStringFile << endl;
        }
    }

    return 0;
}
