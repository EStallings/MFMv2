/* -*- C++ -*- */

#include "Element_Scout_Red.h"
#include "Element_Colonist_Red.h"
#include "Element_Soldier_Red.h"

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
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultSoldier() const
  {
    return Element_Soldier_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }
}
