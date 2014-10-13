/*                                              -*- mode:C++ -*-
  Abstract_Element_Tower.h Abstract Tower element for base class
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
  \file   Abstract_Element_Tower.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ABSTRACT_ELEMENT_TOWER_H
#define ABSTRACT_ELEMENT_TOWER_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"

namespace MFM
{
  #define WAR_VERSION 1

  template <class CC>
  class Abstract_Element_Tower : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  protected:

    ElementParameterS32<CC> m_defaultHealth;
    ElementParameterS32<CC> m_soldierSpawnChance;
    ElementParameterS32<CC> m_colonistSpawnChance;
    ElementParameterS32<CC> m_scoutSpawnChance;

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

    Abstract_Element_Tower(UUID u)
      : Element<CC>(u),
        m_defaultHealth(this, "defaultHealth", "Default Health",
                  "This is the health the tower will start with.", 1, 200, 1000, 10),
        m_soldierSpawnChance(this, "soldierSpawnChance", "Soldier Spawn Chance",
                  "This is the chance of spawning a soldier in a given tick.", 1, 500, 1000, 10),
        m_colonistSpawnChance(this, "colonistSpawnChance", "Colonist Spawn Chance",
                  "This is the chance of spawning a colonist in a given tick.", 1, 1000, 1000, 10),
        m_scoutSpawnChance(this, "scoutSpawnChance", "Scout Spawn Chance",
                  "This is the chance of spawning a scout in a given tick.", 1, 400, 1000, 10)
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
      return 0;
    }

    virtual void Behavior(EventWindow<CC>& window) const;

    virtual const typename CC::ATOM_TYPE& GetDefaultScout() const = 0;
    virtual const typename CC::ATOM_TYPE& GetDefaultSoldier() const = 0;
    virtual const typename CC::ATOM_TYPE& GetDefaultColonist() const = 0;

   private:
    void PlaceAtomRandomly(EventWindow<CC>& window, const T& atom) const
    {
        SPoint location;
        u32 emptyCount = 0;
        MDist<R> n = MDist<R>::get();

        for(u32 i = n.GetFirstIndex(1); i <= n.GetLastIndex(R); i++)
	      {
          SPoint searchLoc = n.GetPoint(i);
          if(window.GetRelativeAtom(searchLoc).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
	        {
            emptyCount++;
            if(window.GetRandom().OneIn(emptyCount))
	          {
              location = searchLoc;
            }
          }
        }
        window.SetRelativeAtom(location, atom);
      }
  };
}

#include "Abstract_Element_Tower.tcc"

#endif /* ABSTRACT_ELEMENT_TOWER_H */
