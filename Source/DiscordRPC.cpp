#include "stdafx.h"
#include "DiscordRPC.h"
#include <discord_rpc.h>
#include "script.h"
#include "xorstring.hpp"
__int64 start = 0;

void DiscordRPC::updateDiscordPresence( std::string & menu)
{
    menu = "Tab: " + menu;
    start = time(nullptr);
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Modders Only - GTA V";
    discordPresence.details = menu.c_str(  );
    discordPresence.largeImageKey = "moddersonly";
    discordPresence.largeImageText = "A Premium Cheat Provider.";
    discordPresence.smallImageKey = "gta_small";
    discordPresence.smallImageText = "GTA V";
    discordPresence.instance = 1;
    discordPresence.startTimestamp = start;
    Discord_UpdatePresence(&discordPresence);
}


void DiscordRPC::discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(XSChar("427555974400835595"), &handlers, 1, nullptr);
}