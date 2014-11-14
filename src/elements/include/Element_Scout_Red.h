/*                                              -*- mode:C++ -*-
  Element_Scout_Red.h Red Scout Agent
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
  \file   Element_Scout_Red.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_SCOUT_RED_H
#define ELEMENT_SCOUT_RED_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Abstract_Element_Scout.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Element_Scout_Red : public Abstract_Element_Scout<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  public:

    static Element_Scout_Red<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Scout_Red()
      : Abstract_Element_Scout<CC>(MFM_UUID_FOR("Scout", WAR_VERSION))
    {
      Element<CC>::SetAtomicSymbol("Sc");
      Element<CC>::SetName("Scout");
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xffdda0a0;
    }

    virtual const T& GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Scout<CC>::
	    SetCurrentHealth(defaultAtom, (u32) Abstract_Element_Scout<CC>::m_defaultHealth.GetValue());

      Abstract_Element_Scout<CC>::
      SetCurrentDirection(defaultAtom, rand() % Dirs::DIR_COUNT);

      u32 id = rand() % (1<<Abstract_Element_Scout<CC>::ID_LEN);
      LOG.Debug("Made scout with id = %d", id);
      Abstract_Element_Scout<CC>::
      SetID(defaultAtom, id);


      Abstract_Element_Scout<CC>::
      SetCurrentLifeTimer(defaultAtom, (u32) Abstract_Element_Scout<CC>::m_defaultLifeTimer.GetValue());

      return defaultAtom;
    }

    virtual const Abstract_Element_Breadcrumb<CC>& GetBreadcrumbElement() const;

    virtual const bool IsAtomInteresting(EventWindow<CC>& window, const T& atom) const;

    virtual const char* GetDescription() const
    {
      return "Red Scout element.";
    }
  };

  template <class CC>
  Element_Scout_Red<CC> Element_Scout_Red<CC>::THE_INSTANCE;

}

#include "Element_Scout_Red.tcc"

#endif /* ELEMENT_SCOUT_RED_H */
