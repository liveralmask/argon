class ArgonRuler
  def initialize
    @tokens = []
    @syntaxes = {}
    @priorities = []
  end
  
  def token( rule, type = nil, is_node = false )
    rule = rule.source if rule.instance_of?( Regexp )
    codes = []
    if ! type.nil?
      type = "'#{type}'" if type.instance_of?( String )
      codes.push "parser->line = yylineno;"
      codes.push "data->node = argon_node_new_token( parser, #{type}, yytext, (size_t)yyleng );" if is_node
      codes.push "return #{type};"
    end
    @tokens.push({ :name => is_node ? type.to_s : nil, :code => <<EOS
#{rule} {
  #{codes.join( "\n  " )}
}
EOS
    })
  end
  
  def syntax( id, rule, type = nil )
    type = "'#{type}'" if type.instance_of?( String )
    value = { :rule => rule, :type => type }
    if ! @syntaxes.key?( id )
      @syntaxes[ id ] = [ value ]
    else
      @syntaxes[ id ].push value
    end
  end
  
  def priority( type, value )
    @priorities.push({ :type => type, :value => value })
  end
  
  def generate
    open( "./lexer.l", "wb" ){|f|
      f.puts <<EOS
%{
#include <argon.h>

#define YY_DECL int yylex( YYSTYPE* data, ArgonParser* parser )
%}

%option noyywrap
%option nounput
%option noinput
%option yylineno

%%
EOS
      
      @tokens.each{|token|
        f.puts <<EOS
#{token[ :code ]}
EOS
      }
      
      f.puts <<EOS
. {
  char c = yytext[ 0 ];
  fprintf( stderr, "token error, %c(0x%02X) %s:%zd\\n", ( 0 == isprint( c ) ) ? ' ' : c, (unsigned char)c, parser->src, parser->line );
  exit( EXIT_FAILURE );
}
%%
EOS
    }
    
    open( "./parser.y", "wb" ){|f|
      f.puts <<EOS
%{
#include <argon.h>

#define YYERROR_VERBOSE 1
#define YYDEBUG 1
%}

%union {
  void* node;
}

%pure-parser
%parse-param {ArgonParser* parser}
%lex-param {ArgonParser* parser}

%{
int yylex( YYSTYPE* data, ArgonParser* parser );
static void yyerror( ArgonParser* parser, const char* string ){
  fprintf( stderr, "%s %s:%zd\\n", string, parser->src, parser->line );
}
%}

EOS
      
      types = [ "<node>" ]
      @tokens.each{|token|
        next if token[ :name ].nil?
        
        f.puts "%token #{token[ :name ]}"
        types.push token[ :name ]
      }
      
      @syntaxes.each{|id, value|
        types.push id.to_s
      }
      f.puts "\n%type #{types.join( ' ' )}"
      
      @priorities.each{|priority|
        f.puts "%#{priority[ :type ]} #{priority[ :value ]}"
      }
      
      f.puts "\n%%"
      @syntaxes.each{|id, values|
        indent = " " * ( id.length + 1 )
        codes = []
        values.each{|value|
          if value[ :type ].nil?
            codes.push "#{value[ :rule ]}"
          else
            args = []
            nodes = value[ :rule ].split( " " )
            nodes.each_with_index{|node, i|
              args.push "$#{i + 1}" if types.include?( node )
            }
            codes.push "#{value[ :rule ]} { $$ = argon_node_new_syntax( parser, #{value[ :type ]}, #{args.length}, #{args.join( ', ' )} ); }"
          end
        }
        f.puts <<EOS
#{id} : #{codes.join( "\n#{indent}| " )}
EOS
      }
      f.puts "%%"
    }
  end
end
