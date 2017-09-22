#include "polylex.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
using namespace std;


extern int currentLine = 0;

bool byValue(const pair<string,int> &a,const pair<string,int> &b)
{
       return a.second>b.second;
}

bool byKey(const string &a,const string &b)
{
       return a>b;
}

Token getToken(std::ifstream &infile)
{
	TokenTypes T;
  	bool fcon = false,  goodString = false, random = false;
   	
	char ch;
   	string lexeme = "";
   	string empty = "";
    
    while(ch = infile.get())
    {
        /*if(ch == '\n')
        {
            currentLine++;
            ch = infile.get();

        }
        */
		while(isspace(ch))
		{

            if(ch == '\n')
  			 {
       			currentLine++;
   			 }
            /*if (ch == ' ')
            {
             ch = infile.get();
            }
            */
                            ch = infile.get();

		}
		
		if(isalpha(ch))
	   	{
	   		lexeme += ch;
	     	while(isalnum(infile.peek()))
	       	{
	        	ch = infile.get();
				lexeme += ch;
	       	}
	            return Token(ID, lexeme);
            
			
	    }
		if (isdigit(ch))
		{
			lexeme += ch;
	        while(isdigit(infile.peek()) || ispunct(infile.peek()))
	        {
	        	infile.get(ch);
	            lexeme += ch;
				
	            if(ch == '.')
	            	fcon = true;
	        }
	        if(fcon == true)
	        	return Token(FCONST, lexeme);
	        else
	            return Token(ICONST, lexeme);
		}
        
        if(ch == '.')
        {
            lexeme += ch;
            while(isdigit(infile.peek()))
            {
                return Token(ERR, lexeme);
            }
            
        }
		
	   if(ch == '"')
	   {
		   lexeme += ch;
	       while(infile.get(ch))
	       {
			   if(ch == '"')
			   {
				   lexeme +=ch;
				   return Token(STRING, lexeme);
	           }
	           else if (ch == '\n')
               {
                   currentLine++;
				   return Token(ERR, lexeme);
               }
	           else
	               lexeme +=ch;  
	       }
	   }
	   
	   if (ch == '#')
	   {
           while(ch != '\n' || ch == EOF)
           {
           		ch = infile.get();
           }
           

           if(ch == EOF)
           {
               return Token(DONE, "");
           }

           currentLine++;           
           
           
       }
	   
	   if(ch == '+')
	   {
		   return Token(PLUS, "");
	   }
	   
	   if(ch == '-')
	   {
		   if(isspace(infile.peek()) || ispunct(infile.peek()))
	       {
			   return Token(MINUS, lexeme);
	       }

	       lexeme += ch;
	       while(isdigit(infile.peek()) || ispunct(infile.peek()))
		   {
			   infile.get(ch);
	           lexeme += ch;

	           if(ch == '.')
	               fcon = true;
	       }

	       if(fcon == true)
	           return Token(FCONST, lexeme);
	       else
	           return Token(ICONST, lexeme);
	   }
	   
	   if(ch == '*')
	   {
	       return Token(STAR, lexeme);
	   }
	   if(ch == ',')
	   {
	       return Token(COMMA, lexeme);
	   }
	   if(ch == '{')
	   {
	       return Token(LBR, lexeme);
	   }
	   if(ch == '}')
	   {
	       return Token(RBR, lexeme);
	   }
	   if(ch == '[')
	   {
	       return Token(LSQ, lexeme);
	   }
	   if(ch == ']')
	   {
	       	return Token(RSQ, lexeme);

	   }
	   if(ch == '(')
	   {
	       return Token(LPAREN, lexeme);
	   }
	   if(ch == ')')
	   {
	       return Token(RPAREN, lexeme);
	   }
	   if(ch == ';')
	   {
	       return Token(SC, lexeme);
	   }
	   
	   if(ch == EOF)
	   {
		   return Token(DONE, "");	
	   }	
    }
}

void printIDs(Token x)
{
   if(x.getType() == ID)
   {
       cout<< "ID (" << x.getLexeme() << ")"<< endl;
   }
   if(x.getType() == STRING)
   {

       cout<< "STRING (" << x.getLexeme() << ")"<< endl;
   }
   if(x.getType() == ICONST)
   {
       cout<< "ICONST (" << x.getLexeme() << ")"<< endl;
   }
   if(x.getType() == FCONST)
   {
       cout<< "FCONST (" << x.getLexeme() << ")"<< endl;
   }
   if(x.getType() == PLUS)
   {
       cout<< "PLUS" << endl;
   }
   if(x.getType() == MINUS)
   {
       cout<< "MINUS"<< endl;
   }
   if(x.getType() == STAR)
   {
       cout<< "STAR"<< endl;
   }
   if(x.getType() == COMMA)
   {
       cout<< "COMMA"<< endl;
   }
   if(x.getType() == LBR)
   {
       cout<< "LBR"<< endl;
   }
   if(x.getType() == RBR)
   {
       cout<< "RBR"<< endl;
   }
   if(x.getType() == LSQ)
   {
       cout<< "LSQ"<< endl;
   }
   if(x.getType() == RSQ)
   {
       cout<< "RSQ"<< endl;
   }
   if(x.getType() == LPAREN)
   {
       cout<< "LPAREN"<< endl;
   }
   if(x.getType() == RPAREN)
   {
       cout<< "RPAREN"<< endl;
   }
   if(x.getType() == SC)
   {
       cout<< "SC"<< endl;
   }
   if(x.getType() == ERR)
   {
       cout<< "Error on line "<< currentLine << " (" << x.getLexeme() << ")"<< endl;
   }
}

int main(int argc, const char * argv[])
{
   bool verbose =  false , stats = false, sum = false, fileExsists = false;
   ifstream infile;
   string name, s;
   int tokens = 0;
   int totalIDs = 0, counter = 0;
   
   typedef map<string, int> frequencyList;
   frequencyList mapCount;

   vector <string> idList;
   vector <string> count;

   vector <pair <string, float>> vecCount;
    
   vector <string>::iterator it;
   vector <string>::iterator itr;



   for (int i = 1; i < argc; i++)
   {
       name = argv[i];

       if (name[0] == '-')
       {
           if (name.find("-v") != string::npos )
           {
               verbose = true;
           }
           else if (name.find("-stats") != string::npos )
           {
               stats = true;
           }
           else if (name.find("-sum") != string::npos )
           {
               sum = true;
           }

           else
           {
               cout << "Invalid argument " << name << endl;
               return 0;
           }
       }

       else
       {

           if (i < argc-1)
           {
               cout << "Too many file names" << endl;
               return 0;
           }

           else
           {
               infile.open(argv[i]);

               if(infile.is_open() == false )
               {
                   cout << "Could not open " << argv[i] << endl;
                   return 0;
               }

               else
               {
                    if (infile.peek() == EOF)
               		{
                  		 continue;
               		}
                   
                    Token current;
                   	current = getToken(infile);

					while(!(current.getType() == DONE) )
                    {
                        tokens++;
                        
                        if(current.getType() == ID)
                        {
                            ++mapCount["ID"];
                        }
                        if(current.getType() == ICONST)
                        {
                            ++mapCount["ICONST"];
                        }
                        if(current.getType() == FCONST)
                        {
                            ++mapCount["FCONST"];
                        }
                        if(current.getType() == STRING)
                        {
                            ++mapCount["STRING"];
                        }

                 
                        if(current.getType() == ERR)
                       	{
                        	printIDs(current);
                            break;
                        }
                        
                        if(verbose == true)
                        {
                            printIDs(current);
                        }
                        
                        if (stats == true)
                        {
                            if(current.getType() == ID){
                                totalIDs++;
                                
                  			if ( std::find(idList.begin(), idList.end(), current.getLexeme()) == 																									idList.end() )
                                    idList.push_back(current.getLexeme());                                
                            }                       
                        }
                        
                        current = getToken(infile);
                   }

               }
           }
       }
   }
    
   
    copy(mapCount.begin(), mapCount.end(), back_inserter(vecCount));
    sort(vecCount.begin(),vecCount.end(),byValue);
    
    float avg;
    
    avg = vecCount[0].second/tokens;



   if (sum == true and stats == false)
   {
      
       if(tokens == 0){
           cout << "Total Lines: " << currentLine << endl;
           cout << "Total tokens: " << tokens << endl;
       }
       else
       {
           cout << "Total Lines: " << currentLine << endl;
           cout << "Total tokens: " << tokens << endl;
           cout << "Most frequently used tokens: ";
           
           for(int i = 0; i < vecCount.size(); ++i)
           {
               if((vecCount[i].second/tokens) >= avg)
               {
                   count.push_back(vecCount[i].first);
               }
           }
           
               sort(count.begin(), count.end(), byKey);

           
           for (itr = count.begin(); itr != count.end(); itr++)
           {
               if(itr == count.begin())
                   cout << *itr;
               if(itr != count.begin())
                   cout << ", " <<  *itr;
           }
           
           cout << endl;

       }

   }
   
   else if(stats == true and sum == false)
   {
       if (totalIDs != 0)
       {
           sort(idList.begin(), idList.end());
           
           cout << "Total IDs: " << totalIDs << endl;
           cout << "List of IDs: ";
           for (it = idList.begin(); it != idList.end(); it++)
           {
               if(it == idList.begin())
                   cout << *it;
               if(it != idList.begin())
                   cout << ", " <<  *it;
           }
           cout<<endl;
       }
       else
       cout << "Total IDs: " << totalIDs << endl;    
   }
    
   else if(stats == true && sum == true)
   {
       if (totalIDs != 0)
       {
           sort(idList.begin(), idList.end());
           
           cout << "Total IDs: " << totalIDs << endl;
           cout << "List of IDs: ";
           for (it = idList.begin(); it != idList.end(); it++)
           {
               if(it == idList.begin())
                   cout << *it;
               if(it != idList.begin())
                   cout << ", " <<  *it;
           }
           cout<<endl;
       }
       else
       cout << "Total IDs: " << totalIDs << endl;  
       
       if(tokens == 0){
           cout << "Total Lines: " << currentLine << endl;
           cout << "Total tokens: " << tokens << endl;
       }
       else
       {
           cout << "Total Lines: " << currentLine << endl;
           cout << "Total tokens: " << tokens << endl;
           cout << "Most frequently used tokens: ";
           
           
           for(int i = 0; i < vecCount.size(); ++i)
           {
               if((vecCount[i].second/tokens) >= avg)
               {
                   count.push_back(vecCount[i].first);
               }
           }
           
           
           for (itr = count.begin(); itr != count.end(); itr++)
           {
               if(itr == count.begin())
                   cout << *itr;
               if(itr != count.begin())
                   cout << ", " <<  *itr;
           }
           
           cout << endl;

       }
       
       
       
   }
	
   return 0;
}