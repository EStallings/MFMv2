/*                                              -*- mode:C++ -*-
  Abstract_Element_Soldier.h Abstract Soldier element for base class
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
  \file   Abstract_Element_Soldier.h Abstract Soldier element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ABSTRACT_ELEMENT_SOLDIER_H
#define ABSTRACT_ELEMENT_SOLDIER_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Element_Empty.h"
#include "Abstract_Element_Tower.h"
#include "Abstract_Element_Scout.h"
#include "Abstract_Element_Colonist.h"
#include "Abstract_Element_Breadcrumb.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Abstract_Element_Soldier : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  protected:

    ElementParameterS32<CC> m_defaultHealth;
    ElementParameterS32<CC> m_attackDamage;

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      CURRENT_HEALTH_POS = P3Atom<P>::P3_STATE_BITS_POS,
      CURRENT_HEALTH_LEN = 10
    };

    typedef BitField<BitVector<BITS>, CURRENT_HEALTH_LEN, CURRENT_HEALTH_POS> AFCurrentHealth;

  public:

    Abstract_Element_Soldier(UUID u)
      : Element<CC>(u),
        m_defaultHealth(this, "defaultHealth", "Default Health",
                  "This is the health the soldier will start with.", 1, 200, 1000, 10),
        m_attackDamage(this, "attackDamage", "Attack Damage",
		            "This is the damage done in a single tick.", 1, 5, 100, 1)
    {}

    u32 GetCurrentHealth(const T& us) const
    {
      return AFCurrentHealth::Read(this->GetBits(us));
    }

    void SetCurrentHealth(T& us, const u32 health) const
    {
      AFCurrentHealth::Write(this->GetBits(us), health);
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 100;
    }

    T GetMutableMe(const T& me) const
    {
      T newMe(me.GetType(), 0, 0, 0);

      SetCurrentHealth(newMe, GetCurrentHealth(me));

      return newMe;
    }

    virtual const Element<CC>* GetScoutElement()      const = 0;
    virtual const Element<CC>* GetTowerElement()      const = 0;
    virtual const Element<CC>* GetColonistElement()   const = 0;
    virtual const Element<CC>* GetBreadcrumbElement() const = 0;


    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& constSelf = window.GetCenterAtom();
      T self = GetMutableMe(constSelf);
      //Random& rand = window.GetRandom();

      //If out of health, die
      if(GetCurrentHealth(self) <= 0){
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        return;
      }
    }
  };
}


#endif /* ABSTRACT_ELEMENT_SOLDIER_H */
