/*
Copyright (c) 2013. The YARA Authors. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#undef yyparse
#undef yylex
#undef yyerror
#undef yywarning
#undef yyfatal
#undef yychar
#undef yydebug
#undef yynerrs
#undef yyget_extra
#undef yyget_lineno

#undef YY_FATAL_ERROR
#undef YY_DECL
#undef LEX_ENV

#define yyparse      re_yyparse
#define yylex        re_yylex
#define yyerror      re_yyerror
#define yywarning    re_yywarning
#define yyfatal      re_yyfatal
#define yychar       re_yychar
#define yydebug      re_yydebug
#define yynerrs      re_yynerrs
#define yyget_extra  re_yyget_extra
#define yyget_lineno re_yyget_lineno

// Define the ECHO macro as an empty macro in order to avoid the default
// implementation from being used. The default implementation of ECHO
// prints to the console any byte that is not matched by the lexer. It's
// not safe to print random bytes to the console as it may cause the calling
// program to terminate. See: https://github.com/VirusTotal/yara/issues/2007
#define ECHO

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#define YY_EXTRA_TYPE RE_AST*
#define YY_USE_CONST

#define VALID_ESCAPE_SEQUENCE   1
#define UNKNOWN_ESCAPE_SEQUENCE 2

typedef struct _RE_LEX_ENVIRONMENT
{
  RE_CLASS re_class;
  int last_error;
  char last_error_message[256];
  bool strict_escape;

} RE_LEX_ENVIRONMENT;

#define LEX_ENV ((RE_LEX_ENVIRONMENT*) lex_env)

// The default behavior when a fatal error occurs in the parser is calling
// exit(YY_EXIT_FAILURE) for terminating the process. This is not acceptable
// for a library, which should return gracefully to the calling program. For
// this reason we redefine the YY_FATAL_ERROR macro so that it expands to our
// own function instead of the one provided by default.
#define YY_FATAL_ERROR(msg) re_yyfatal(yyscanner, msg)

#include "re_grammar.h"


#define YY_DECL \
  int re_yylex( \
      YYSTYPE* yylval_param, yyscan_t yyscanner, RE_LEX_ENVIRONMENT* lex_env)

YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);

int yylex(
    YYSTYPE* yylval_param,
    yyscan_t yyscanner,
    RE_LEX_ENVIRONMENT* lex_env);

void yyerror(
    yyscan_t yyscanner,
    RE_LEX_ENVIRONMENT* lex_env,
    const char* error_message);

void yywarning(
    yyscan_t yyscanner,
    RE_LEX_ENVIRONMENT* lex_env,
    const char* error_message);

void yyfatal(yyscan_t yyscanner, const char* error_message);

int yyparse(void* yyscanner, RE_LEX_ENVIRONMENT* lex_env);

int yr_parse_re_string(
    const char* re_string,
    RE_AST** re_ast,
    RE_ERROR* error,
    int flags);
