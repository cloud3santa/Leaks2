#include "stdafx.h"
#include "DiscordRPC.h"
#include "CMenuUI.h"
#include <memory>
#include "CRegOption.h"
#include "CSlideOption.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "CVectorOption.h"
#include "CSubOption.h"
#include "CBoolOption.h"
#include "bools.h"
#include "memory_arrays.h"
#include "Player.h"
#include "int.h"
#include "float.h"  // NOLINT
#include "lists.h"
#include "CBindOption.h"
#include "Recovery.h"
bool starting = true;
std::unique_ptr< CMenuUI > MainWindow = std::make_unique< CMenuUI >( );
void initialize( )
{
    const auto welcomemsg = XS("Welcome back, ") + std::string(player::get_player_name(player::player_id()));
    MainWindow -> setup_keys( );
    DiscordRPC::discordInit( );
    for (auto i = 0; i <= 32; ++i)
    {
        players::OnlinePlayer.push_back(std::make_unique<COnlinePlayer>(i));
    }
    draw_icon_notif(
        welcomemsg , XSChar("moddersonly") , // XSChar keeps it from being marooon? odd
        XS("~c~Authenticator") , XS("club") );
}

void feature_loop( )
{
    players::SelfPlayer -> update_info( );
    Memory::set_value< bool >( vars::mem_vectors::God_Mode , vars::player::bools::p_invincible );
    if ( vars::player::bools::p_never_wanted )
    {
        Memory::set_value< int >( vars::mem_vectors::Wanted_Level , 0 );
        vars::player::bools::p_always_wanted = false;
    }
    if ( vars::player::bools::p_always_wanted )
        Memory::set_value< int >( vars::mem_vectors::Wanted_Level , vars::player::ints::wanted_level );
    entity::set_entity_visible( player::player_ped_id( ) , !vars::player::bools::p_invisible ,
                                !vars::player::bools::p_invisible );
    if ( vars::player::bools::p_nragdoll )
    {
        Memory::set_value< bool >( vars::mem_vectors::No_Ragdoll , vars::player::bools::p_nragdoll );
        ped::set_ped_ragdoll_on_collision( players::SelfPlayer -> ped( ) , !vars::player::bools::p_nragdoll );
        ped::set_ped_can_ragdoll_from_player_impact( players::SelfPlayer -> ped( ) , !vars::player::bools::p_nragdoll );
        ped::set_ped_can_ragdoll( players::SelfPlayer -> ped( ) , !vars::player::bools::p_nragdoll );
    }
    Memory::set_value< float >( vars::mem_vectors::Sprint_Speed , vars::player::floats::p_runspeed );
    Memory::set_value< float >( vars::mem_vectors::Swim_Speed , vars::player::floats::p_swimspeed );

    if ( vars::player::bools::p_offradar )
    {
        globalHandle(2422736 + (1 + players::SelfPlayer->player(  ) * 420) + 215);
        globalHandle(2436251 + 69) = network::get_network_time();

    }
    if ( vars::player::bools::p_superjump )
        gameplay::set_super_jump_this_frame( players::SelfPlayer -> player( ) );
    if (players::SelfPlayer->in_vehicle()) {
        players::SelfPlayer->vehicle()->set_invincible(vars::vehicle::bools::v_invincible);
        players::SelfPlayer->vehicle()->jump(vars::vehicle::ints::v_jump_bind, vars::vehicle::bools::v_jump);
    }

    players::SelfPlayer->fly(vars::player::bools::p_hancock);
    for( auto & players : players::OnlinePlayer)
    {
        players->update_info();
    }
}
void timer_loop()
{
    static auto timer = GetTickCount();
    if(static_cast< int >(GetTickCount()) - timer >= 75)
    {
        if (vars::vehicle::bools::v_boost && KeyDown(vars::vehicle::ints::v_boost_bind) && players::SelfPlayer->in_vehicle()) {
            audio::set_vehicle_boost_active(players::SelfPlayer->vehicle()->vehicle(  ), true);
            vehicle::set_vehicle_forward_speed(players::SelfPlayer->vehicle()->vehicle(), vars::vehicle::ints::v_boost_speed);
        }
        else
            audio::set_vehicle_boost_active(players::SelfPlayer->vehicle()->vehicle(), false);
        timer = GetTickCount();;
    }
    static auto timer2 = GetTickCount();
    if (static_cast<int>(GetTickCount() - timer2) >= vars::recovery::ints::r_stealth_delay)
    {
        if (vars::recovery::bools::r_stealth)
            vars::recovery::bools::r_stealth_to_bank ? stealth->addToBank() : stealth->addToWallet();
        timer2 = GetTickCount();
    }
}

void main( )
{
    MainWindow -> check_keys( );

    if ( MainWindow -> opened( ) )
    {
        MainWindow -> add_menu( XS("player_options") , XS("Player") , XS("icon_user") , []
        {
            using namespace vars::player;


            MainWindow -> add_sub( XS("main") , XS("Main") , []
            {
                MainWindow -> push_option(
                    new CSubOption( XS("Model Changer") , XS("modelc") , "Goes to the model list." ) );
                MainWindow -> push_option( new CSlideOption< float >(
                    XS("Run Speed") , &floats::p_runspeed , 0.f , 2.f , 0.05f , XS("Adjusts running speed.") ) );
                MainWindow -> push_option( new CSlideOption< float >(
                    XS("Swim Speed") , &floats::p_swimspeed , 0.f , 2.f , 0.05f , XS("Adjusts swimming speed.") ) );
                MainWindow->push_option(new CSlideOption< int >(
                    XS("Opacity"), &ints::p_opacity, 0, 5, 1, XS("Adjusts player opacity."), [] {entity::set_entity_alpha(players::SelfPlayer->ped(), ints::p_opacity * 51, 0); }));
                MainWindow -> push_option(
                    new CBoolOption( XS("Invincibility") , &bools::p_invincible , XS("No damage can be inflicted.") ) );
                MainWindow -> push_option( new CBoolOption(
                    XS("Never Wanted") , &bools::p_never_wanted , XS("Never get the ~b~police ~w~on your tail.") ) );
                MainWindow -> push_option( new CBoolOption(
                    XS("Always Wanted") , &bools::p_always_wanted ,
                                       XS("Always have the ~b~police ~w~on your tail.") ) );
                if ( bools::p_always_wanted )
                    MainWindow -> push_option( new CSlideOption< int >(
                        XS("Always Wanted Level") , &ints::wanted_level , 1 , 5 , 1 ,
                                                 XS("Sets wanted level for Always Wanted.") ) );
                MainWindow -> push_option(
                    new CBoolOption( XS("Invisible") , &bools::p_invisible, XS("Your model turns invisble.") ) );
                MainWindow -> push_option( new CBoolOption(
                    XS("No Ragdoll") , &bools::p_nragdoll , XS("Your model cannot fall down.") , []
                    {
                        Memory::set_value< bool >( vars::mem_vectors::No_Ragdoll , bools::p_nragdoll );
                        ped::set_ped_ragdoll_on_collision( players::SelfPlayer -> ped( ) , !bools::p_nragdoll );
                        ped::set_ped_can_ragdoll_from_player_impact( players::SelfPlayer -> ped( ) ,
                                                                     !bools::p_nragdoll );
                        ped::set_ped_can_ragdoll( players::SelfPlayer -> ped( ) , !bools::p_nragdoll );
                    } ) );
                MainWindow -> push_option(
                    new CBoolOption( XS("Super Jump") , &bools::p_superjump , XS("Makes you jump abnormally high.") ) );
            MainWindow->push_option(new CBoolOption(XS("Hancock Mode"), &bools::p_hancock, XS("Allows you to fly.")));
                MainWindow -> push_option(
                    new CBoolOption( XS("Off Radar") , &bools::p_offradar , XS("People cannot see you on the map.") ) );
                MainWindow->push_option(new CRegOption(XS("Clone Model"), XS(""), XS("Makes a clone of you."), [] {ped::clone_ped(players::SelfPlayer->ped(), 0.f, true, true); }));

            } );


            MainWindow -> add_sub( XS("modelc") , XS("Model Changer") , []
            {
                MainWindow -> push_option(
                    new CSubOption( XS("Full List") , XS("fullmodelc") , XS("Goes to the full model list.") ) );
            } );


            MainWindow -> add_sub( XS("fullmodelc") , XS("Model Changer (Full List)") , []
            {
                for ( auto & player_model : lists::player_models )
                {
                    MainWindow -> push_option( new CRegOption( player_model , "" ,
                                                               XS("Changes player model to ") + std::string(
                                                                   player_model ) , [player_model]
                                                               {
                                                                   players::SelfPlayer -> apply_skin(
                                                                       $( player_model ) );
                                                               } ) );
                }
            } );
        } );
        MainWindow->add_menu(XS("PList"), XS("Players"), XS("icon_group"), []
        {
            MainWindow->add_sub(XS("main"), XS("Main"), []
            {
                for (auto & player_option : players::OnlinePlayer)
                {
                    if (player_option->is_online())
                        MainWindow -> push_option( new CSubOption(player_option-> name( ) ,
                                                                   XS("onlineplayeroptions") ,
                                                                   XS("Go to player options") , [&player_option]
                                                                   {
                                                                       selected_player = player_option->player();
                                                                   } ) );
                }
            });
            MainWindow->add_sub(XS("onlineplayeroptions"), players::OnlinePlayer[selected_player]->name(), []
            {
                MainWindow->push_option(new CRegOption(XS("Filler Option"), XS(""), XS("nothing.")));
            });
        });
        MainWindow -> add_menu( XS("vehicle") , XS("Vehicle") , XS("icon_vehicle") , []
        {
            using namespace vars::vehicle;
            MainWindow -> add_sub( XS("main") , XS("Main") , []
            {
                MainWindow -> push_option( new CBoolOption(
                    XS("Boost Bind") , &bools::v_boost ,
                    XS("Press ") + std::string(lists::KeyStrings[ints::v_boost_bind]) + XS(" to ~o~boost.") ) );
                MainWindow->push_option(new CSlideOption< int >(
                    XS("Boost Speed"), &ints::v_boost_speed, 0, 500, 5, XS("Adjusts boost speed.")));
                MainWindow->push_option(new CBoolOption(XS("Invincible"), &bools::v_invincible, XS("Makes vehicle ~c~invincible~w~.")));
                MainWindow->push_option(new CBoolOption(XS("Jump Bind"), &bools::v_jump, XS("Press ") + std::string(lists::KeyStrings[ints::v_jump_bind]) + XS(" to jump.")));
                MainWindow->push_option(new CBoolOption(XS("Hancock Mode"), &bools::v_hancock, XS("Allows you to fly your car.")));
            } );
        } );
        
        MainWindow -> add_menu( XS("Weap") , XS("Weapon") , XS("icon_smg") , []
        {
            MainWindow -> add_sub( XS("main") , XS("Main") , []
            {
            } );
        } );
        MainWindow -> add_menu( XS("Recovery") , XS("Recovery") , XS("icon_dollar") , []
        {
            using namespace vars::recovery;
            MainWindow -> add_sub( XS("main") , XS("Main") , []
            {
                MainWindow->push_option(new CSubOption(XS("Stealth"), XS("stealth"), XS("Goes to the stealth menu.")));
            } );
            MainWindow->add_sub(XS("stealth"), XS("Stealth"),[]
            {
                MainWindow->push_option(new CBoolOption(XS("Enabled"), &bools::r_stealth, XS("Enables stealth money.")));
                MainWindow->push_option(new CBoolOption(XS("To Bank"), &bools::r_stealth_to_bank, XS("Changes money transfer.")));
                MainWindow -> push_option( new CSlideOption< int >(
                    XS("Amount"), &ints::r_stealth_amount, 0, 10000000, 250000, XS("Adjusts stealth amount."), [] {stealth->setAmount(ints::r_stealth_amount); }));
                MainWindow->push_option(new CSlideOption< int >(
                    XS("Delay"), &ints::r_stealth_delay, 0, 9999, 5, XS("Adjusts stealth delay."), [] {stealth->setdelay(ints::r_stealth_delay); }));
            });
        } );
        
        MainWindow->add_menu(XS("Settings"), XS("Settings"), XS("icon_cogs"), []
        {
            MainWindow->add_sub(XS("main"), XS("Main"), []
            {
                MainWindow->push_option(new CSubOption(XS("Binds"), XS("binds"), XS("Goes to the Binds menu.")));
            });
            MainWindow->add_sub(XS("binds"), XS("Binds"), []
            {
                MainWindow->push_option(new CBindOption(XS("Vehicle Boost Bind"), &vars::vehicle::ints::v_boost_bind, XS("Choose a key to boost your vehicle.")));
                MainWindow->push_option(new CBindOption(XS("Vehicle Jump Bind"), &vars::vehicle::ints::v_jump_bind, XS("Choose a key to jump.")));
            });
        });
        
        MainWindow -> render( );
    }
    MainWindow -> navigate( );
}

void ScriptMain( )
{
    srand( GetTickCount( ) );
    if ( starting )
    {
        initialize( );
        starting = false;
    }
    while ( true )
    {
        main( );
        feature_loop( );
        timer_loop();
        // ReSharper disable once CppMsExtBindingRValueToLvalueReference
        DiscordRPC::updateDiscordPresence( MainWindow->current_tab(  ).uiname );
        WAIT( 0 );
    }
}

void set_menu_sub( const std::string & sub )
{
    MainWindow -> set_sub( sub );
}
