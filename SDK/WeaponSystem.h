#pragma once

class CCSWeaponInfo;

class IWeaponSystem
{
public:
    virtual ~IWeaponSystem() = 0;
    virtual void pad04() = 0;
    virtual CCSWeaponInfo* GetWpnData(int weaponId) = 0;
};


