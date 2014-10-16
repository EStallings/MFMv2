/*                                              -*- mode:C++ -*-
  Element_Soldier_Blue.h Blue Soldier Agent
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
  \file   Element_Soldier_Blue.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_SOLDIER_BLUE_H
#define ELEMENT_SOLDIER_BLUE_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Abstract_Element_Soldier.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Element_Soldier_Blue : public Abstract_Element_Soldier<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  public:

    static Element_Soldier_Blue<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Soldier_Blue()
      : Abstract_Element_Soldier<CC>(MFM_UUID_FOR("SoldierXBBlue", WAR_VERSION))
    {
      Element<CC>::SetAtomicSymbol("Sb");
      Element<CC>::SetName("Blue Soldier");
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff0000a0;
    }

    virtual const T& GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Soldier<CC>::
	    SetCurrentHealth(defaultAtom, (u32) Abstract_Element_Soldier<CC>::m_defaultHealth.GetValue());
      
      Abstract_Element_Soldier<CC>::
      SetCurrentLifeTimer(defaultAtom, (u32) Abstract_Element_Soldier<CC>::m_defaultLifeTimer.GetValue());

      return defaultAtom;
    }

    const Element<CC>* GetScoutElement()      const;
    const Element<CC>* GetTowerElement()      const;
    const Element<CC>* GetColonistElement()   const;
    const Element<CC>* GetBreadcrumbElement() const;

    const bool AttemptAttack(EventWindow<CC>& window, SPoint location) const;


    const bool IsMyBreadcrumbType(const u32 type) const;
    const bool IsMyBreadcrumbAlerted(const T& bc) const;
    const u32 GetMyBreadcrumbIndex(const T& bc) const;

    virtual const char* GetDescription() const
    {
      return "Blue Soldier element.";
    }
  };

  template <class CC>
  Element_Soldier_Blue<CC> Element_Soldier_Blue<CC>::THE_INSTANCE;

}


#include "Element_Soldier_Blue.tcc"

#endif /* ELEMENT_SOLDIER_BLUE_H */
