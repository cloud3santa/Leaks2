#include "stdafx.h"


using namespace Memory;

ScriptThread *(*GetActiveThread)( ) = nullptr;
HMODULE Hooking::_hmoduleDLL;
HANDLE mainFiber;
DWORD wakeAt;

std::vector< LPVOID > Hooking::m_hooks;
uint64_t * Hooking::m_frameCount;
fpIsDLCPresent Hooking::is_DLC_present;
static eGameState * m_gameState;
static tUPSIDEDOWN m_upsidedown;
static uint64_t m_worldPtr;
static BlipList * m_blipList;
static Hooking::NativeRegistrationNew ** m_registrationTable;
static std::unordered_map< uint64_t , Hooking::NativeHandler > m_handlerCache;
static std::vector< LPVOID > m_hookedNative;
static __int64 ** m_globalPtr;
HWND Hooking::hWindow;
const int EVENT_COUNT = 78;
static std::vector< void* > EventPtr;
static char EventRestore[EVENT_COUNT] = { };


/* Start Hooking */
void Hooking::Start( const HMODULE hmoduleDLL )
{
    _hmoduleDLL = hmoduleDLL;
    CLog::initialize( "moddersonly.club" , "moddersonly.log" );
    hWindow = nullptr;
    while ( hWindow == nullptr )
    {
        hWindow = FindWindow( L"grcWindow" , nullptr );
        Sleep( 100 );
    }
    FindPatterns( );
    if ( !InitializeHooks( ) ) Cleanup( );
}

BOOL Hooking::InitializeHooks( )
{
    auto returnVal = TRUE;

    if ( !iHook . Initialize( ) )
    {
        CLog::error( "Failed to initialize InputHook" );
        returnVal = FALSE;
    }

    if ( MH_Initialize( ) != MH_OK )
    {
        CLog::error( "MinHook failed to initialize" );
        returnVal = FALSE;
    }

    if ( !HookNatives( ) )
    {
        CLog::error( "Failed to initialize NativeHooks" );
        returnVal = FALSE;
    }
    return returnVal;
}

template < typename T >
bool Native( const DWORD64 hash, const LPVOID hookFunction, T ** trampoline )
{
    if ( *reinterpret_cast< LPVOID* >( trampoline ) != nullptr )
        return true;
    const auto originalFunction = Hooking::GetNativeHandler( hash );
    if ( originalFunction != nullptr )
    {
        const auto createHookStatus = MH_CreateHook( originalFunction , hookFunction ,
                                                     reinterpret_cast< LPVOID* >( trampoline ) );
        if ( ( ( createHookStatus == MH_OK ) || ( createHookStatus == MH_ERROR_ALREADY_CREATED ) ) && (
            MH_EnableHook( originalFunction ) == MH_OK ) )
        {
            m_hookedNative . push_back( static_cast< LPVOID >( originalFunction ) );
            return true;
        }
    }

    return false;
}

uint64_t CMetaData::m_begin = 0;
uint64_t CMetaData::m_end = 0;
DWORD CMetaData::m_size = 0;

uint64_t CMetaData::begin( )
{
    return m_begin;
}

uint64_t CMetaData::end( )
{
    return m_end;
}

DWORD CMetaData::size( )
{
    return m_size;
}

void CMetaData::init( )
{
    if ( m_begin && m_size )
        return;

    m_begin = reinterpret_cast< uint64_t >( GetModuleHandleA( nullptr ) );
    const auto * headerDos = reinterpret_cast< const IMAGE_DOS_HEADER* >( m_begin );
    const auto headerNt = reinterpret_cast< const IMAGE_NT_HEADERS64* >( reinterpret_cast< const BYTE* >( headerDos ) +
        headerDos -> e_lfanew );
    m_size = headerNt -> OptionalHeader . SizeOfCode;
    m_end = m_begin + m_size;
}

fpIsDLCPresent OG_IS_DLC_PRESENT = nullptr;

BOOL __cdecl HK_IS_DLC_PRESENT( )
{
    static uint64_t last = 0;
    const auto cur = *Hooking::m_frameCount;
    if ( last != cur )
    {
        last = cur;
        Hooking::onTickInit( );
    }
    return OG_IS_DLC_PRESENT( );
}


bool Hooking::HookNatives( )
{
    const auto status = MH_CreateHook( is_DLC_present , HK_IS_DLC_PRESENT ,
                                       reinterpret_cast< void** >( &OG_IS_DLC_PRESENT ) );
    if ( ( status != MH_OK && status != MH_ERROR_ALREADY_CREATED ) || MH_EnableHook( is_DLC_present ) != MH_OK )
        return false;
    m_hooks . push_back( is_DLC_present );
    return true;
}

void __stdcall ScriptFunction( LPVOID lpParameter )
{
    try
    {
        ScriptMain( );
    }
    catch ( ... )
    {
        CLog::fatal( "Failed scriptFiber" );
    }
}

void Hooking::onTickInit( )
{
    if ( mainFiber == nullptr )
        mainFiber = ConvertThreadToFiber( nullptr );

    if ( timeGetTime( ) < wakeAt )
        return;

    static HANDLE scriptFiber;
    if ( scriptFiber )
        SwitchToFiber( scriptFiber );
    else
        scriptFiber = CreateFiber( NULL , ScriptFunction , nullptr );
}

void Hooking::FailPatterns( const char * name )
{
    char buf[4096];
    sprintf_s( buf , "finding %s" , name );
    CLog::error( buf );
    Cleanup( );
}

/*
//CPatternResult
*/

CPatternResult::CPatternResult( void * pVoid ) :
    m_pVoid( pVoid )
{
}

CPatternResult::CPatternResult( void * pVoid, void * pBegin, void * pEnd ) :
    m_pVoid( pVoid ),
    m_pBegin( pBegin ),
    m_pEnd( pEnd )
{
}

CPatternResult::~CPatternResult( ) = default;

/*
//CPattern Public
*/

CPattern::CPattern( char * szByte, char * szMask ) :
    m_szByte( szByte ),
    m_szMask( szMask ),
    m_bSet( false )
{
}

CPattern::~CPattern( ) = default;

CPattern & CPattern::find( const int i, const uint64_t startAddress )
{
    match( i , startAddress , false );
    if ( int( m_result . size( ) ) <= i )
        m_result . push_back( nullptr );
    return *this;
}

CPattern & CPattern::virtual_find( const int i, const uint64_t startAddress )
{
    match( i , startAddress , true );
    if ( int( m_result . size( ) ) <= i )
        m_result . push_back( nullptr );
    return *this;
}

CPatternResult CPattern::get( const int i )
{
    if ( int( m_result . size( ) ) > i )
        return m_result[i];
    return nullptr;
}

/*
//CPattern Private
*/
bool CPattern::match( const int i, const uint64_t startAddress, const bool virt )
{
    if ( m_bSet )
        return false;
    uint64_t begin = 0;
    uint64_t end = 0;
    if ( !virt )
    {
        CMetaData::init( );
        begin = CMetaData::begin( ) + startAddress;
        end = CMetaData::end( );
        if ( begin >= end )
            return false;
    }
    auto j = 0;
    do
    {
        if ( virt )
            begin = virtual_find_pattern( reinterpret_cast< BYTE* >( m_szByte ) , m_szMask ) + 1;
        else
            begin = find_pattern( begin , end , reinterpret_cast< BYTE* >( m_szByte ) , m_szMask ) + 1;
        if ( begin == NULL )
            break;
        j++;
    }
    while ( j < i );

    m_bSet = true;
    return true;
}

bool CPattern::byte_compare( const BYTE * pData, const BYTE * btMask, const char * szMask )
{
    for ( ; *szMask ; ++szMask, ++pData, ++btMask )
        if ( *szMask == 'x' && *pData != *btMask )
            break;
    return ( *szMask ) == 0;
}


uint64_t CPattern::find_pattern( const uint64_t address, const uint64_t end, BYTE * btMask, char * szMask )
{
    const auto len = strlen( szMask ) + 1;
    for ( uint64_t i = 0 ; i < ( end - address - len ) ; i++ )
    {
        const auto ptr = reinterpret_cast< BYTE* >( address + i );
        if ( byte_compare( ptr , btMask , szMask ) )
        {
            m_result . push_back( CPatternResult( reinterpret_cast< void* >( address + i ) ) );
            return address + i;
        }
    }
    return NULL;
}

uint64_t CPattern::virtual_find_pattern( BYTE * btMask, char * szMask )
{
    MEMORY_BASIC_INFORMATION mbi;
    char * pStart = nullptr;
    char * pEnd = nullptr;
    char * res = nullptr;
    const auto maskLen = strlen( szMask );

    while ( res == nullptr && sizeof( mbi ) == VirtualQuery( pEnd , &mbi , sizeof( mbi ) ) )
    {
        pStart = pEnd;
        pEnd += mbi . RegionSize;
        if ( mbi . Protect != PAGE_READWRITE || mbi . State != MEM_COMMIT )
            continue;

        for ( auto i = 0 ; pStart < pEnd - maskLen && res == nullptr ; ++pStart )
        {
            if ( byte_compare( reinterpret_cast< BYTE* >( pStart ) , btMask , szMask ) )
            {
                m_result . push_back( CPatternResult( static_cast< void* >( pStart ) , mbi . BaseAddress , pEnd ) );
                res = pStart;
            }
        }

        mbi = { };
    }
    return reinterpret_cast< uint64_t >( res );
}

void failPat( const char * name )
{
    CLog::fatal( "Failed to find %s pattern." , name );
    exit( 0 );
}

template < typename T >
void setPat
(
    const char * name,
    char * pat,
    char * mask,
    T ** out,
    const bool rel,
    const int offset = 0,
    int deref = 0,
    const int skip = 0
)
{
    T * ptr = nullptr;

    CPattern pattern( pat , mask );
    pattern . find( 1 + skip );
    if ( rel )
        ptr = pattern . get( skip ) . get_rel< T >( offset );
    else
        ptr = pattern . get( skip ) . get< T >( offset );

    while ( true )
    {
        if ( ptr == nullptr )
            failPat( name );

        if ( deref <= 0 )
            break;
        ptr = *reinterpret_cast< T** >( ptr );
        --deref;
    }

    *out = ptr;
}

template < typename T >
void setFn
(
    const char * name,
    char * pat,
    char * mask,
    T * out,
    const int skip = 0
)
{
    char * ptr = nullptr;

    CPattern pattern( pat , mask );
    pattern . find( 1 + skip );
    ptr = pattern . get( skip ) . get< char >( 0 );

    if ( ptr == nullptr )
        failPat( name );

    *out = T( ptr );
}

void Hooking::FindPatterns( )
{
    HANDLE steam = GetModuleHandleA( "steam_api64.dll" );

    auto p_activeThread = pattern( "E8 ? ? ? ? 48 8B 88 10 01 00 00" );
    auto p_blipList = pattern( "4C 8D 05 ? ? ? ? 0F B7 C1" );
    auto p_fixVector3Result = pattern( "83 79 18 00 48 8B D1 74 4A FF 4A 18" );
    auto p_gameLegals = pattern( "72 1F E8 ? ? ? ? 8B 0D" );
    auto p_gameLogos = pattern( "70 6C 61 74 66 6F 72 6D 3A" );
    auto p_gameState = pattern( "83 3D ? ? ? ? ? 8A D9 74 0A" );
    auto p_modelCheck = pattern( "48 85 C0 0F 84 ? ? ? ? 8B 48 50" );
    auto p_modelSpawn = pattern( "48 8B C8 FF 52 30 84 C0 74 05 48" );
    auto p_nativeTable = pattern( "76 32 48 8B 53 40 48 8D 0D" );
    auto p_worldPtr = pattern( "48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07" );
    auto p_globalPtr = pattern( "4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11" );
    auto p_eventHook = pattern( "48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03" );
    auto p_chatnameaddr = pattern("48 8B 05 ? ? ? ? C3 8A D1");
    auto p_is_upsidown = pattern("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 19 4C 8B ? 8B 15 ? ? ? ? 48 8B C8 41 FF 50 08 84 C0 74 05 48 8B C3 EB 02 33 C0 48 83 C4 20 5B C3 48 89 5C 24 ? 48 89 6C 24 ? 56 57");
    setPat< uint64_t >( "frame count" ,
                        "\x8B\x15\x00\x00\x00\x00\x41\xFF\xCF" ,
                        "xx????xxx" ,
                        &Hooking::m_frameCount ,
                        true ,
                        2 );

    setFn< fpIsDLCPresent >( "is_DLC_present" ,
                             "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x81\xF9\x00\x00\x00\x00" ,
                             "xxxx?xxxxxxx????" ,
                             &Hooking::is_DLC_present );

    char * c_location = nullptr;
    void * v_location = nullptr;

    Sleep( 5000 );

    CLog::msg( "Getting Game State..." );
    c_location = p_gameState . count( 1 ) . get( 0 ) . get< char >( 2 );
    c_location == nullptr
        ? FailPatterns( "gameState" )
        : m_gameState = reinterpret_cast< decltype(m_gameState) >( c_location + *reinterpret_cast< int32_t* >(
            c_location ) + 5 );
    CLog::msg("Getting Game State...");
    c_location = p_gameState.count(1).get(0).get< char >(2);
    c_location == nullptr
        ? FailPatterns("gameState")
        : m_gameState = reinterpret_cast< decltype(m_gameState) >(c_location + *reinterpret_cast< int32_t* >(
            c_location) + 5);
    CLog::msg( "Getting vector3 result fixer func..." );
    v_location = p_fixVector3Result . count( 1 ) . get( 0 ) . get< void >( 0 );
    if ( v_location != nullptr )
        scrNativeCallContext::SetVectorResults = static_cast< void(*)( scrNativeCallContext * )
        >( v_location );

    CLog::msg( "Getting native registration table..." );
    c_location = p_nativeTable . count( 1 ) . get( 0 ) . get< char >( 9 );
    c_location == nullptr
        ? FailPatterns( "native registration table" )
        : m_registrationTable = reinterpret_cast< decltype(m_registrationTable) >( c_location + *reinterpret_cast<
            int32_t* >( c_location ) + 4 );

    CLog::msg( "Getting World Pointer..." );
    c_location = p_worldPtr . count( 1 ) . get( 0 ) . get< char >( 0 );
    c_location == nullptr
        ? FailPatterns( "world Pointer" )
        : m_worldPtr = reinterpret_cast< uint64_t >( c_location ) + *reinterpret_cast< int* >( reinterpret_cast<
            uint64_t >( c_location ) + 3 ) + 7;

    CLog::msg( "Getting Blip List..." );
    c_location = p_blipList . count( 1 ) . get( 0 ) . get< char >( 0 );
    c_location == nullptr
        ? FailPatterns( "blip List" )
        : m_blipList = reinterpret_cast< BlipList* >( c_location + *reinterpret_cast< int* >( c_location + 3 ) + 7 );


    CLog::msg( "Bypassing Object restrictions.." );

    // Bypass online model requests block
    nop( p_modelCheck . count( 1 ) . get( 0 ) . get< void >( 0 ) , 24 );
    // Bypass is player model allowed to spawn checks
    nop( p_modelSpawn . count( 1 ) . get( 0 ) . get< void >( 8 ) , 2 );

    CLog::msg( "Getting active script thread..." );
    c_location = p_activeThread . count( 1 ) . get( 0 ) . get< char >( 1 );
    c_location == nullptr
        ? FailPatterns( "Active Script Thread" )
        : GetActiveThread = reinterpret_cast< decltype(GetActiveThread) >( c_location + *reinterpret_cast< int32_t* >(
            c_location ) + 4 );

    CLog::msg( "Getting World Pointer..." );
    c_location = p_globalPtr . count( 1 ) . get( 0 ) . get< char >( 0 );
    __int64 patternAddr = NULL;
    c_location == nullptr
        ? FailPatterns( "globalTable" )
        : patternAddr = reinterpret_cast< decltype(patternAddr) >( c_location );
    m_globalPtr = reinterpret_cast< __int64** >( patternAddr + *reinterpret_cast< int* >( patternAddr + 3 ) + 7 );

    CLog::msg( "Getting Event Hook..." );
    if ( ( c_location = p_eventHook . count( 1 ) . get( 0 ) . get< char >( 0 ) ) )
    {
        auto i = 0, j = 0, matches = 0, found = 0;
        char * pattern = "\x4C\x8D\x05";
        while ( found != EVENT_COUNT )
        {
            if ( c_location[i] == pattern[j] )
            {
                if ( ++matches == 3 )
                {
                    EventPtr . push_back(
                        reinterpret_cast< void* >( reinterpret_cast< uint64_t >( c_location + i - j ) + *
                            reinterpret_cast< int* >(
                                c_location + i + 1 ) + 7 ) );
                    found++;
                    j = matches = 0;
                }
                j++;
            }
            else
            {
                matches = j = 0;
            }
            i++;
        }
    }
    CLog::msg( "Initializing natives..." );
    CrossMapping::initNativeMap( );

    CLog::msg( "Checking if GTA V is ready..." );
    while ( *m_gameState != GameStatePlaying )
    {
        Sleep( 200 );
    }
    CLog::msg( "GTA V ready!" );
}

static Hooking::NativeHandler _Handler( const uint64_t origHash )
{
    const auto newHash = CrossMapping::MapNative( origHash );
    if ( newHash == 0 )
    {
        return nullptr;
    }

    auto table = m_registrationTable[newHash & 0xFF];

    for ( ; table ; table = table -> getNextRegistration( ) )
    {
        for ( uint32_t i = 0 ; i < table -> getNumEntries( ) ; i++ )
        {
            if ( newHash == table -> getHash( i ) )
            {
                return table -> handlers[i];
            }
        }
    }
    return nullptr;
}

Hooking::NativeHandler Hooking::GetNativeHandler( const uint64_t origHash )
{
    auto & handler = m_handlerCache[origHash];

    if ( handler == nullptr )
    {
        handler = _Handler( origHash );
    }

    return handler;
}

eGameState Hooking::GetGameState( )
{
    return *m_gameState;
}

uint64_t Hooking::getWorldPtr( )
{
    return m_worldPtr;
}

void WAIT( const DWORD ms )
{
    wakeAt = timeGetTime( ) + ms;
    SwitchToFiber( mainFiber );
}

void Hooking::Cleanup( )
{
    CLog::msg( "Cleaning up hooks" );
    auto b = ( MH_DisableHook( &ResetWriteWatch ) == MH_OK && MH_RemoveHook( &ResetWriteWatch ) == MH_OK );
    for ( auto & m_hook : m_hooks )
        b = ( MH_DisableHook( m_hook ) != MH_OK && MH_RemoveHook( m_hook ) != MH_OK );
    MH_Uninitialize( );

    FreeLibraryAndExitThread( static_cast< HMODULE >( _hmoduleDLL ) , 1 );
}

void Hooking::defuseEvent( const RockstarEvent e, const bool toggle )
{
    static const unsigned char retn = 0xC3;
    const auto p = static_cast< char* >( EventPtr[e] );
    if ( toggle )
    {
        if ( EventRestore[e] == 0 )
            EventRestore[e] = p[0];
        *p = retn;
    }
    else
    {
        if ( EventRestore[e] != 0 )
            *p = EventRestore[e];
    }
}

__int64 ** Hooking::getGlobalPtr( )
{
    return m_globalPtr;
}
