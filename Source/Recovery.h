#pragma once
#include "stdafx.h"
class GTAMoney
{
    int amount_;
    int tick_;
    int delay_;
public:
    explicit GTAMoney(const int amount)
        : amount_(amount)
    {
        tick_ = GetTickCount();
        delay_ = 100;
    }
    bool addToBank()
    {
        

        if (int(GetTickCount()) - tick_ >= delay_)
        {
            auto ID = gameplay::get_random_int_in_range(1000000000, 2000000000);
            unk3::_network_shop_begin_service(&ID, 1474183246, 312105838, 1445302971, amount_, 4);
            unk3::_network_shop_checkout_start(ID);
            tick_ = GetTickCount();
        }
        return true;
    }
    bool addToWallet()
    {
        if (int(GetTickCount()) - tick_ >= delay_)
        {
            auto ID = gameplay::get_random_int_in_range(1000000000, 2000000000);
            unk3::_network_shop_begin_service(&ID, 1474183246, 312105838, 1445302971, amount_, 1);
            unk3::_network_shop_checkout_start(ID);
            tick_ = GetTickCount();
        }
        return true;
    }
    void setAmount(const int amount)
    {
        amount_ = amount;
    }
    int getAmount() const { return amount_; }

    void setdelay(const int delay)
    {
        delay_ = delay;
    }
    int getdelay() const { return delay_; }
};




std::unique_ptr<GTAMoney> stealth = std::make_unique<GTAMoney>(9999999);