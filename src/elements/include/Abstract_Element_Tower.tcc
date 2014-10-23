/* -*- C++ -*- */

#include "Abstract_Element_Scout.h"
#include "Abstract_Element_Colonist.h"
#include "Abstract_Element_Soldier.h"

namespace MFM
{
  template <class CC>
  void Abstract_Element_Tower<CC>::Behavior(EventWindow<CC>& window) const
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
      //PlaceAtomRandomly(window, GetDefaultSoldier());
    }

    //disabled for now
    if(rand.OneIn(m_colonistSpawnChance.GetValue()))
    {
      //PlaceAtomRandomly(window, GetDefaultColonist());
    }

    if(rand.OneIn(m_scoutSpawnChance.GetValue()))
    {
      const T& scout = GetDefaultScout();
      //T mutableScout = Abstract_Element_Scout<CC>::GetMutableAtom(scout);
      PlaceAtomRandomly(window, scout);
    }
  }
}
