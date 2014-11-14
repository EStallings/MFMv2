/*                                              -*- mode:C++ -*-
  Element_Tower_Red.h Abstract Tower element for base class
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
  \file   Element_Tower_Red.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_TOWER_RED_H
#define ELEMENT_TOWER_RED_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Abstract_Element_Tower.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Element_Tower_Red : public Abstract_Element_Tower<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  public:

    static Element_Tower_Red<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Tower_Red()
      : Abstract_Element_Tower<CC>(MFM_UUID_FOR("Village", WAR_VERSION))
    {
      Element<CC>::SetAtomicSymbol("Vi");
      Element<CC>::SetName("Village");
    }

    virtual u32 LocalPhysicsColor(const T& atom, u32 selector) const
    {
      u32 demand = Abstract_Element_Tower<CC>::GetCurrentDemand(atom);
      switch(demand)
      {
        case 0:  return 0xff0000; break;
        case 1:  return 0xff4c4c; break;
        case 2:  return 0xe50000; break;
        case 3:  return 0xe54c4c; break;
        case 4:  return 0xcc0000; break;
        case 5:  return 0xcc4c4c; break;
        case 6:  return 0xb20000; break;
        case 7:  return 0xb24c4c; break;
        case 8:  return 0x990000; break;
        case 9:  return 0x994c4c; break;
        case 10: return 0x7f0000; break;
        case 11: return 0x7f4c4c; break;
        case 12: return 0x660000; break;
        case 13: return 0x664c4c; break;
        case 14: return 0x4c0000; break;
        case 15: return 0x4c4c4c; break;
        default: return 0xffffff; break;
      }
    }
    virtual u32 DefaultPhysicsColor() const
    {
      return 0xffff0b0b;
    }

    virtual const T& GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Tower<CC>::
      SetID(defaultAtom, rand() % (1<<Abstract_Element_Tower<CC>::ID_LEN));

      Abstract_Element_Tower<CC>::
      SetCurrentDemand(defaultAtom, rand() % (1<<Abstract_Element_Tower<CC>::CURRENT_DEMAND_LEN));

      return defaultAtom;
    }

    const typename CC::ATOM_TYPE& GetDefaultScout() const;
    const typename CC::ATOM_TYPE& GetDefaultColonist() const;
    virtual void Behavior(EventWindow<CC>& window) const;
    
    virtual const char* GetDescription() const
    {
      return "Red Tower element.";
    }
  };

  template <class CC>
  Element_Tower_Red<CC> Element_Tower_Red<CC>::THE_INSTANCE;

}

#include "Element_Tower_Red.tcc"

#endif /* ELEMENT_TOWER_RED_H */
