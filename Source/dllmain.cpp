// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

DWORD WINAPI ControlThread( LPVOID lpParam )
{
    Hooking::Start( static_cast< HMODULE >( lpParam ) );
    while ( !GetAsyncKeyState( VK_DELETE ) )
        Sleep( 0x60 );
    Hooking::Cleanup( );
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH :
        CreateThread( nullptr , NULL , ( LPTHREAD_START_ROUTINE ) ControlThread , hModule , NULL , nullptr );
        break;
    case DLL_THREAD_ATTACH :
        break;
    case DLL_THREAD_DETACH :
        break;
    case DLL_PROCESS_DETACH :
        Hooking::Cleanup( );
        break;
    }
    return TRUE;
}
