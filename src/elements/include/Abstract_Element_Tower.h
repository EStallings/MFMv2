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
    ElementParameterS32<CC> m_mode;

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields
      ID_POS = P3Atom<P>::P3_STATE_BITS_POS,
      ID_LEN = 10,

      CURRENT_DEMAND_POS = ID_POS + ID_LEN,
      CURRENT_DEMAND_LEN = 4,

      LOCAL_DEMAND_POS = CURRENT_DEMAND_POS + CURRENT_DEMAND_LEN,
      LOCAL_DEMAND_LEN = 5,

      CURRENT_SUPPLY_POS = LOCAL_DEMAND_POS + LOCAL_DEMAND_LEN,
      CURRENT_SUPPLY_LEN = 4,

      NUM_CONNECTIONS_POS = CURRENT_SUPPLY_POS + CURRENT_SUPPLY_LEN,
      NUM_CONNECTIONS_LEN = 3
      
    };

    typedef BitField<BitVector<BITS>, ID_LEN, ID_POS> AFID;
    typedef BitField<BitVector<BITS>, CURRENT_DEMAND_LEN, CURRENT_DEMAND_POS> AFCurrentDemand;
    typedef BitField<BitVector<BITS>, LOCAL_DEMAND_LEN, LOCAL_DEMAND_POS> AFLocalDemand;
    typedef BitField<BitVector<BITS>, CURRENT_SUPPLY_LEN, CURRENT_SUPPLY_POS> AFCurrentSupply;
    typedef BitField<BitVector<BITS>, NUM_CONNECTIONS_LEN, NUM_CONNECTIONS_POS> AFNumConnections;

  public:

    Abstract_Element_Tower(UUID u): 
      Element<CC>(u),
      m_mode(this, "mode", "Simulation Mode",
                  "This is the mode of simulation: 0 for no scouts spawn, 1 for scouts do spawn.", 1, 1, 2, 1)
    {}

    u32 GetID(const T& us) const
    {
      return AFID::Read(this->GetBits(us));
    }

    void SetID(T& us, const u32 id) const
    {
      LOG.Debug("P3 bits: %d, ID bits: %d, demand: %d, supply: %d\n", P3Atom<P>::P3_STATE_BITS_POS, ID_POS, CURRENT_DEMAND_POS, CURRENT_SUPPLY_POS);
       
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

    u32 GetNumConnections(const T& us) const
    {
      return AFNumConnections::Read(this->GetBits(us));
    }

    void SetNumConnections(T& us, const u32 num) const
    {
      AFNumConnections::Write(this->GetBits(us), num);
    }

    u32 GetCurrentSupply(const T& us) const
    {
      return AFCurrentSupply::Read(this->GetBits(us));
    }

    void SetCurrentSupply(T& us, const u32 num) const
    {
      AFCurrentSupply::Write(this->GetBits(us), num);
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
        if(emptyCount > 0)
          window.SetRelativeAtom(location, atom);
      }
  };
}

#include "Abstract_Element_Tower.tcc"

#endif /* ABSTRACT_ELEMENT_TOWER_H */
