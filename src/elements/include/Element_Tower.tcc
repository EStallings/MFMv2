/* -*- C++ -*- */

#include "Element_Scout.h"
#include "Element_Colonist.h"
#include "Element_Soldier.h"

namespace MFM
{
  template <class CC>
  void Element_Tower<CC>::Behavior(EventWindow<CC>& window) const
  {
    typedef typename CC::ATOM_TYPE T;

    const T& self = window.GetCenterAtom();
    Random& rand = window.GetRandom();

    if(GetCurrentHealth(self) <= 0)
    {
      window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
    }

    //disabled for now
    if(rand.OneIn(m_soldierSpawnChance.GetValue()))
    {
      PlaceAtomRandomly(window, GetDefaultSoldier());
    }

    //disabled for now
    if(rand.OneIn(m_colonistSpawnChance.GetValue()))
    {
      //PlaceAtomRandomly(window, GetDefaultColonist());
    }

    if(rand.OneIn(m_scoutSpawnChance.GetValue()))
    {
      const T& scout = GetDefaultScout();
      T mutableScout = Element_Scout<CC>::GetMutableAtom(scout);
      Element_Scout<CC>::SetID(mutableScout, Element_Tower<CC>::GetID(self));
      PlaceAtomRandomly(window, mutableScout);
    }
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower<CC>::GetDefaultScout() const
  {
    return Element_Scout<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower<CC>::GetDefaultColonist() const
  {
    return Element_Colonist<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower<CC>::GetDefaultSoldier() const
  {
    return Element_Soldier<CC>::THE_INSTANCE.GetDefaultAtom();
  }
}
