/* -*- C++ -*- */

#include "Element_Scout_Blue.h"
#include "Element_Colonist_Blue.h"
#include "Element_Soldier_Blue.h"

namespace MFM
{
  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Blue<CC>::GetDefaultScout() const
  {
    return Element_Scout_Blue<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Blue<CC>::GetDefaultColonist() const
  {
    return Element_Colonist_Blue<CC>::THE_INSTANCE.GetDefaultAtom();
  }

  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Blue<CC>::GetDefaultSoldier() const
  {
    return Element_Soldier_Blue<CC>::THE_INSTANCE.GetDefaultAtom();
  }
}
