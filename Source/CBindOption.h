#pragma once
#include "COption.h"
#include "lists.h"
#include "keyboard.h"

class CBindOption : public COption
{
    std::string left_;
    std::string desc_;
    std::function< void() > func_;
    int * key_;

public:
    CBindOption(std::string left, int * key, std::string desc = "", std::function< void() > func = []
    {
    })
        :
        left_(std::move(left)),
        desc_(std::move(desc)),
        func_(std::move(func)),
        key_(key)
    {
        
    }

    CBindOption(const CBindOption & obj) = default;
    CBindOption & operator=(const CBindOption & obj) = default;
    CBindOption(CBindOption && obj) = default;
    CBindOption & operator=(CBindOption && obj) = default;
    ~CBindOption() = default;

    std::string left_text() const override
    {
        return left_;
    }

    std::string right_text() const override
    {
        return *key_ == 1337 ?  " < Press A Key  >" :  "< " + std::string(lists::KeyStrings[*key_]) + " >";
    }

    std::string description() const override
    {
        return desc_;
    }

    sprite_info sprite() const override
    {
        return { "", "", 0, 0, 0, 0 };
    }

    void action() override
    {
        if(*key_ == 1337)
        {
            for ( auto i = 0; i < 255; i++)
            {
                if (KeyJustUp(i))
                {
                    *key_ = i;
                    break;
                }
            }
        }
        if (KeyJustUp(keys_map_[k_press].vk_key)) {
            *key_ = 1337;
            func_();
        }
    }
    std::string get_type() const override
    {
        return ("CBindOption");
    }
};
