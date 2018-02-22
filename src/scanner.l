%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "parser_driver.h"
# include "parser.yy.hpp"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1
static yy::location loc;
%}

%option noyywrap nounput batch debug noinput

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
double [0-9]+"."[0-9]+
blank [ \t]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+    { loc.step (); }
[\n]+       { loc.lines (yyleng); loc.step ();}

"+"         { return yy::parser::make_PLUS(loc); }
"-"         { return yy::parser::make_SUBTRACT(loc); }
"*"         { return yy::parser::make_MULTIPLY(loc); }
"/"         { return yy::parser::make_DIVIDE(loc); }
"<="        { return yy::parser::make_LEQ(loc); }
"("         { return yy::parser::make_LPAREN(loc); }
")"         { return yy::parser::make_RPAREN(loc); }
"true"      { return yy::parser::make_TRUE(loc); }
"false"     { return yy::parser::make_FALSE(loc); }
"if"        { return yy::parser::make_IF(loc); }
"else"      { return yy::parser::make_ELSE(loc); }
"then"      { return yy::parser::make_THEN(loc); }
"NaN"       { return yy::parser::make_NAN(loc); }

{double} {
    errno = 0;
    double n = strtod (yytext, NULL);

    return yy::parser::make_DOUBLE(n, loc);
}

{int} {
    errno = 0;
    long n = strtol (yytext, NULL, 10);
    if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
        driver.error (loc, "integer is out of range");
    }
    return yy::parser::make_INT(n, loc);
}

.          { driver.error (loc, "invalid character"); }
<<EOF>>    { return yy::parser::make_END(loc); }
%%

void parser_driver::scan_begin () {
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-") {
        yyin = stdin;
    } else if (!(yyin = fopen (file.c_str (), "r"))) {
        error (loc, "cannot open " + file + ": " + strerror(errno));
        exit (EXIT_FAILURE);
    }
}

void parser_driver::scan_end () {
    fclose (yyin);
}