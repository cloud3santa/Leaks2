#ifndef LOG_H
#define LOG_H
#ifdef __DEBUG
#define DEBUGOUT( X, ... ) Log::Debug( X, __VA_ARGS__ )
#else
#define DEBUGOUT( X, ... )
#endif

#if _DEBUG
#define DEBUGMSG Log::Debug
#else
#define DEBUGMSG //
#endif

typedef std::vector< std::string > vec_str;

class CLog
{
public:
    static vec_str m_msg;
    static vec_str m_error;
    static vec_str m_fatal;

    static bool initialize( std::string szDir, std::string szFile );
    static void msg( char * msg, ... );
    static void error( char * error, ... );
    static void fatal( char * error, ... );

protected:
    static std::string m_szFile;
    static std::string log( char *, char *, va_list vars );
};
#endif // LOG_H
