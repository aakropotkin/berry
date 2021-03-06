
#include "be_mem.h"
#include "be_object.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define READLINE_STEP    100

static int
l_assert( bvm * vm )
{
  /* assertion fails when there is no argument
   * or the first argument is nil or false. */
  if ( !be_top( vm ) || !be_tobool( vm, 1 ) ) be_pusherror( vm,
                                                            "assert failed!" );
  be_return_nil( vm );
}

static int
l_print( bvm * vm )
{
  int i, n = be_top( vm );

  for ( i = 1; i <= n; ++i )
    {
      const char * str = be_tostring( vm, i );
      size_t       len = be_strlen( vm, i );
      be_fwrite( stdout, str, len );
      if ( i < n ) be_fwrite( stdout, " ", 1 );
    }
  be_fwrite( stdout, "\n", 1 );
  be_return_nil( vm );
}

static char *
m_readline()
{
  size_t pos = 0, size = READLINE_STEP;
  char * buffer = be_malloc( size );
  char * res    = be_fgets( stdin, buffer, (int) size );

  while ( res )
    {
      pos += strlen( buffer + pos ) - 1;
      if ( !pos || buffer[pos] == '\n' ) break;
      size  += READLINE_STEP;
      buffer = be_realloc( buffer, size );
      res    = be_fgets( stdin, buffer + pos + 1, READLINE_STEP );
    }

  return( buffer );
}

static int
l_input( bvm * vm )
{
  char * line;

  if ( be_top( vm ) && be_isstring( vm, 1 ) ) /* echo prompt */
    be_writestring( be_tostring( vm, 1 ) );
  line = m_readline();
  be_pushstring( vm, line );
  be_free( line );
  be_return( vm );
}

static int
l_clock( bvm * vm )
{
  be_pushreal( vm, clock() / (breal) CLOCKS_PER_SEC );
  be_return( vm );
}

static int
l_exit( bvm * vm )
{
  int status = 0;

  if ( be_top( vm ) && be_isint( vm, -1 ) ) status = be_toindex( vm, -1 );
  be_exit( vm, status );
  be_return_nil( vm );
}

static int
l_memcount( bvm * vm )
{
  size_t count = be_mcount();

  if ( count < 0x80000000 ) be_pushint( vm, (bint) count );
  else be_pushreal( vm, (breal) count );
  be_return( vm );
}

static int
l_super( bvm * vm )
{
  if ( be_top( vm ) )
    {
      be_getsuper( vm, 1 );
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_type( bvm * vm )
{
  if ( be_top( vm ) )
    {
      be_pushstring( vm, be_typename( vm, 1 ) );
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_classname( bvm * vm )
{
  if ( be_top( vm ) )
    {
      const char * t = be_classname( vm, 1 );
      if ( t )
        {
          be_pushstring( vm, t );
          be_return( vm );
        }
    }
  be_return_nil( vm );
}

static int
l_number( bvm * vm )
{
  if ( be_top( vm ) )
    {
      if ( be_isstring( vm, 1 ) )
        {
          be_str2num( vm, be_tostring( vm, 1 ) );
          be_return( vm );
        }
      else if ( be_isnumber( vm, 1 ) )
        {
          be_pushvalue( vm, 1 );
          be_return( vm );
        }
    }
  be_return_nil( vm );
}

static int
l_int( bvm * vm )
{
  if ( be_top( vm ) )
    {
      if ( be_isstring( vm, 1 ) )
        {
          const char * s = be_tostring( vm, 1 );
          be_pushint( vm, be_str2int( s, NULL ) );
        }
      else if ( be_isreal( vm, 1 ) )
        {
          be_pushint( vm, (bint) be_toreal( vm, 1 ) );
        }
      else if ( be_isint( vm, 1 ) )
        {
          be_pushvalue( vm, 1 );
        }
      else
        {
          be_return_nil( vm );
        }
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_real( bvm * vm )
{
  if ( be_top( vm ) )
    {
      if ( be_isstring( vm, 1 ) )
        {
          const char * s = be_tostring( vm, 1 );
          be_pushreal( vm, be_str2real( s, NULL ) );
        }
      else if ( be_isint( vm, 1 ) )
        {
          be_pushreal( vm, (breal) be_toint( vm, 1 ) );
        }
      else if ( be_isreal( vm, 1 ) )
        {
          be_pushvalue( vm, 1 );
        }
      else
        {
          be_return_nil( vm );
        }
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_iterator( bvm * vm )
{
  if ( be_isinstance( vm, 1 ) )
    {
      be_getmember( vm, 1, "iter" );
      be_pushvalue( vm, 1 );
      be_call( vm, 1 );
      be_pop( vm, 1 );
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_hasnext( bvm * vm )
{
  if ( be_isinstance( vm, 1 ) )
    {
      be_getmember( vm, 1, "hasnext" );
      be_pushvalue( vm, 1 );
      be_call( vm, 1 );
      be_pop( vm, 1 );
    }
  else
    {
      be_pushbool( vm, bfalse );
    }
  be_return( vm );
}

static int
l_next( bvm * vm )
{
  if ( be_isinstance( vm, 1 ) )
    {
      be_getmember( vm, 1, "next" );
      be_pushvalue( vm, 1 );
      be_call( vm, 1 );
      be_pop( vm, 1 );
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
l_str( bvm * vm )
{
  if ( be_top( vm ) ) be_tostring( vm, 1 );
  else be_pushstring( vm, "" );
  be_return( vm );
}

static int
l_length( bvm * vm )
{
  if ( be_top( vm ) && be_isstring( vm, 1 ) )
    {
      be_pushint( vm, be_strlen( vm, 1 ) );
      be_return( vm );
    }
  be_return_nil( vm );
}

static int
m_compile_str( bvm * vm )
{
  int          len = be_strlen( vm, 1 );
  const char * src = be_tostring( vm, 1 );
  int          res = be_loadbuffer( vm, "string", src, len );

  if ( res == BE_OK ) be_return( vm );
  be_return_nil( vm );
}

static int
m_compile_file( bvm * vm )
{
  const char * fname = be_tostring( vm, 1 );
  int          res   = be_loadfile( vm, fname );

  if ( res == BE_OK ) be_return( vm );
  be_return_nil( vm );
}

static int
l_compile( bvm * vm )
{
  if ( be_top( vm ) && be_isstring( vm, 1 ) )
    {
      if ( be_top( vm ) >= 2 && be_isstring( vm, 2 ) )
        {
          const char * s = be_tostring( vm, 2 );
          if ( !strcmp( s, "string" ) ) return( m_compile_str( vm ) );

          if ( !strcmp( s, "file" ) ) return( m_compile_file( vm ) );
        }
      else
        {
          return( m_compile_str( vm ) );
        }
    }
  be_return_nil( vm );
}

static int
l_codedump( bvm * vm )
{
  if ( be_top( vm ) >= 1 ) be_codedump( vm, 1 );
  be_return_nil( vm );
}

void
be_load_baselib( bvm * vm )
{
  be_regfunc( vm, "assert", l_assert );
  be_regfunc( vm, "print", l_print );
  be_regfunc( vm, "read", l_input );
  be_regfunc( vm, "clock", l_clock );
  be_regfunc( vm, "exit", l_exit );
  be_regfunc( vm, "super", l_super );
  be_regfunc( vm, "sizeof", l_memcount );
  be_regfunc( vm, "typeof", l_type );
  be_regfunc( vm, "classof", l_classname );
  be_regfunc( vm, "number", l_number );
  be_regfunc( vm, "str", l_str );
  be_regfunc( vm, "int", l_int );
  be_regfunc( vm, "real", l_real );
  be_regfunc( vm, "length", l_length );
  be_regfunc( vm, "compile", l_compile );
  be_regfunc( vm, "codedump", l_codedump );
  be_regfunc( vm, "__iterator__", l_iterator );
  be_regfunc( vm, "__hasnext__", l_hasnext );
  be_regfunc( vm, "__next__", l_next );
}

/*
   Used to generate compile-time strings:
   be_const_str_assert
   be_const_str_print
   be_const_str_input
   be_const_str_clock
   be_const_str_exit
   be_const_str_super
   be_const_str_memcount
   be_const_str_type
   be_const_str_classname
   be_const_str_number
   be_const_str_str
   be_const_str_int
   be_const_str_real
   be_const_str_length
   be_const_str_compile
   be_const_str___iterator__
   be_const_str___hasnext__
   be_const_str___next__
 */
