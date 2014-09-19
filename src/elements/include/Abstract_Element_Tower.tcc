/* -*- C++ -*- */

#include "Abstract_Element_Scout.h"

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

    if(rand.OneIn(m_soldierSpawnChance.GetValue()))
    {

    }

    if(rand.OneIn(m_colonistSpawnChance.GetValue()))
    {

    }

    if(rand.OneIn(m_scoutSpawnChance.GetValue()))
    {
      PlaceAtomRandomly(window, GetDefaultScout());
    }
  }
}
