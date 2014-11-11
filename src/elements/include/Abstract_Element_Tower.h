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

    ElementParameterS32<CC> m_colonistSpawnChance;
    ElementParameterS32<CC> m_scoutSpawnChance;
    ElementParameterS32<CC> m_resSpawnChance;

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      CURRENT_DEMAND_POS = P3Atom<P>::P3_STATE_BITS_POS,
      CURRENT_DEMAND_LEN = 4,

      LOCAL_DEMAND_POS = CURRENT_DEMAND_POS + CURRENT_DEMAND_LEN,
      LOCAL_DEMAND_LEN = 4,

      NUM_CONNECTIONS_POS = LOCAL_DEMAND_POS + LOCAL_DEMAND_LEN,
      NUM_CONNECTIONS_LEN = 3,

      ID_POS = NUM_CONNECTIONS_POS + NUM_CONNECTIONS_LEN,
      ID_LEN = 10
    };

    typedef BitField<BitVector<BITS>, CURRENT_DEMAND_LEN, CURRENT_DEMAND_POS> AFCurrentDemand;
    typedef BitField<BitVector<BITS>, LOCAL_DEMAND_LEN, LOCAL_DEMAND_POS> AFLocalDemand;
    typedef BitField<BitVector<BITS>, NUM_CONNECTIONS_LEN, NUM_CONNECTIONS_POS> AFNumConnections;
    typedef BitField<BitVector<BITS>, ID_LEN, ID_POS> AFID;


  public:

    Abstract_Element_Tower(UUID u)
      : Element<CC>(u),
        m_colonistSpawnChance(this, "colonistSpawnChance", "Colonist Spawn Chance",
                  "This is the chance of spawning a colonist in a given tick.", 500, 5000, 10000, 500),
        m_scoutSpawnChance(this, "scoutSpawnChance", "Scout Spawn Chance",
                  "This is the chance of spawning a scout in a given tick.", 200, 1000, 10000, 100),
        m_resSpawnChance(this, "resSpawnChance", "Res Spawn Chance",
                  "This is the chance of res spawning in a given tick.", 1, 5, 10, 1)
    {}

    u32 GetID(const T& us) const
    {
      return AFID::Read(this->GetBits(us));
    }

    void SetID(T& us, const u32 id) const
    {
      AFID::Write(this->GetBits(us), id);
    }

    u32 GetCurrentDemand(const T& us) const
    {
      return AFCurrentDemand::Read(this->GetBits(us));
    }

    void SetCurrentDemand(T& us, const u32 demand) const
    {
      AFCurrentDemand::Write(this->GetBits(us), demand);
    }

    u32 GetLocalDemand(const T& us) const
    {
      return AFLocalDemand::Read(this->GetBits(us));
    }

    void SetLocalDemand(T& us, const u32 demand) const
    {
      AFLocalDemand::Write(this->GetBits(us), demand);
    }

    T GetMutableAtom(const T& oldMe) const
    {
      T me = oldMe;

      return me;
    }

    virtual u32 Diffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return nowAt.Equals(maybeAt)?Element<CC>::COMPLETE_DIFFUSABILITY:0;
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }


    virtual const typename CC::ATOM_TYPE& GetDefaultScout() const = 0;
    virtual const typename CC::ATOM_TYPE& GetDefaultColonist() const = 0;

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
