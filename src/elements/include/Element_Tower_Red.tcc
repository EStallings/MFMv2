/* -*- C++ -*- */

#include "Element_Scout_Red.h"
#include "Element_Colonist_Red.h"
#include "Element_Breadcrumb_Red.h"
#include "Element_Empty.h"
#include "Element_Res.h"

namespace MFM
{
  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultScout() const
  {
    return Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultColonist() const
  {
    return Element_Colonist_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  void Element_Tower_Red<CC>::Behavior(EventWindow<CC>& window) const
  {
    typedef typename CC::ATOM_TYPE T;

    const T& self = window.GetCenterAtom();
    Random& rand = window.GetRandom();
    u32 resDemand = GetCurrentDemand(self);
    MDist<R> md = MDist<R>::get();


    //Consume Res
    for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
    {
      SPoint pt = md.GetPoint(i);
      if(window.GetRelativeAtom(pt).GetType() == Element_Res<CC>::THE_INSTANCE.GetType())
      {
        if(rand.OneIn(11-resDemand)){
          window.SetRelativeAtom(pt, Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        }
      }
    }
    
    if(rand.OneIn(Abstract_Element_Tower<CC>::m_scoutSpawnChance.GetValue()))
    {
      const T& scout = Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
      //T mutableScout = Abstract_Element_Scout<CC>::GetMutableAtom(scout);
      PlaceAtomRandomly(window, scout);
    }

    if(rand.OneIn(Abstract_Element_Tower<CC>::m_resSpawnChance.GetValue()))
    {
      PlaceAtomRandomly(window, Element_Res<CC>::THE_INSTANCE.GetDefaultAtom());
    }

    //TODO Adjust supply and demand based on nearby breadcrumbs
    for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
    {
      SPoint pt = md.GetPoint(i);
      if(window.GetRelativeAtom(pt).GetType() == Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetType()){
        
      }
    }
    //disabled for now
    if(rand.OneIn(Abstract_Element_Tower<CC>::m_colonistSpawnChance.GetValue()))
    {
      //PlaceAtomRandomly(window, GetDefaultColonist());
    }

  }
}
