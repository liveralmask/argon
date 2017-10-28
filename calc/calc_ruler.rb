require File.expand_path( "../argon_ruler", File.dirname( __FILE__ ) )

class CalcRuler < ArgonRuler
  def initialize
    super
    
    token( /0|[1-9][0-9]*|0x[0-9a-fA-F]+/, :INTEGER, true )
    token( /\+/, '+' )
    token( /\-/, '-' )
    token( /\*/, '*' )
    token( '\/', '/' )
    token( /\%/, '%' )
    token( /\(/, '(' )
    token( /\)/, ')' )
    token( /\<\</, :L_SHIFT )
    token( /\>\>/, :R_SHIFT )
    token( /[ \t]/ )
    
    priority( :left, "L_SHIFT R_SHIFT" )
    priority( :left, "'+' '-' '%'" )
    priority( :left, "'*' '/'" )
    
    syntax( :program, "expr", 0 )
    syntax( :lt_integer, "INTEGER" )
    syntax( :lt_integer, "'+' INTEGER", 1 )
    syntax( :lt_integer, "'-' INTEGER", -1 )
    syntax( :expr, "lt_integer" )
    syntax( :expr, "expr '*' expr", '*' )
    syntax( :expr, "expr '/' expr", '/' )
    syntax( :expr, "expr '+' expr", '+' )
    syntax( :expr, "expr '-' expr", '-' )
    syntax( :expr, "expr '%' expr", '%' )
    syntax( :expr, "expr L_SHIFT expr", :L_SHIFT )
    syntax( :expr, "expr R_SHIFT expr", :R_SHIFT )
    syntax( :expr, "'(' expr ')'", 1 )
  end
end
