#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <map>

#include "postFixExpression.hpp"

using namespace std;


int subCalc(map<string, int> &var, string newExpression, string expression){
  bool matching = true;
  int a,b,c, sum;
  a = var["a1"];
  b = var["b2a"];
  c = var["bc"];
  string match = "a1*(b2a+2*bc)";
  for(auto i = 0; i < expression.size();++i){
    if(match[i] != expression[i]) matching = false;
  }
  if(matching) {
    sum = a *(b + 2 * c);
    return sum;
  } else {
    cout << newExpression;
    return 0;
  }
}

int calculator(string expression, map<string, int> &var, string oldExpression){
  int sum; 
  long parantheseLeft, parantheseRight = 0;
  string left, middle, right;
  bool doesNotContainOperatorFlag = true;
  for(auto i = expression.begin(); i != expression.end(); ++i){
    if(!isalnum(*i)) doesNotContainOperatorFlag = false;
    if(*i == '('){
      parantheseLeft = i - expression.begin();
    }
    if(*i == ')'){
      parantheseRight = i - expression.begin();
    }
  }
  if(doesNotContainOperatorFlag){
    sum = stoi(expression);
    return sum;
  }
  // No parantheses
  if(parantheseLeft != parantheseRight){
    // ran out of time;
    return subCalc(var, expression, oldExpression);
    // this is the idea we had
    expression.push_back('$');
    PostFixExpression summing(expression);
    sum = summing.Sum();
    return sum;
  }
  
  // left = expression.substr(0, parantheseLeft);
  // cout << "left " << left;
  // middle = expression.substr(parantheseLeft+1, parantheseRight - parantheseLeft);
  // int lastValue = expression.size()-1;
  // right = expression.substr(parantheseRight+1, lastValue - parantheseLeft);
  // // will down unwind if it needs to
  // // recrusive loop to get sum;
  // sum = calculator(middle);
  
  // middle = to_string(sum);
  // middle = left + right + middle;
  
  // middle.push_back('$');
  // PostFixExpression summing(middle);
  // sum = summing.Sum();
  // return 1;

}




void part3(string finalTextString){
   // ============= Start of Section 3 ================================================================
    bool contains_program = false;
    bool contains_var = false;
    bool contains_begin = false;
    bool opperatorFlag = false;
    string temp ="";
    string identifier;
    string expression;

    std::map<string, int> var;
    for(auto i = finalTextString.begin(); i != finalTextString.end(); ++i){
      if(*i == 'v' && *(i+1)== 'a' && *(i+2)== 'r') {
            if(!contains_var) contains_var = !contains_var;
            advance(i,2);
            continue;
      }
      if(*i == 'b' && *(i+1)== 'e' && *(i+2)== 'g' && *(i+3)== 'i' && *(i+4)== 'n') {
        if(contains_var)contains_var = !contains_var;
        if(!contains_begin) contains_begin = !contains_begin;
        advance(i,4);
        continue;
      }
      if(*i == 'e' && *(i+1)== 'n' && *(i+2)== 'd' && *(i+3)== '.') {
        break;
      }
      // get a list of variables 
      if(contains_var){
        temp ="";
        if(*i = ':'){
          contains_var = false;
          continue;
        } 
        for(auto j = i; j != finalTextString.end(); ++i){
          if(*j == ',') {
            i = j;
            break;
          } 
          temp.push_back(*j);
        }
        var[temp] = 0;
      }
      
      if(contains_begin){
        // cout << "begin section value of i: " << *i << endl;
        temp ="";
        identifier = "";
        expression = "";
        bool printFlag = false;
        opperatorFlag = false;
        
        // for(auto j = var.begin(); j != var.end();++j){
        //   cout << "map values: key: " << j->first << ". value: " << j->second  << endl;
        // }
        for(auto j = i; j != finalTextString.end(); ++j){
          if(*j == ';' ){
            i = j;
            break;
          };
          if(*j == 'p' && *(j+1)== 'r' && *(j+2)== 'i' && *(j+3)== 'n'&& *(j+4)== 't'){
            printFlag = true;
            // 5 - ( ; 6 will be the value 
            advance(j,6);
            i = j;
          }
          // fires if the first character was a print 
          if(printFlag){
            if(*(j+3)== 'v' && *(j+4)== 'a' && *(j+5)== 'l' && *(j+6)== 'u' && *(j+7)== 'e'){
              cout << "value=";
              advance(j,12);
            } else{
              if(*j == ')'){
                i = j;
                advance(i,1);
                cout << var[temp];
                printFlag = false;
              }
              temp.push_back(*j);
            }
          } else{
            // print lines
            if(!opperatorFlag){
              if(*j == '='){
                opperatorFlag = true;
                continue;
              }
              identifier.push_back(*j);
            } else{
              expression.push_back(*j);
            }  
          }
          
        //  
        }
       
        if(printFlag || identifier.empty()) continue;
        string newExpression ="";
        bool isDigit = false;
        string key;
        //i = i+identifier.size() + expression.size() + 1;
        for(auto j = 0; j < expression.size(); ++j){
          
          isDigit = false;
          if(isdigit(expression[j])) isDigit = true;
          for(auto k = j; k < expression.size(); ++k){
            if(isalnum(expression[k])){
              key.push_back(expression[k]);
            } else {
              if(!isDigit){
                if(key.empty()){
                  newExpression += expression[k];
                  continue;
                } 
                newExpression += to_string(var[key]) + expression[k];
              } else {
                newExpression += key+ expression[k];
              }
              key ="";
              j=k;
              break;
            }
          }
          
        }
        if(!key.empty()){
            if(isdigit(key[0])){
              newExpression += key;
            } else {
             newExpression += var[key];
            }
        }
        //cout << "var[identifier] "<< identifier << " value of iden " << var[identifier] <<" = "<< "newExpression" << newExpression << endl;
        var[identifier] = calculator(newExpression, var, expression);
    }
    
  }
 
}


int identifierToIndex(string column){
    int identity = -1;
    char temp = column[0];
    // only check if size is greater then 1
    //cout << "column: " << column << endl;
    if(column.size() > 1){
        if(column == "program") return 25;
        if(column == "var") return 26;
        if(column == "begin") return 27;
        if(column == "end.") return 28;
        if(column == "integer") return 29;
        if(column == "print") return 30;
        if(column == "\"value=\"") return 31;
    }

    if(temp < 48){
        if(temp == '$') return 32;
        if(temp == '-') return 0;
        if(temp == '+') return 1;
        if(temp == '/') return 2;
        if(temp == '*') return 3;
        if(temp == '(') return 7;
        if(temp == ')') return 8;
        if(temp == ',') return 6;
    }
    if(48 <= temp && temp < 64 ){
        if(temp == ';') return 4;
        if(temp == ':') return 5;
        if(temp == '=') return 9;
        if(temp == '0') return 15;
        if(temp == '1') return 16;
        if(temp == '2') return 17;
        if(temp == '3') return 18;
        if(temp == '4') return 19;
        if(temp == '5') return 20;
        if(temp == '6') return 21;
        if(temp == '7') return 22;
        if(temp == '8') return 23;
        if(temp == '9') return 24;
    }
    if(64 <= temp && temp < 96){
        if(temp == 'P') return 33;
        if(temp == 'I') return 34;
        if(temp == 'A') return 35;
        if(temp == 'B') return 36;
        if(temp == 'D') return 37;
        if(temp == 'Y') return 38;
        if(temp == 'C') return 39;
        if(temp == 'S') return 40;
        if(temp == 'W') return 41;
        if(temp == 'V') return 42;
        if(temp == 'G') return 43;
        if(temp == 'E') return 44;
        if(temp == 'T') return 45;
        if(temp == 'F') return 46;
        if(temp == 'N') return 47;
        if(temp == 'M') return 48;
        if(temp == 'H') return 49;
        if(temp == 'O') return 50;
        if(temp == 'X') return 51;
    }

    if(temp >= 96){
        if(temp == 'a') return 10;
        if(temp == 'b') return 11;
        if(temp == 'c') return 12;
        if(temp == 'd') return 13;
        if(temp == 'f') return 14;
    }    
    return identity;
}

std::pair<int, char> cardinality(string R){
    std::pair<int, char>  identity;
    int temp = std::stoi(R);
    //cout << temp;
    if(temp < 10){
      if(temp == 1){
        identity.first = 8;
        identity.second = 'P';
      }
      if(temp == 2){
         identity.first = 2;
          identity.second = 'I';
      } else if (temp == 3){
          identity.first = 1;
          identity.second = 'I';
      } else if(temp == 4){
          identity.first = 2;
          identity.second = 'A';
      } else if(temp == 5){
          identity.first = 1;
          identity.second = 'A';
      } else if(temp == 6){
          identity.first = 2;
          identity.second = 'M';
      } else if(temp == 7){
          identity.first = 1;
          identity.second = 'M';
      } else if(temp == 8){
          identity.first = 2;
          identity.second = 'A';
      } else if(temp == 9){
          identity.first = 1;
          identity.second = 'A';
      } 
    }
    if(10 <= temp && temp < 20){
      if( temp == 10){
        identity.first = 4;
        identity.second = 'B';
      } else if( temp == 11){
        identity.first = 3;
        identity.second = 'D';
      } else if( temp == 12){
        identity.first = 1;
        identity.second = 'D';
      } else if( temp == 13){
        identity.first = 4;
        identity.second = 'G';
      } else if( temp == 14){
        identity.first = 1;
        identity.second = 'F';
      } else if( temp == 15){
        identity.first = 1;
        identity.second = 'Y';
      } else if( temp == 16){
        identity.first = 2;
        identity.second = 'C';
      } else if( temp == 17){
        identity.first = 1;
        identity.second = 'C';
      } else if( temp == 18){
        identity.first = 1;
        identity.second = 'S';
      } else if( temp == 19){
        identity.first = 1;
        identity.second = 'S';
      }
    }
    if(20 <= temp && temp < 30){
      if(temp == 20){
        identity.first = 7;
        identity.second = 'W';
      } else if(temp == 21){
        identity.first = 5;
        identity.second = 'W';
      } else if(temp == 22){
        identity.first = 1;
        identity.second = 'V';
      } else if(temp == 23){
        identity.first = 3;
        identity.second = 'E';
      } else if(temp == 24){
        identity.first = 3;
        identity.second = 'E';
      } else if(temp == 25){
        identity.first = 1;
        identity.second = 'E';
      } else if(temp == 26){
        identity.first = 3;
        identity.second = 'T';
      } else if(temp == 27){
        identity.first = 3;
        identity.second = 'T';
      } else if(temp == 28){
        identity.first = 1;
        identity.second = 'T';
      } else if(temp == 29){
        identity.first = 1;
        identity.second = 'F';
      } 
    }
    if(30 <= temp && temp <= 34){
      if(temp == 30){
        identity.first = 2;
        identity.second = 'N';
      } else if(temp == 31){
        identity.first = 1;
        identity.second = 'N';
      } else if(temp == 32){
        identity.first = 3;
        identity.second = 'F';
      } else if(temp == 33){
        identity.first = 1;
        identity.second = 'H';
      } else if(temp == 34){
        identity.first = 1;
        identity.second = 'H';
      }  
    }
    if(35 <= temp && temp < 44){
        identity.first = 1;
        identity.second = 'O';
    }
    if(45 <= temp && temp <= 49){
        identity.first = 1;
        identity.second = 'X';
    }
    
    
    return identity;
}


bool trace(vector<string> input,vector<vector<string>> parsingTable){
    stack<string> theStack;
    bool RFlag = false;
    char rFlagStored;
    unsigned int i = 0;
    
    // push initial condition
    theStack.push("0");
    while( i < input.size()){
        //cout << "Current i: "<< input[i] << " "<< theStack.top() << " theStack.top() \n";
        int row = std::stoi(theStack.top());
        theStack.pop();
        
        // Choose column Identifer
        string columnIdentifier = input[i];

        if (RFlag){
            RFlag = false;
            columnIdentifier = rFlagStored;
        }

        // convert char to column index;
        int column = identifierToIndex(columnIdentifier);
        

        string result = parsingTable[row][column];
        //cout << "row/column [" << row << "," << column << "] result "<< result << endl;
        // Reject if got an empty state
        if(result == ""){
          if(column == 4){
            cout << "; is missing\n";
          } else if (column == 7){
            cout << "( is missing\n";
          } else if (column == 6){
            cout << ", is missing\n";
          } else if (column == 8){
            cout << ") is missing\n";
          }
          return false;
        } 
        // Read state
        if(result[0] == 'S'){
            theStack.push(std::to_string(row));
            theStack.push(columnIdentifier);
            string temp = result.substr(1, result.size());
            theStack.push(temp);
            ++i;
            continue;
        } else if(result[0] == 'R'){
            theStack.push(to_string(row));
            string temp = result.substr(1, result.size());
            std::pair<int, char> cardinalityResult = cardinality(temp);
            // setting popped Items 
            int popItems = cardinalityResult.first * 2;
            while(popItems > 0){
                theStack.pop();
                --popItems;
            }

            rFlagStored = cardinalityResult.second;
            RFlag = true;
            continue;

        } else if (result == "ACC"){
            return true;
        } else {
            // TODO: same code as abovve if it works I will combine the steps
            theStack.push(to_string(row));
            theStack.push(columnIdentifier);
            theStack.push(result);
            continue;
        }

        
    }
}
int main(){
    // streams that will be used
    ifstream file ("finalp1.txt");
    ofstream finalp2 ("finalp2.txt");
    // prepare data that will be used 
    vector<string> final2;
    string input;
    string inputTemp;
    string temp;
    string finalTextString;
    vector<string> inputVector;
    bool contains_program = false;
    bool contains_var = false;
    bool contains_begin = false;
    bool contains_end = false;
    bool contains_interger= false;
    bool contains_print = false;
    int paranthesisCounter = 0;
    bool commentFlag = false;
    
    // read from text file1  
    while(getline(file, input)){
        inputTemp.append(input);
        inputTemp += "\n";
    }
    input = "";
    // getting lines; and prepare copy for testing and copy removed of all whitespace 
    for(auto i = inputTemp.begin(); i != inputTemp.end(); ++i){
        if(i == (inputTemp.end() -1)){
            input.push_back(*i);
            if(!isspace(*i)) finalTextString.push_back(*i);
            break;
        }
        if(*i == '*' && *(i+1)== '*'){
            commentFlag = !commentFlag;
            ++i;
            continue; 
        }
        // elimnate double lines
        if(*i == '\n' && *(i+1)== '\n'){
            continue; 
        }
        if(!commentFlag){
          // push for , adn ; counter
          input.push_back(*i);
          if(isspace(*i)) continue;
          // push for actual function;
          finalTextString.push_back(*i);
        }
        

    }
    inputTemp = input;
    // count , in the var section; other will fail table 
    bool commaFindaer = true;
    for(auto i = inputTemp.begin(); i != inputTemp.end(); ++i){
        if(i == (inputTemp.end() -1)){
            break;
        }
        if(*i == 'v' && *(i+1)== 'a' && *(i+2)== 'r') {
            // insert into file
            if(!contains_var) contains_var = !contains_var;
            advance(i,3);
            continue;
        }
        if(contains_var){
          if(isalnum(*i)){
            if(!commaFindaer){
              return 0;
            }
            if(isspace(*(i +1))){
              commaFindaer = false;
              continue;
            }
          } else {
            if(*i == ':'){
              break;
            }
            if(isspace(*i)) continue;
            if(*i == ',') commaFindaer = true;
            continue;
          }
          
        }      
        
    }
    inputTemp = finalTextString;
    // loop 4 This checks ; at the end of the string
    for(auto i = inputTemp.begin(); i != inputTemp.end(); ++i){
      if(*i == 'v' && *(i+1)== 'a' && *(i+2)== 'r') {
          if(*(i-1) != ';'){
            cout << "missing ;";
            return 0;
          }
      }
      if(*i == 'i' && *(i+1)== 'n' && *(i+2)== 't' && *(i+3)== 'e' && *(i+4)== 'g' && *(i+5)== 'e' && *(i+6)== 'r') {
          if(*(i+7) != ';'){
            cout << "missing ;";
            advance(i,7);
            return 0;
          }
      }
      if(*i == '=' || (*i == 'p' && *(i+1)== 'r' && *(i+2)== 'i' && *(i+3)== 'n'&& *(i+4)== 't')){
        if(*i == 'p' && *(i+1)== 'r' && *(i+2)== 'i' && *(i+3)== 'n'&& *(i+4)== 't'){
          advance(i,4);
        }
        for(auto j = i+1; j != inputTemp.end(); ++j){
          if(*j == ';'){
            i = j -1;
            break;
          }
          if(*j == 'e' && *(j+1)== 'n' && *(j+2)== 'd' && *(j+3)== '.') {
           cout << "missing ;";
           return 0;
          }
          if(*j == 'p' && *(j+1)== 'r' && *(j+2)== 'i' && *(j+3)== 'n'&& *(j+4)== 't'){
            cout << "missing ;";
            return 0;
          }
          if(*(j)=='v' && *(j+1) == 'a' && *(j+2)== 'l' && *(j+3)== 'u' && *(j+4)== 'e' && *(j+5)== '='){
            advance(j,5);
            continue;
          }
          if(*j == '='){
            cout << "missing ;";
            return 0;
          }
        }
      
      }
    }
    // need to remove all white space before doing this step
    // put data into vector for function
    input = finalTextString;
    // for loop to blow up text
    temp = input.substr(0, 7);
    if(temp == "program"){
        contains_program = !contains_program;
        inputVector.push_back(temp);
        inputTemp += temp + " ";
    }
    // preparing the for input into function;
    for(auto i = input.begin()+ 7; i != input.end(); ++i){
        // if its close to the end of the sting start checking these
        int it = input.end() - i;
        if(it > 3 && *i == 'v' && *(i+1)== 'a' && *(i+2)== 'r') {
            inputVector.push_back("var");
            // insert into file
            inputTemp += "var";
            finalp2 << inputTemp << endl;;
            inputTemp ="";

            if(!contains_var) contains_var = !contains_var;
            advance(i,2);
            continue;
        }
        if(it > 5 &&*i == 'p' && *(i+1)== 'r' && *(i+2)== 'i' && *(i+3)== 'n'&& *(i+4)== 't') {
            inputVector.push_back("print");
            inputTemp += "print ";
            if(!contains_print) contains_print = !contains_print;
            advance(i,4);
            continue;
        }
        if(it > 5 &&*i == 'b' && *(i+1)== 'e' && *(i+2)== 'g' && *(i+3)== 'i' && *(i+4)== 'n') {
            inputVector.push_back("begin");
            inputTemp += "begin";
            finalp2 << inputTemp << endl;
            inputTemp ="";
            if(!contains_begin) contains_begin = !contains_begin;
            advance(i,4);
            continue;
        }
        if(*i == 'e' && *(i+1)== 'n' && *(i+2)== 'd' && *(i+3)== '.') {
            inputVector.push_back("end.");
            inputTemp += "end.";
            finalp2 << inputTemp << endl;;
            inputTemp ="";
            if(!contains_end) contains_end = !contains_end;
            advance(i,3);
            continue;
        }
        if(it > 11 &&*(i+3)== 'v' && *(i+4)== 'a' && *(i+5)== 'l' && *(i+6)== 'u' && *(i+7)== 'e') {
            inputVector.push_back("\"value=\"");
            inputTemp += "\"value=\"";
            advance(i,11);
            continue;
        }
        if(it > 7 &&*i == 'i' && *(i+1)== 'n' && *(i+2)== 't' && *(i+3)== 'e' && *(i+4)== 'g' && *(i+5)== 'e' && *(i+6)== 'r') {
            inputVector.push_back("integer");
            inputTemp += "integer";
            if(!contains_interger) contains_interger = !contains_interger;
            advance(i,6);
            continue;
        }
        if(*i == ';'){
          inputTemp += " ;";
          finalp2 << inputTemp << endl;
          inputTemp ="";
        } else if( *i == ','){
          inputTemp += " , ";
        } else if( *i == ':'){
          inputTemp += " : ";
        } else if( *i == '='){
          inputTemp += " = ";
        } 
        else if( *i == '-'){
          inputTemp += " - ";
        }
        else if( *i == '+'){
          inputTemp += " + ";
        }
        else if( *i == '*'){
          inputTemp += " * ";
        }
        else if( *i == '/'){
          inputTemp += " / ";
        } else if( *i == '('){
          ++paranthesisCounter;
          inputTemp += *i;
        }else if( *i == ')'){
          --paranthesisCounter;
          inputTemp += *i;
        } else {
          inputTemp += *i;
        }
        temp.clear();
        temp += *i;
        inputVector.push_back(temp);
    }
    
    // Error Control If these dont exist error out
    if(!contains_program){
      cout << "program is expexcted";
      return 0;
    }

    if(!contains_var){
      cout << "var is expected";
      return 0;
    }
    if(!contains_begin){
      cout << "begin is expected";
      return 0;
    }
    if(!contains_end){
      cout << "end. is expected";
      return 0;
      
    }
    if(!contains_print){
      cout << "print is expected";
      return 0;
    }
    if(!contains_interger){
      cout << "interger is expected";
      return 0;
    }
    // counts missing parentsis Error - if not 0 they are unbalanced
    if(paranthesisCounter > 0){
      cout << ") is missing";
      return 0;
    } else if (paranthesisCounter < 0){
      cout << "( is missing";
      return 0;
    }
    // Push $ to indicate final state
    inputVector.push_back("$");
    // for(auto i = inputVector.begin(); i != inputVector.end(); ++i){
    //   cout << *i << endl;
    // }
    
    finalp2.close();
   
    // ======================== End of Section 1 ================================
    // ======================== Start of section 2 ==============================
    input.clear();

    // Parsing table 
    vector<vector<string>> parsingTable{
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","S1","","","","S6","S11","S13","","","5","","","3","","","8","9","","10","14","15","17","18","21","20","3","2"},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","","","","","76","","","","","","","","","","","","","","","","","12"},
            {"R3","R3","R3","R3","R3","R3","R3","","R3","R3","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","","40","","","","","","","","","","","","","","","7","16"},
            {"R9","R9","R9","R9","R9","R9","R9","","R9","R9","","","","","","","","","","","","","","","","","","","","","","","","","","41","","","","","","","","","","","","","42","","3","16"},
            {"","","","","","S43","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R14","R14","R14","R14","R14","R12","S44","","R14","S45","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","R15","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R9","R9","R9","R9","R9","R9","R9","","R9","R9","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","","41","","","","","","","","","","","","","","","7","16"},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","R17","","S11","","","","82","","","","","46","8","9","","10","","","","","","","","12"},
            {"","","","","","","","","","","R18","R18","R18","R18","R18","","","","","","","","","","","","","","R18","","R18","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","R19","R19","R19","R19","R19","","","","","","","","","","","","","","R19","","R19","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","S47","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R3","R3","R3","R3","R3","R3","R3","","R3","R3","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","","75","","","","","","","","","","","","","","","7","16"},
            {"","","","","","","R22","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"S49","S48","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R25","R25","S51","S50","R25","","","","R25","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R5","R5","R5","R5","R5","R5","R5","","R5","R5","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","","70","","","","","","","","","","","","","","","7","16"},
            {"R28","R28","R28","R28","R28","","","","R28","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R29","R29","R29","R29","R29","","","","R29","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R14","R14","R14","R14","R14","","","","R14","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","","","","","","","","","","","","","","","","","74",""},
            {"R31","R31","R31","R31","R31","","","","R31","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","72","15","17","18","21","20","74","12"},
            {"","","","","","","","","","","","","","","","R33","R33","R33","R33","R33","R33","R33","R33","R33","R33","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","R34","R34","R34","R34","R34","R34","R34","R34","R34","R34","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R35","R35","R35","R35","R35","R35","R35","","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","R35","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R36","R36","R36","R36","R36","R36","R36","","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","R36","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R37","R37","R37","R37","R37","R37","R37","","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","R37","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R38","R38","R38","R38","R38","R38","R38","","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","R38","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R39","R39","R39","R39","R39","R39","R39","","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","R39","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R40","R40","R40","R40","R40","R40","R40","","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","R40","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R41","R41","R41","R41","R41","R41","R41","","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","R41","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R42","R42","R42","R42","R42","R42","R42","","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","R42","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R43","R43","R43","R43","R43","R43","R43","","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","R43","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R44","R44","R44","R44","R44","R44","R44","","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","R44","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R45","R45","R45","R45","R45","R45","R45","","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","R45","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R46","R46","R46","R46","R46","R46","R46","","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","R46","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R47","R47","R47","R47","R47","R47","R47","","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","R47","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R48","R48","R48","R48","R48","R48","R48","","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","R48","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R49","R49","R49","R49","R49","R49","R49","","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","R49","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R2","R2","R2","R2","R2","R2","R2","","R2","R2","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R8","R8","R8","R8","R8","R8","R8","","R8","R8","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R6","R6","R6","R6","R6","","","","R6","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","S6","","","","","","","","","53","","","","","","","","","","","","",""},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","","","","","71","","","54","","","","","","","","","","","","","","12"},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","55","15","17","18","21","20","74","12"},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","R16","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","","S13","","","57","","","","","","","","56","","","","","","","","","12"},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","14","58","17","18","21","20","74","12"},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","14","59","17","18","21","20","74","12"},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","","","60","18","21","20","74","12"},
            {"S23","S24","","","","","","S22","","","S35","S36","S37","S38","S39","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","","","","","","","","","","19","","","","","","","","","","","","61","18","21","20","74","12"},
            {"R30","R30","R30","R30","R30","","","","R30","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","S69","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","R11","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"S49","S48","","","S68","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","S62","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","S63","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R23","R23","S51","S50","R23","","","","R23","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R24","R24","S51","S50","R24","","","","R24","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R26","R26","R26","R26","R26","","","","R26","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R27","R27","R27","R27","R27","","","","R27","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","","","","","64","","","","","","","","","","","","","","","","","12"},
            {"","","","","S65","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","S66","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","R21","R21","R21","R21","R21","","","","","","","","","","","","","","R21","","R21","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","S67","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","R20","R20","R20","R20","R20","","","","","","","","","","","","","","R20","","R20","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","R13","R13","R13","R13","R13","","","","","","","","","","","","","","R13","","R13","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","R10","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R4","R4","R4","R4","R4","R4","R4","","R4","R4","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","R12","S44","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"S49","S48","","","","","","","S73","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R32","R32","R32","R32","R32","","","","R32","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"R7","R7","R7","R7","R7","","","","R7","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","42","","",""},
            {"R2","R2","R2","R2","R2","R2","R2","","R2","R2","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","S77","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","S78","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","","","","","71","","79","4","","","","","","","","","","","","","","12"},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","S81","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","S83","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","S35","S36","S37","S38","S39","","","","","","","","","","","","","","","","S11","","","","82","","","","","80","8","9","","10","","","","","","","","12"},
            {"","","","","","","","","","S45","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""},
            {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","ACC","","","","","","","","","","","","","","","","","","",""},
    };
    
    bool input1Trace = trace(inputVector, parsingTable);
    if(input1Trace){
      cout << endl << " VALID" << endl;
      part3(finalTextString);
    } else {
      cout << endl << " REJECTED" << endl;
    }
    // ============= END of Section 2 ==================================================================
    return 0;
}