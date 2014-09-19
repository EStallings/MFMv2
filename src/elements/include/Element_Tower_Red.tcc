/* -*- C++ -*- */

#include "Element_Scout_Red.h"

namespace MFM
{
  template <class CC>
  const typename CC::ATOM_TYPE& Element_Tower_Red<CC>::GetDefaultScout() const
  {
    return Element_Scout_Red<CC>::THE_INSTANCE.GetDefaultAtom();
  }
}
