#pragma once
#include "stdafx.h"
#include "keyboard.h"

class CVehicle
{
private:
    Vehicle vehicle_;

public:

    explicit CVehicle( const Vehicle veh)
    :
    vehicle_( veh )
    {
        
    }
    CVehicle(const CVehicle & obj) = default;
    CVehicle & operator=(const CVehicle & obj) = default;
    CVehicle(CVehicle && obj) = default;
    CVehicle & operator=(CVehicle && obj) = default;
    ~CVehicle() = default;
    void set_invincible(const bool set) const
    {
        rcoe(vehicle_);
        entity::set_entity_invincible(vehicle_, set);
        entity::set_entity_proofs(vehicle_, set, set, set, set, set, set, set, set);
    }
    void jump( const int bind, const bool toggle) const
    {
        if (toggle) {
            if (entity::get_entity_height_above_ground(vehicle_) < 1)
            {
                if (KeyJustUp(bind) || controls::is_control_pressed(2, ControlFrontendAccept))
                    entity::apply_force_to_entity_center_of_mass(vehicle_, 1, 0.f, 0.f, 10.f, TRUE, TRUE, TRUE, TRUE);
            }
        }
    }
    void fly( const bool set ) const
    {
        if (set) {
            const auto leftStickY = controls::get_control_normal(2, ControlFrontendAxisY);


            //Going upwards
            if ((GetAsyncKeyState(VK_KEY_S) || leftStickY < -0.1))
                entity::apply_force_to_entity(vehicle_, 1, 0.f, 100.f, 50.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            //Downwards
            else if ((GetAsyncKeyState(VK_KEY_W) || leftStickY > 0.1))
                entity::apply_force_to_entity(vehicle_, 1, 0.f, 100.f, -10.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            //Getting Back In The Air
            else if ((GetAsyncKeyState(VK_SPACE) || (controls::is_control_pressed(0, ControlFrontendX) || controls::is_disabled_control_pressed(0, ControlFrontendX))) )
                entity::apply_force_to_entity(vehicle_, 1, 0.f, 0.f, 150.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
            else
                entity::apply_force_to_entity(vehicle_, 1, 0.f, 100.f, 1.f, 0.f, 0.f, 0.f, 0.f, true, true, true, true, true);
        }
    }
    Vehicle vehicle() const { return vehicle_; };
};




