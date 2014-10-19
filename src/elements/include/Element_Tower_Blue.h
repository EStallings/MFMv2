/*                                              -*- mode:C++ -*-
  Element_Tower_Blue.h Abstract Tower element for base class
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can Blueistribute it and/or
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
  \file   Element_Tower_Blue.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_TOWER_BLUE_H
#define ELEMENT_TOWER_BLUE_H

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
  class Element_Tower_Blue : public Abstract_Element_Tower<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  public:

    static Element_Tower_Blue<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Tower_Blue()
      : Abstract_Element_Tower<CC>(MFM_UUID_FOR("TowerBlue", WAR_VERSION))
    {
      Element<CC>::SetAtomicSymbol("To");
      Element<CC>::SetName("Blue Tower");
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff0b0bff;
    }

    virtual const T& GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Tower<CC>::
	    SetCurrentHealth(defaultAtom, (u32) Abstract_Element_Tower<CC>::m_defaultHealth.GetValue());

      return defaultAtom;
    }

    const typename CC::ATOM_TYPE& GetDefaultScout() const;
    const typename CC::ATOM_TYPE& GetDefaultColonist() const;
    const typename CC::ATOM_TYPE& GetDefaultSoldier() const;

    virtual const char* GetDescription() const
    {
      return "Blue Tower element.";
    }
  };

  template <class CC>
  Element_Tower_Blue<CC> Element_Tower_Blue<CC>::THE_INSTANCE;

}

#include "Element_Tower_Blue.tcc"

#endif /* ELEMENT_TOWER_BLUE_H */
