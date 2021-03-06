/*                                              -*- mode:C++ -*-
  Element_Breadcrumb_Red.h An element that acts similarly to a Wa-Tor fish
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file Element_Breadcrumb_Red.h An element that acts similarly to a Wa-Tor fish
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_BREADCRUMB_RED_H
#define ELEMENT_BREADCRUMB_RED_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "Atom.h"
#include "Abstract_Element_Breadcrumb.h"

namespace MFM
{

  template <class CC>
  class Element_Breadcrumb_Red : public Abstract_Element_Breadcrumb<CC>
  {
    // Short names for params
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum
    {
      R = P::EVENT_WINDOW_RADIUS
    };

   public:

    static Element_Breadcrumb_Red THE_INSTANCE;
    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Breadcrumb<CC>::
      SetActive(defaultAtom, (1<<Abstract_Element_Breadcrumb<CC>::ACTIVE_TIMER_LEN)-1);

      return defaultAtom;
    }

    Element_Breadcrumb_Red() :
      Abstract_Element_Breadcrumb<CC>(MFM_UUID_FOR("Trail", 1))
    {
      Element<CC>::SetAtomicSymbol("Tr");
      Element<CC>::SetName("Trail");
    }

    virtual u32 LocalPhysicsColor(const T& atom, u32 selector) const
    {
      if(Abstract_Element_Breadcrumb<CC>::IsActive(atom))
      {
	      return 0x333333;
      }
      else if(Abstract_Element_Breadcrumb<CC>::GetIsEndpoint(atom))
      {
        if(Abstract_Element_Breadcrumb<CC>::GetEndpointSwitch(atom))
          return 0x330033;
        else
          return 0x003333;
      }
      else if(Abstract_Element_Breadcrumb<CC>::GetTrafficDir(atom))
        return 0x996666;
      else
        return DefaultPhysicsColor();
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0x666699;
    }

    // Non-diffusable
    virtual u32 Diffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return nowAt.Equals(maybeAt)?Element<CC>::COMPLETE_DIFFUSABILITY:0;
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual void Behavior(EventWindow<CC>& window) const;
  };

  template <class CC>
  Element_Breadcrumb_Red<CC> Element_Breadcrumb_Red<CC>::THE_INSTANCE;

}
#include "Element_Breadcrumb_Red.tcc"
#endif /* ELEMENT_BREADCRUMB_RED_H */
