/*                                              -*- mode:C++ -*-
  Element_Tower.h Abstract Tower element for base class
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
  \file   Element_Tower.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_TOWER_H
#define ELEMENT_TOWER_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"

namespace MFM
{
  #define WAR_VERSION 1

  template <class CC>
  class Element_Tower : public Element<CC>
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
      CURRENT_HEALTH_LEN = 10,

      ID_POS = CURRENT_HEALTH_POS + CURRENT_HEALTH_LEN,
      ID_LEN = 10
    };

    typedef BitField<BitVector<BITS>, CURRENT_HEALTH_LEN, CURRENT_HEALTH_POS> AFCurrentHealth;
    typedef BitField<BitVector<BITS>, ID_LEN, ID_POS> AFID;


  public:

    static Element_Tower<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Tower()
      : Element<CC>(MFM_UUID_FOR("TowerRed", WAR_VERSION)),
       m_defaultHealth(this, "defaultHealth", "Default Health",
                  "This is the health the tower will start with.", 1, 10, 20, 1),
        m_soldierSpawnChance(this, "soldierSpawnChance", "Soldier Spawn Chance",
                  "This is the chance of spawning a soldier in a given tick.", 1, 80, 1000, 10),
        m_colonistSpawnChance(this, "colonistSpawnChance", "Colonist Spawn Chance",
                  "This is the chance of spawning a colonist in a given tick.", 500, 5000, 10000, 500),
        m_scoutSpawnChance(this, "scoutSpawnChance", "Scout Spawn Chance",
                  "This is the chance of spawning a scout in a given tick.", 200, 1000, 10000, 100)
    {
      Element<CC>::SetAtomicSymbol("To");
      Element<CC>::SetName("Red Tower");
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xffff0b0b;
    }

    virtual const T& GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      Abstract_Element_Tower<CC>::
      SetCurrentHealth(defaultAtom, (u32) Abstract_Element_Tower<CC>::m_defaultHealth.GetValue());

      Abstract_Element_Tower<CC>::
      SetID(defaultAtom, rand() % (1<<Abstract_Element_Tower<CC>::ID_LEN));

      return defaultAtom;
    }

    const typename CC::ATOM_TYPE& GetDefaultScout() const;
    const typename CC::ATOM_TYPE& GetDefaultColonist() const;
    const typename CC::ATOM_TYPE& GetDefaultSoldier() const;

    virtual const char* GetDescription() const
    {
      return "Red Tower element.";
    }

    u32 GetID(const T& us) const
    {
      return AFID::Read(this->GetBits(us));
    }

    void SetID(T& us, const u32 id) const
    {
      AFID::Write(this->GetBits(us), id);
    }

    u32 GetCurrentHealth(const T& us) const
    {
      return AFCurrentHealth::Read(this->GetBits(us));
    }

    void SetCurrentHealth(T& us, const u32 health) const
    {
      AFCurrentHealth::Write(this->GetBits(us), health);
    }

    T GetMutableAtom(const T& oldMe) const
    {
      T me = oldMe;

      return me;
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual void Behavior(EventWindow<CC>& window) const;


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

  template <class CC>
  Element_Tower<CC> Element_Tower<CC>::THE_INSTANCE;
}

#include "Element_Tower.tcc"

#endif /* ELEMENT_TOWER_H */
