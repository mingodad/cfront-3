/*ident	"@(#)cls4:incl-master/const-headers/iostream.h	1.6" */
/*******************************************************************************

C++ source for the C++ Language System, Release 3.0.  This product
is a new release of the original cfront developed in the computer
science research center of AT&T Bell Laboratories.

Copyright (c) 1991, 1992 AT&T and UNIX System Laboratories, Inc.
Copyright (c) 1984, 1989, 1990 AT&T.  All Rights Reserved.

THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of AT&T and UNIX System
Laboratories, Inc.  The copyright notice above does not evidence
any actual or intended publication of such source code.

*******************************************************************************/
#ifndef IOSTREAMH
#define IOSTREAMH

#pragma lib "ape/libap.a"
#pragma lib "c++/libC.a"

#include <memory.h>
		/* Some inlines use memcpy */

/* If EOF is defined already verify that it is -1.  Otherwise
 * define it.
 */

#ifdef EOF
#	if EOF!=-1
#		define EOF (-1)
#	endif
#else
#	define EOF (-1)
#endif

/* Don't worry about NULL not being 0 */
#ifndef NULL
#	define NULL 0
#endif

#define	zapeof(c) ((c)&0377)
	    /* extracts char from c. The critical requirement is
	     *      zapeof(EOF)!=EOF
	     * ((c)&0377) and ((unsigned char)(c)) are alternative definitions
	     * whose efficiency depends on compiler environment.
	     */

typedef long streampos ;
typedef long streamoff ;

class streambuf ;
class ostream ;


class ios {
public: /* Some enums are declared in ios to avoid pollution of
	 * global namespace
	 */
	enum io_state	{ goodbit=0, eofbit=1, failbit=2, badbit=4,
				hardfail=0200};
				/* hard fail can be set and reset internally,
				 * but not via public function */
	enum open_mode	{ in=1, out=2, ate=4, app=010, trunc=020,
				nocreate=040, noreplace=0100} ;
	enum seek_dir	{ beg=0, cur=1, end=2 } ;

	/* flags for controlling format */
	enum		{ skipws=01,
					/* skip whitespace on input */
			  left=02,  right=04, internal=010,
					/* padding location */
			  dec=020, oct=040, hex=0100,
					/* conversion base */
			  showbase=0200, showpoint=0400, uppercase=01000,
			  showpos=02000,
					/* modifiers */
			  scientific=04000, fixed=010000,
					/* floating point notation */
			  unitbuf=020000, stdio=040000
					/* stuff to control flushing */
			  } ;
	static const long
			basefield ; /* dec|oct|hex */
	static const long
			adjustfield ; /* left|right|internal */
	static const long
			floatfield ; /* scientific|fixed */
public:
			ios(streambuf*) ;
	virtual		~ios() ;

	long		flags() const 	{ return x_flags ; }
	long		flags(long f);

	long		setf(long setbits, long field);
	long		setf(long) ;
	long		unsetf(long) ;

	int		width() const	{ return x_width ; }
	int		width(int w)
	{
			int i = x_width ; x_width = w ; return i ;
	}

	ostream*	tie(ostream* s);
	ostream*	tie()		{ return x_tie ; }
	char		fill(char) ;
	char		fill() const	{ return x_fill ; }
	int		precision(int) ;
	int		precision() const	{ return x_precision ; }

	int		rdstate() const	{ return state ; }
			operator void*()
				{
				void *retval;
				if (state&(failbit|badbit|hardfail)) retval = 0 ;
				else retval = this ;
				return retval;
				}
			operator const void*() const
				{
				const void *retval;
				if (state&(failbit|badbit|hardfail)) retval = 0 ;
				else retval = this ;
				return retval;
				}

	int		operator!() const
				{ return state&(failbit|badbit|hardfail); }
	int		eof() const	{ return state&eofbit; }
	int		fail() const	{ return state&(failbit|badbit|hardfail); }
	int		bad() const	{ return state&badbit ; }
	int		good() const	{ return state==0 ; }
	void		clear(int i =0)
				{
				state =  (i&0377) | (state&hardfail) ;
				ispecial = (ispecial&~0377) | state ;
				ospecial = (ospecial&~0377) | state ;
				}
	streambuf*	rdbuf() { return bp ;}

public: /* Members related to user allocated bits and words */
	long &		iword(size_t) ;
	void* &		pword(size_t) ;
	static long	bitalloc() ;
	static int	xalloc() ;

private: /*** privates for implemting allocated bits and words */
	static long	nextbit ;
	static long	nextword ;

	size_t		nuser ;
	union ios_user_union*
			x_user ;
	void	uresize(size_t) ;
public: /* static member functions */
	static void	sync_with_stdio() ;
protected:
	enum 		{ skipping=01000, tied=02000 } ;
			/*** bits 0377 are reserved for userbits ***/
	streambuf*	bp;
	void		setstate(int b)
			{	state |= (b&0377) ;
				ispecial |= b&~skipping ;
				ispecial |= b ;
			}
	int		state;
	int		ispecial;
	int		ospecial;
	int		isfx_special;
	int		osfx_special;
	int		delbuf;
	ostream*	x_tie;
	long 		x_flags;
	short		x_precision;
	char		x_fill;
	short 		x_width;

	static void	(*stdioflush)() ;

	void		init(streambuf*) ;
				/* Does the real work of a constructor */
			ios() ; /* No initialization at all. Needed by
				 * multiple inheritance versions */
	int		assign_private ;
				/* needed by with_assgn classes */
public:
			ios(ios&) ; /* Declared but not defined */
	void		operator=(ios&) ; /* Declared but not defined */
public:   /* old stream package compatibility */
	int		skip(int i) ;
};

class streambuf {
	short		alloc;
	short		x_unbuf;
	char* 		x_base;
	char*		x_pbase;
	char*		x_pptr;
	char* 		x_epptr;
	char* 		x_gptr;
	char*		x_egptr;
	char*		x_eback;
	size_t		x_blen;
    private:
			streambuf(streambuf&); /* Declared but not defined */
	void		operator=(streambuf&); /* Declared but not defined */
    public:
	void		dbp();
    protected:
	char*		base() 		{ return x_base ; }
	char*		pbase()		{ return x_pbase ; }
	char*		pptr() 		{ return x_pptr ; }
	char*		epptr() 	{ return x_epptr ; }
	char*		gptr() 		{ return x_gptr ; }
	char*		egptr() 	{ return x_egptr ; }
	char*		eback() 	{ return x_eback ; }
	char* 		ebuf()		{ return x_base+x_blen ; }
	size_t		blen() const	{ return x_blen; }
	void		setp(char*  p, char*  ep)
	{
		x_pbase=x_pptr=p ; x_epptr=ep ;
	}
	void		setg(char*  eb,char*  g, char*  eg)
	{
		x_eback=eb; x_gptr=g ; x_egptr=eg ;
	}
	void		pbump(size_t n)
	{
		x_pptr+=n ;
	}

	void		gbump(size_t n)
	{
		x_gptr+=n ;
		}

	void		setb(char* b, char* eb, int a = 0 )
	{
		if ( alloc && x_base ) delete x_base ;
		x_base = b ;
		x_blen= (eb>b) ? (eb-b) : 0 ;
		alloc = a ;
		}
	int		unbuffered() const { return x_unbuf; }
	void		unbuffered(int unb) { x_unbuf = (unb!=0)  ; }
	int		allocate()
	{
		int retval;
		if ( x_base== 0 && !unbuffered() ) retval = doallocate() ;
		else			  	 retval = 0 ;
		return retval;
	}
	virtual int 	doallocate();
    public :
	virtual int	overflow(int c=EOF);
	virtual int	underflow();
	virtual int	pbackfail(int c);
	virtual int	sync();
	virtual streampos
			seekoff(streamoff,ios::seek_dir,int =ios::in|ios::out);
	virtual streampos
			seekpos(streampos, int =ios::in|ios::out) ;
	virtual int	xsputn(const char*  s,int n);
	virtual int	xsgetn(char*  s,int n);

	int		in_avail()
	{
		return x_gptr<x_egptr ? x_egptr-x_gptr : 0 ;
	}

	int		out_waiting()
	{
		int retval;
		if ( x_pptr ) retval = x_pptr-x_pbase ;
		else	      retval = 0 ;
		return retval;
	}

	int		sgetc()
	{
		/***WARNING: sgetc does not bump the pointer ***/
		return (x_gptr>=x_egptr) ? underflow() : zapeof(*x_gptr);
	}
	int		snextc()
	{
		return (++x_gptr>=x_egptr)
				? x_snextc()
				: zapeof(*x_gptr);
	}
	int		sbumpc()
	{
		return  ( x_gptr>=x_egptr && underflow()==EOF )
				? EOF
				: zapeof(*x_gptr++) ;
	}
	int		optim_in_avail()
	{
		return x_gptr<x_egptr ;
	}
	int		optim_sbumpc()
	{
		return  zapeof(*x_gptr++) ;
	}
	void		stossc()
	{
		if ( x_gptr >= x_egptr ) underflow() ;
		else x_gptr++ ;
	}

	int		sputbackc(char c)
	{
		int retval;
		if (x_gptr > x_eback ) {
			if ( *--x_gptr == c ) retval = zapeof(c) ;
			else 		      retval = zapeof(*x_gptr=c) ;
		} else {
			retval = pbackfail(c) ;
		}
		return retval;
	}

	int		sputc(int c)
	{
		return (x_pptr>=x_epptr) ? overflow(zapeof(c))
				      : zapeof(*x_pptr++=c);
	}
	int		sputn(const char*  s,int n)
	 {
		int retval;
		if ( n <= (x_epptr-x_pptr) ) {
			memcpy(x_pptr,s,n) ;
			pbump(n);
			retval = n ;
		} else {
			retval = xsputn(s,n) ;
		}
		return retval;
	}
	int		sgetn(char*  s,int n)
	{
		int retval;
		if ( n <= (x_egptr-x_gptr) ) {
			memcpy(s,x_gptr,n) ;
			gbump(n);
			retval = n ;
		} else {
			retval = xsgetn(s,n) ;
		}
		return retval;
	}
	virtual streambuf*
			setbuf(char*  p, int len) ;
   	streambuf*	setbuf(unsigned char*  p, int len) ;

	streambuf*	setbuf(char*  p, int len, int count) ;
				/* obsolete third argument */
  			/*** Constructors -- should be protected ***/
			streambuf() ;
			streambuf(char*  p, int l) ;

			streambuf(char*  p, int l,int c) ;
			/* 3 argument form is obsolete.
			 * Use strstreambuf.
			 */
	virtual		~streambuf() ;
private:
	int		x_snextc() ;
};

class istream : virtual public ios {
public: /* Constructor */
			istream(streambuf*) ;
	virtual		~istream() ;
public:
	int		ipfx(int noskipws=0)
			{
				int retval;
				if ( noskipws?(ispecial&~skipping):ispecial) {
					retval = do_ipfx(noskipws) ;
				} else retval = 1 ;
				return retval;
			}
	void		isfx() { }
	istream&	seekg(streampos p) ;
	istream&	seekg(streamoff o, ios::seek_dir d) ;
   	streampos	tellg() ;
	istream&	operator>> (istream& (*f)(istream&))
			{	return (*f)(*this) ; }
	istream&	operator>> (ios& (*f)(ios&) ) ;
	istream&	operator>>(char*);
	istream&	operator>>(unsigned char*);
	istream&	operator>>(unsigned char& c)
			{
				istream* retval;
				if ( !ispecial && bp->optim_in_avail() ) {
					c = bp->optim_sbumpc() ;
					retval = this;
				}
				else {
					retval = &(rs_complicated(c));
				}
				return *retval;
			}
	istream&	operator>>(char& c)
			{
				istream* retval;
				if ( !ispecial && bp->optim_in_avail() ) {
					c = bp->optim_sbumpc() ;
					retval = this;
				}
				else {
					retval = &(rs_complicated(c));
				}
				return *retval;
			}
	istream&	rs_complicated(unsigned char& c);
	istream&	rs_complicated(char& c);
	istream&	operator>>(short&);
	istream&	operator>>(int&);
	istream&	operator>>(long&);
	istream&	operator>>(unsigned short&);
	istream&	operator>>(unsigned int&);
	istream&	operator>>(unsigned long&);
	istream&	operator>>(float&);
	istream&	operator>>(double&);
	istream&	operator>>(streambuf*);
	istream&	get(char* , int lim, char delim='\n');
	istream&	get(unsigned char* b,int lim, char delim='\n') {
				return get((char*)b, lim, delim);
			}
	istream&	getline(char* b, int lim, char delim='\n');
	istream&	getline(unsigned char* b, int lim, char delim='\n');
	istream&	get(streambuf& sb, char delim ='\n');
	istream&	get_complicated(unsigned char& c);
	istream&	get_complicated(char& c);
	istream&	get(unsigned char& c)
			{
				istream* retval;
				if ( !(ispecial & ~skipping) && bp->optim_in_avail() ) {
					x_gcount = 1 ;
					c = bp->sbumpc() ;
					retval = this;
				} else {
					retval = &(get_complicated(c));
				}
				return *retval;
			}
	istream&	get(char& c)
			{
				istream* retval;
				if ( !(ispecial & ~skipping) && bp->optim_in_avail() ) {
					x_gcount = 1 ;
					c = bp->sbumpc() ;
					retval = this;
				} else {
					retval = &(get_complicated(c));
				}
				return *retval;
			}
	int 		get()
			{
				int retval;
				if ( !ipfx(1) ) retval = EOF ;
				else {
					retval = bp->sbumpc() ;
					if ( retval == EOF ) setstate(eofbit) ;
					}
				return retval;
			}
	int		peek()
			{
				int retval;
				if ( ipfx(-1) ) retval = bp->sgetc() ;
				else		retval = EOF ;
				return retval;
			}
	istream&	ignore(int n=1,int delim=EOF) ;
	istream&	read(char*  s,int n);
	istream&	read(unsigned char* s,int n)
			{
				return read((char*)s,n) ;
			}
	int		gcount() ;
	istream&	putback(char c);
	int		sync()	{ return bp->sync() ; }
protected:
	int		do_ipfx(int noskipws) ;
	void		eatwhite() ;
			istream() ;
private:
	int		x_gcount ;
	void 		xget(char*  c) ;
public: /*** Obsolete constructors, carried over from stream package ***/
			istream(streambuf*, int sk, ostream* t=0) ;
				/* obsolete, set sk and tie
				 * via format state variables */
			istream(int size ,char*,int sk=1) ;
				/* obsolete, use strstream */
			istream(int fd,int sk=1, ostream* t=0) ;
				/* obsolete use fstream */
};

class ostream : virtual public ios {
public: /* Constructor */
			ostream(streambuf*) ;
	virtual		~ostream();
public:
	int		opfx()	/* Output prefix */
			{
				int retval;
				if ( ospecial )	retval = do_opfx() ;
				else		retval = 1 ;
				return retval;
			}
	void		osfx()
			{	if ( osfx_special ) do_osfx() ; }

	ostream&	flush() ;
	ostream&	seekp(streampos p) ;
	ostream&	seekp(streamoff o, ios::seek_dir d) ;
 	streampos	tellp() ;
	ostream&	put(char c)
	{
		ostream* retval;
		if (ospecial || osfx_special) {
			retval = &complicated_put(c);
		}
		else {
			if (  bp->sputc(c) == EOF )  {
				setstate(eofbit|failbit) ;
			}
			retval = this ;
		}
		return *retval;
	}
	ostream&	complicated_put(char c);
	ostream&	operator<<(char c)
	{
		ostream* retval;
		if (ospecial || osfx_special) {
			retval = &ls_complicated(c);
		}
		else {
			if (  bp->sputc(c) == EOF )  {
				setstate(eofbit|failbit) ;
			}
			retval = this ;
		}
		return *retval;
	}

	ostream&	operator<<(unsigned char c)
	{
		ostream* retval;
		if (ospecial || osfx_special) {
			retval = &ls_complicated(c);
		}
		else {
			if (  bp->sputc(c) == EOF )  {
				setstate(eofbit|failbit) ;
			}
			retval = this ;
		}
		return *retval;
	}
	ostream& 	ls_complicated(char);
	ostream& 	ls_complicated(unsigned char);

	ostream&	operator<<(const char*);
	ostream&	operator<<(int a);
	ostream&	operator<<(long);
	ostream&	operator<<(long long);
	ostream&	operator<<(double);
	ostream&	operator<<(float);
	ostream&	operator<<(unsigned int a);
	ostream&	operator<<(unsigned long);
	ostream&	operator<<(unsigned long long);
	ostream&	operator<<(void*);
/*	ostream&	operator<<(const void*);   add this later */
	ostream&	operator<<(streambuf*);
	ostream&	operator<<(short i) { return *this << (int)i ; }
	ostream&	operator<<(unsigned short i)
			{ return *this << (int)i  ; }

	ostream&	operator<< (ostream& (*f)(ostream&))
			{ return (*f)(*this) ; }
	ostream&	operator<< (ios& (*f)(ios&) ) ;

	ostream&	write(const char*  s,int n)
	{
		if ( !state ) {
			if ( bp->sputn(s,n) != n ) setstate(eofbit|failbit);
			}
		return *this ;
	}
	ostream&	write(const unsigned char* s, int n)
	{
		return write((const char*)s,n);
	}
protected: /* More ostream members */
	int		do_opfx() ;
	void		do_osfx() ;
			ostream() ;

public: /*** Obsolete constructors, carried over from stream package ***/
			ostream(int fd) ;
				/* obsolete use fstream */
			ostream(int size ,char*) ;
				/* obsolete, use strstream */
} ;

class iostream : public istream, public ostream {
public:
			iostream(streambuf*) ;
	virtual		~iostream() ;
protected:
			iostream() ;
	} ;

class istream_withassign : public istream {
public:
			istream_withassign() ;
	virtual		~istream_withassign() ;
	istream_withassign&	operator=(istream&) ;
	istream_withassign&	operator=(streambuf*) ;
} ;

class ostream_withassign : public ostream {
public:
			ostream_withassign() ;
	virtual		~ostream_withassign() ;
	ostream_withassign&	operator=(ostream&) ;
	ostream_withassign&	operator=(streambuf*) ;
} ;

class iostream_withassign : public iostream {
public:
			iostream_withassign() ;
	virtual		~iostream_withassign() ;
	iostream_withassign&	operator=(ios&) ;
	iostream_withassign&	operator=(streambuf*) ;
} ;

extern istream_withassign cin ;
extern ostream_withassign cout ;
extern ostream_withassign cerr ;
extern ostream_withassign clog ;

ios&		dec(ios&) ;
ostream&	endl(ostream& i) ;
ostream&	ends(ostream& i) ;
ostream&	flush(ostream&) ;
ios&		hex(ios&) ;
ios&		oct(ios&) ;
istream&	ws(istream&) ;

static class Iostream_init {
	static int	stdstatus ; /* see cstreams.c */
	static int	initcount ;
	friend class	ios ;
public:
	Iostream_init() ;
	~Iostream_init() ;
} iostream_init ;

#endif
