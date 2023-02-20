#include<bits/stdc++.h>
using namespace std;
vector<pair<string,int>>tkn;
//if else recursive desent parser
char* S(char *_token, const char s[2]);
char* _S(char *_token, const char s[2]);

//function to move pointer
char* moveip(char *_token, const char s[2]){
    if(_token != "$"){
        return strtok(NULL, s);
    }
    else{
        return NULL;
    }
}
//function for symbol A
//A --> st ;
char* A(char *_token, const char s[2]){
    int value;
    if(_token != NULL && strcmp(_token, "st")==0){
        _token = moveip(_token,s);
        if(_token != NULL && strcmp(_token, ";")==0){
            _token = moveip(_token,s);
            return _token;
        }
        else{
            return NULL;
        }
    }
    else{
        return NULL;
    }
}

//function for symbol B
//B --> if ( c ) { S }
char* B(char *_token, const char s[2]){
    int value;
    if(_token != NULL && strcmp(_token, "if") == 0){
        _token = moveip(_token, s);
        if(_token != NULL && strcmp(_token, "(") == 0){
            _token = moveip(_token, s);
            if(_token != NULL && strcmp(_token, "c") == 0){
                _token = moveip(_token, s);
                if(_token != NULL && strcmp(_token, ")") == 0){
                    _token = moveip(_token, s);
                    if(_token != NULL && strcmp(_token, "{") == 0){
                        _token = moveip(_token, s);
                        _token = S(_token, s);
                        if(_token != NULL && strcmp(_token,"}") == 0){
                            _token = moveip(_token, s);
                            return _token;
                        }
                        else{
                            return NULL;
                        }
                    }
                    else{
                        return NULL;
                    }
                }
                else{
                    return NULL;
                }
            }
            else{
                return NULL;
            }
        }
        else{
            return NULL;
        }
    }
    else{
        return NULL;
    }
}
//function for start symbol S
//S --> B | B else { S } | A
char* S(char *_token, const char s[2]){
    if(strcmp(_token, "$")==0){
        return _token;
    }
    char* temptok;
    temptok = B(_token, s);
    if(temptok != NULL){
        if(strcmp(temptok, "else")==0){
            temptok = moveip(temptok, s);
            if(temptok != NULL && strcmp(temptok, "{")==0){
                temptok = moveip(temptok, s);
                temptok = S(temptok, s);
                if(temptok != NULL && strcmp(temptok, "}")==0){
                    temptok = moveip(temptok, s);
                    _token = temptok;
                    return _token;
                }
                else{
                    return NULL;
                }
            }
            else{
                return NULL;
            }
        }
        else{
            _token = temptok;
            return _token;
        }
    }
    temptok = A(_token, s);
    if(temptok != NULL){
        if(strcmp(temptok, "}")==0 || strcmp(temptok, "$")==0){
            _token = temptok;
            return _token;
        }
        return _token;
    }
}
//function for starting symbol S'
//S' --> S | SS'
char* _S(char *_token, const char s[2]){
    if(strcmp(_token, "$")==0){
        return _token;
    }
    char* temptok;
    temptok = S(_token, s);
    if(temptok != NULL){
        if(strcmp(temptok, "$")==0){
            return temptok;
        }
        temptok = _S(temptok, s);
    }
}
//expression parsing

enum types { DELIMITER = 1, VARIABLE, NUMBER, FUNCTION };
const int NUMVARS = 26;
class parser {
    char *exp_ptr; // points to the expression
    char token[256]; // holds current token
    char tok_type; // holds token's type
    double vars[NUMVARS]; // holds variable's values
    void eval_exp1(double &result);
    void eval_exp2(double &result);
    void eval_exp3(double &result);
    void eval_exp4(double &result);
    void eval_exp5(double &result);
    void eval_exp6(double &result);
    void get_token();
public:
    parser();
    double eval_exp(char *exp);
    char errormsg[64];
};
// Parser constructor.
parser::parser()
{
    int i;
    exp_ptr = NULL;
    for (i = 0; i < NUMVARS; i++)
    vars[i] = 0.0;
    errormsg[0] = '\0';
}
// Parser entry point.
double parser::eval_exp(char *exp)
{
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token)
    {
        strcpy(errormsg, "No Expression Present"); // no expression present
        return (double)0;
    }
    eval_exp1(result);
    if (*token) // last token must be null
        strcpy(errormsg, "Syntax Error");
    return result;
}
// Process an assignment.
void parser::eval_exp1(double &result)
{
    int slot;
    char temp_token[80];
    if (tok_type == VARIABLE)
    {
        // save old token
        char *t_ptr = exp_ptr;
        strcpy(temp_token, token);
        // compute the index of the variable
        slot = *token - 'A';
        get_token();
        if (*token != '=')
        {
            exp_ptr = t_ptr; // return current token
            strcpy(token, temp_token); // restore old token
            tok_type = VARIABLE;
        }
        else {
            get_token(); // get next part of exp
            eval_exp2(result);
            vars[slot] = result;
            return;
        }
    }
    eval_exp2(result);
}
// Add or subtract two terms.
void parser::eval_exp2(double &result)
{
    register char op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-')
    {
        get_token();
        eval_exp3(temp);
        switch (op)
        {
        case '-':
            result = result - temp;
            break;
        case '+':
            result = result + temp;
            break;
        }
    }
}
// Multiply or divide two factors.
void parser::eval_exp3(double &result)
{
    register char op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/')
    {
        get_token();
        eval_exp4(temp);
        switch (op)
        {
        case '*':
            result = result * temp;
            break;
        case '/':
            result = result / temp;
            break;
        }
    }
}
// Process an exponent.
void parser::eval_exp4(double &result)
{
    double temp;
    eval_exp5(result);
    while (*token == '^')
    {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}
// Evaluate a unary + or -.
void parser::eval_exp5(double &result)
{
    register char op;
    op = 0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-')
    {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-')
        result = -result;
}
// Process a function, a parenthesized expression, a value or a variable
void parser::eval_exp6(double &result)
{
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc)
    {
        strcpy(temp_token, token);
        get_token();
    }
    if ((*token == '('))
    {
        get_token();
        eval_exp2(result);
        if (*token != ')')
            strcpy(errormsg, "Unbalanced Parentheses in expression ");
        get_token();
    }
    else
        switch (tok_type)
        {
        case VARIABLE:
            result = vars[*token - 'A'];
            get_token();
            return;
        case NUMBER:
            result = atof(token);
            get_token();
            return;
        default:
            strcpy(errormsg, "Syntax Error");
        }
}
// Obtain the next token.
void parser::get_token()
{
    register char *temp;
    tok_type = 0;
    temp = token;
    *temp = '\0';
    if (!*exp_ptr)  // at end of expression
        return;
    while (isspace(*exp_ptr))  // skip over white space
        ++exp_ptr;
    if (strchr("+-*/%^=()", *exp_ptr))
    {
        tok_type = DELIMITER;
        *temp++ = *exp_ptr++;  // advance to next char
    }
    else if (isalpha(*exp_ptr))
    {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        while (isspace(*exp_ptr))  // skip over white space
            ++exp_ptr;
        tok_type = (*exp_ptr == '(') ? FUNCTION : VARIABLE;
    }
    else if (isdigit(*exp_ptr) || *exp_ptr == '.')
    {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    if ((tok_type == VARIABLE) && (token[1]))
        strcpy(errormsg, "Only first letter of variables is considered");
}
//REGEX class implement here
class REGEX
{
public:
    regex remove_front_space()
    {
        return regex("^[ \t]*");
    }
    regex remove_back_space()
    {
        return regex("[ \t]*$");
    }
    regex remove_comment_line()
    {
        return regex("[ \t]*//");
    }
    regex header_file_cheak()
    {
        return regex("#[ \t]*include[ \t]*<(stdio.h|string.h|math.h|dog.h)[ \t]*>");
    }
    regex main_function_cheak()
    {
        return regex ("int|void[ \t]+main[ \t]*([ \t]*)");
    }
    regex return_cheak()
    {
        return regex ("return[ \t]+[0-9]*[ \t]*;");
    }
    regex loop_cheak()
    {
        //return regex("rof[ \t]*([ \t]*[a-z]+=[0-9]+[ \t]*;[a-z]+[ \t]*<[ \t]*[0-9]+[ \t]*;[ \t]*[a-z]+ ++)");
        return regex("rof[(][a-z]+=[0-9]+;[a-z]+<[0-9]+;[a-z]+++[)]");
    }
};
//check if the given token is a keyword or not
bool isKeyword(string str,int ln)
{
string keywords[] = {"auto","break","case","char","const","continue","default",
"do","double","else","enum","extern","float","for","goto",
"if","int","long","register","return","short","signed",
"sizeof","static","struct","switch","typedef","union",
"unsigned","void","volatile","while"};
    for(int i=0;i<32;i++)
    {
        if(str==keywords[i])
        {
           return true;
        }
    }
    return false;
}
//check if the given token is a Operator or not
bool isOperator(string str,int ln)
{
   string value[]={"+","-","*","/",">","<","!","=","(",")","{","}","|","&"};
    for(int i=0;i<14;i++)
    {
        if(value[i]==(str))
        {
            return true;
        }
    }
    return false;
}
//check if the given token is a identifier or not
bool validIdentifier(string str,int ln)
{
    if(str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9')
        {
            return false;
        }
    string s('1',str[0]);
    if(str.size()==1)
    {
        return true;
    }
    if(isOperator(s,ln))
    {
        return false;
    }
    for(int i=0;i<str.size();i++)
    {
       string s('1',str[i]);
        if(isOperator(s,ln))
        {
        return false;
        }
    }
 return true;
}
//check if the given token is a number or not
bool isNumber(string str,int ln)
{
    for(int i=0;i<str.size();i++)
    {
        if(str[i]>='0'&&str[i]<='9')
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}
//create a symbol table
void symbol_table(string tok,int ln)
{
  if(isKeyword(tok,ln))
  {
    cout<<tok<<" is a keyword"<<endl;
  }
  else if(isOperator(tok,ln))
  {
      if(tok=="{"||tok=="}"||tok=="("||tok==")")
        {
            cout<<tok<<" is a parenthesis"<<endl;
        }
        else
        {
          cout<<tok<<" is an Operator"<<endl;
        }

  }
  else if(validIdentifier(tok,ln))
  {
     cout<<tok<<" is a valid identifier"<<endl;
  }
  else if(isNumber(tok,ln))
  {
      cout<<tok<<" is a number"<<endl;
  }
  else
  {
      cout<<tok<<" an Invalid Keyword in line number "<<ln<<endl;
  }
}
bool is_separator(char ch)
{
    string value=" ,;(){}[]";
    for(int i=0;i<value.size();i++)
    {
        if(value[i]==ch)
        {
            return true;
        }
    }
    return false;
}
bool is_operator(char ch)
{
    string value="+-*/><!=";
    for(int i=0;i<value.size();i++)
    {
        if(value[i]==ch)
        {
            return true;
        }
    }
    return false;
}
void Token(string str,int line)
{
   string token="";
   for(int i=0;i<str.size();i++)
   {
       if(str[i]=='('||str[i]==')'||str[i]=='{'||str[i]=='}')
       {
           string s(1,str[i]);
           tkn.push_back({s,line});
       }
       if(is_separator(str[i]))
       {
         if(token!="")
         {
             tkn.push_back({token,line});
             token="";
         }
       }
       else if(is_operator(str[i]))
       {
           if(token!="")
           {
               tkn.push_back({token,line});
               char c=str[i];
               string s1(1,c);
               tkn.push_back({s1,line});
               token="";
           }
       }
       else
       {
           token.push_back(str[i]);
       }
   }
}
int main()
{
    REGEX ob;
    ifstream read;
    read.open("text.txt");
    vector<string>v;
    vector<char>brack;
    vector<pair<string,int>>v1;
    bool flag=false,flag1=false,flag2=false,flag3=false;
    int count=0,count1=0,count2=0,x=0,y=0,p=1,l;
    bool cheak=false,w=0;
    bool cheak1=false;
    map<string,int>mp;
    string prev="";
    if(read.is_open())
    {
        string str;
        while(getline(read,str))
        {
           v.push_back(str);
        }
        regex rg("^[ \t]*");
        smatch match;
        for(int i=0;i<v.size();i++)
        {
             string s;
             flag=false;
            if(regex_search(v[i],match,ob.remove_front_space()))
            {
              s=match.suffix().str();
            }
            if(regex_search(s,match,ob.remove_back_space()))
            {
              s=match.prefix().str();
            }
            if(regex_search(s,match,ob.remove_comment_line()))
            {
              //s=match.suffix().str();
              flag=true;
            }
            if(!s.empty()&&!flag)
            {
                v1.push_back(make_pair(s,i+1));
            }
        }
        for(auto it:v1)
        {
            string s1=it.first;
            if(s1[0]=='#')
            {
                count2++;
               if(regex_search(it.first,match,ob.header_file_cheak()))
               {
                   //cout<<it.first<<"what"<<endl;
                   if(it.first=="#include<dog.h>")
                   {
                     cheak=true;
                   }
                 count++;
                  l=it.second;
               }
               else
               {
                   cerr<<"Wrong header file decleard in line "<<it.second<<endl;
                   return -1;
               }
            }
              else if(regex_search(it.first,match,ob.main_function_cheak())&&p==1)
               {
                   p++;
                   if(count2+1!=it.second)
                   {
                       cout<<"wrong header file declere"<<endl;
                       exit(0);
                   }
                    flag1=true;
               }
              else if(s1=="{"||s1=="}")
                {
                  brack.push_back(s1[0]);
                }

              else if(regex_search(it.first,match,ob.return_cheak()))
              {
                flag2=true;
              }
              else if(regex_search(it.first,match,ob.loop_cheak()))
              {
                  cheak1=true;
                  cout<<"yes"<<endl;
              }

        }
        if(count==0)
        {
            cerr<<"Does not exist any header file"<<endl;
        }
        else
        {

            for(auto it:brack)
            {

                if(it=='{')
                    {
                        count1++;
                        x++;
                    }
                   else if(it=='}')
                        {
                            y++;
                            count1--;
                            if(count1<0)
                            {
                                break;
                            }
                        }
            }
            if(!flag1)
            {
                cout<<"main function is not declared in line number "<<l<<endl;
            }
            if(!flag2)
            {
                cout<<"return keyword is not correctly declared line number "<<v.size()-1<<endl;
            }
            if(count1<0||x!=y)
            {
                cout<<"parenthesis is not balanced"<<endl;
            }
            if(flag1&&flag2&&count1==0)
            {
                for(int i=l;i<v1.size();i++)
                {
                  string s=v1[i].first;
                  int ln=v1[i].second;
                  Token(s,ln);
                }
                 for(auto st:tkn)
                  {
                      if(st.first=="leg"||st.first=="mouth")
                      {
                        if(!cheak)
                        {
                          cout<<"wrong header file declared and unexpected token in line number "<<st.second<<endl;
                          exit(0);
                        }
                      }
                      prev=it.first;
                  }
                 for(auto st:tkn)
                  {
                  symbol_table(st.first,st.second);
                  }
                  cout<<endl;
                  cout<<endl;
                  string exp,p,concat="";
                  int count=0,count1=0;
                  parser ob;
                  for(auto it:v1)
                  {
                      p=it.first;
                      count=0;
                      for(int i=0;i<p.size();i++)
                      {
                          if((p[i]=='='||p[i]=='+'||p[i]=='-'||p[i]=='*'||p[i]=='/')&&count==0)
                          {
                              if(p[p.size()-1]!=';')
                              {
                                cout<<"semicolon is not declared in line number "<<it.second<<endl;
                                exit(0);
                              }
                            exp=p;
                            int sz=exp.size();
                            char expstr[sz];
                            for(int i=0;i<exp.size()-1;i++)
                            {
                             expstr[i]=exp[i];
                            }
                            double ans = ob.eval_exp(expstr);
                            if (*ob.errormsg)
                            {
                            cout << "Error: " << ob.errormsg <<"in line number "<<it.second<<endl;
                            count=1;
                            }

                            else
                           // cout << "Answer: " << ans << "\n\n";
                            count=1;
                            for(int i=0;i<sz;i++)
                            {
                             expstr[i]='\0';
                            }
                            exp.clear();
                          }
                         if((p[0]=='i'&&p[1]=='f')||(p[0]=='e'&&p[1]=='l'&&p[2]=='s'&&p[3]=='e')&&count1==0)
                         {
                           for(int j=it.second-1;j<v1.size()-2;j++)
                           {
                               if(count1==0)
                               {
                              string q=v1[j].first;
                              concat+=q;
                               }
                           }
                           count1=1;
                         }
                      }
                  }
                  char command[concat.size()];
                  int m=0;
                  for(int k=0;k<concat.size();k++)
                  {
                   if(concat[k]=='i'&&concat[k+1]=='f')
                   {
                       k++;
                       command[m++]='i';
                       command[m++]='f';
                       command[m++]=' ';
                   }
                   else if(concat[k]=='e'&&concat[k+1]=='l'&&concat[k+2]=='s'&&concat[k+3]=='e')
                   {
                       command[m++]='e';
                       command[m++]='l';
                       command[m++]='s';
                       command[m++]='e';
                       command[m++]=' ';
                       k++;
                       k++;
                       k++;
                   }
                   else if(concat[k]=='s'&&concat[k+1]=='t')
                   {
                     command[m++]='s';
                     command[m++]='t';
                      command[m++]=' ';
                     k++;
                   }
                   else
                   {
                       if(concat[k]!=' ')
                       {
                           command[m++]=concat[k];
                            command[m++]=' ';
                       }
                   }
                  }
                command[m++]=' ';
                command[m++]='$';

                const char s[2] = " ";
     int invalid = 0;
     char *token;
    /* get the first token */
    token = strtok(command, s);
    //call the function defined for start symbol
    char* final = _S(token, s);
    if(final != NULL && strcmp(final, "$")==0){
        printf("if else block is syntactically correct\n");
    }
    else{
        printf("if else block is not syntactically correct\n");
    }

                 cout<<"Build success"<<endl;

            }
        }
    }
    else
    {
        cerr<<"file does not exist"<<endl;
    }



return 0;
}
