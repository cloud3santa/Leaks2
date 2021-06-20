#pragma once
#include "stdafx.h"
#include "natives_lower.h"
#include "Vehicle.h"
#include "keyboard.h"


class CPlayer
{
public:
    CPlayer( ) = default;
    CPlayer( const CPlayer & obj ) = default;
    CPlayer & operator=( const CPlayer & obj ) = default;
    CPlayer( CPlayer && obj ) = default;
    CPlayer & operator=( CPlayer && obj ) = default;
    virtual ~CPlayer( ) = default;
    virtual Ped ped( ) const = 0;
    virtual Player player( ) const = 0;
    virtual void update_info( ) = 0;
};


class CSelfPlayer : public CPlayer
{
    Ped ped_;
    Player player_;
    bool in_vehicle_;
    CVehicle* vehicle_;
    Vector3 coords_ { };
public:
    explicit CSelfPlayer( const Player player )
        :
        player_( player )
    {
        ped_ = player::player_ped_id( );
        in_vehicle_ = ped::is_ped_in_any_vehicle(ped_, false);
        vehicle_ = new CVehicle(ped::get_vehicle_ped_is_in(ped_, true));
        coords_ = entity::get_entity_coords(ped_, true);
    }

    CSelfPlayer( const CSelfPlayer & obj ) = default;
    CSelfPlayer & operator=( const CSelfPlayer & obj ) = default;
    CSelfPlayer( CSelfPlayer && obj ) = default;
    CSelfPlayer & operator=( CSelfPlayer && obj ) = default;
    ~CSelfPlayer( ) = default;
    Ped ped( ) const override { return ped_; }
    Player player( ) const override { return player_; }
    Vector3 coords() const { return coords_; };
    void update_info( ) override
    {
        ped_ = player::player_ped_id( );
        player_ = player::player_id( );
        in_vehicle_ = ped::is_ped_in_any_vehicle(ped_, false);
        vehicle_ = new CVehicle(ped::get_vehicle_ped_is_in(ped_, true));
        coords_ = entity::get_entity_coords(ped_, true);
    }
    bool in_vehicle() const { return in_vehicle_; };
    CVehicle* vehicle() const { return vehicle_; };
    void apply_skin( const Hash model )
    {
        if (!streaming::has_model_loaded(model))
            streaming::request_model(model);
        while (!streaming::has_model_loaded(model)) {
            WAIT(0);
        }

        player::set_player_model(&player_, model);
        streaming::set_model_as_no_longer_needed(model);
    }
    void fly( const bool set) const
    {
        if (set) {
            const auto leftStickY = controls::get_control_normal(2, ControlFrontendAxisY);

            weapon::give_delayed_weapon_to_ped(ped_, $("GADGET_PARACHUTE"), 1, true);

            //Going upwards
            if ((GetAsyncKeyState(VK_KEY_S) || leftStickY < -0.1) && ped::is_ped_in_parachute_free_fall(ped_))
                entity::apply_force_to_entity(ped_, 1, 0.f, 100.f, 50.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            //Downwards
            else if ((GetAsyncKeyState(VK_KEY_W) || leftStickY > 0.1) && ped::is_ped_in_parachute_free_fall(ped_))
                entity::apply_force_to_entity(ped_, 1, 0.f, 100.f, -10.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            //Neutral
            else if (ped::is_ped_in_parachute_free_fall(ped_))
                entity::apply_force_to_entity(ped_, 1, 0.f, 100.f, 1.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            //Getting Back In The Air
            else if ((GetAsyncKeyState(VK_SPACE) || (controls::is_control_pressed(0, ControlFrontendX) || controls::is_disabled_control_pressed(0, ControlFrontendX))) && !ped::is_ped_in_parachute_free_fall(ped_))
                entity::apply_force_to_entity(ped_, 1, 0.f, 0.f, 150.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
        }
    }
};
class COnlinePlayer : public CPlayer
{
    Ped ped_;
    Player player_;
    bool is_player_;
    std::string name_;
public:
    explicit COnlinePlayer(const Player player)
        :
        player_( player )
    {
        
        ped_ = player::get_player_ped_script_index(player_);
        is_player_ = entity::does_entity_exist(ped_);
        name_ = player::get_player_name(player_);
    }

    COnlinePlayer(const COnlinePlayer & obj) = default;
    COnlinePlayer & operator=(const COnlinePlayer & obj) = default;
    COnlinePlayer(COnlinePlayer && obj) = default;
    COnlinePlayer & operator=(COnlinePlayer && obj) = default;
    ~COnlinePlayer() = default;
    Ped ped() const override { return ped_; }
    Player player() const override { return player_; }
    bool is_online() const { return is_player_; }
    std::string name() const { return name_; }
    void update_info() override
    {
        
        ped_ = player::get_player_ped_script_index(player_);
        name_ = player::get_player_name(ped_);
        is_player_ = entity::does_entity_exist(ped_);
    }
};


namespace players
{
    std::unique_ptr< CSelfPlayer > SelfPlayer = std::make_unique< CSelfPlayer >( player::player_id( ) );
   std::vector< std::unique_ptr< COnlinePlayer >> OnlinePlayer;
}

