/*                                              -*- mode:C++ -*-
  Abstract_Element_Breadcrumb.h
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
  \file Abstract_Element_Breadcrumb.h Pathfinding element
  \author Trent R. Small
  \author Ezra S. Stallings.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ABSTRACT_ELEMENT_BREADCRUMB_H
#define ABSTRACT_ELEMENT_BREADCRUMB_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"


namespace MFM
{
  template <class CC>
  class Abstract_Element_Breadcrumb : public Element<CC>
  {
   public:
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum
    {
      R = P::EVENT_WINDOW_RADIUS,
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      PATH_ID_POS = P3Atom<P>::P3_STATE_BITS_POS,
      PATH_ID_LEN = 10,

      INDEX_POS = PATH_ID_POS + PATH_ID_LEN,
      INDEX_LEN = 10,

      PREV_INDEX_POS = INDEX_POS + INDEX_LEN,
      PREV_INDEX_LEN = 10,

      NEXT_INDEX_POS = PREV_INDEX_POS + PREV_INDEX_LEN,
      NEXT_INDEX_LEN = 10,

      ACTIVE_TIMER_POS = NEXT_INDEX_POS + NEXT_INDEX_LEN,
      ACTIVE_TIMER_LEN = 5,

      UPSTREAM_LOCAL_DEMAND_POS = ACTIVE_TIMER_POS + ACTIVE_TIMER_LEN,
      UPSTREAM_LOCAL_DEMAND_LEN = 4,

      DOWNSTREAM_LOCAL_DEMAND_POS = UPSTREAM_LOCAL_DEMAND_POS + UPSTREAM_LOCAL_DEMAND_LEN, 
      DOWNSTREAM_LOCAL_DEMAND_LEN = 4,

      UPSTREAM_DEMAND_POS = DOWNSTREAM_LOCAL_DEMAND_POS + DOWNSTREAM_LOCAL_DEMAND_LEN,
      UPSTREAM_DEMAND_LEN = 4,

      DOWNSTREAM_DEMAND_POS = UPSTREAM_DEMAND_POS + UPSTREAM_DEMAND_LEN, 
      DOWNSTREAM_DEMAND_LEN = 4,

      TRAFFIC_DIR_POS = DOWNSTREAM_DEMAND_POS + DOWNSTREAM_DEMAND_LEN,
      TRAFFIC_DIR_LEN = 1,

      IS_ENDPOINT_POS = TRAFFIC_DIR_POS + TRAFFIC_DIR_LEN,
      IS_ENDPOINT_LEN = 1,

      ENDPOINT_SWITCH_POS = IS_ENDPOINT_POS + IS_ENDPOINT_LEN,
      ENDPOINT_SWITCH_LEN = 1

    };

   protected:
    typedef BitField<BitVector<BITS>, PATH_ID_LEN, PATH_ID_POS>                                   AFPathID;
    typedef BitField<BitVector<BITS>, INDEX_LEN, INDEX_POS>                                       AFIndex;
    typedef BitField<BitVector<BITS>, PREV_INDEX_LEN, PREV_INDEX_POS>                             AFPrevIndex;
    typedef BitField<BitVector<BITS>, NEXT_INDEX_LEN, NEXT_INDEX_POS>                             AFNextIndex;
    typedef BitField<BitVector<BITS>, ACTIVE_TIMER_LEN, ACTIVE_TIMER_POS>                         AFActiveTimer;
    typedef BitField<BitVector<BITS>, UPSTREAM_LOCAL_DEMAND_LEN, UPSTREAM_LOCAL_DEMAND_POS>       AFUpstreamLocalDemand;
    typedef BitField<BitVector<BITS>, DOWNSTREAM_LOCAL_DEMAND_LEN, DOWNSTREAM_LOCAL_DEMAND_POS>   AFDownstreamLocalDemand;
    typedef BitField<BitVector<BITS>, UPSTREAM_DEMAND_LEN, UPSTREAM_DEMAND_POS>                   AFUpstreamDemand;
    typedef BitField<BitVector<BITS>, DOWNSTREAM_DEMAND_LEN, DOWNSTREAM_DEMAND_POS>               AFDownstreamDemand;
    typedef BitField<BitVector<BITS>, TRAFFIC_DIR_LEN, TRAFFIC_DIR_POS>                           AFTrafficDir;
    typedef BitField<BitVector<BITS>, IS_ENDPOINT_LEN, IS_ENDPOINT_POS>                           AFIsEndpoint;
    typedef BitField<BitVector<BITS>, ENDPOINT_SWITCH_LEN, ENDPOINT_SWITCH_POS>                   AFEndpointSwitch;
    

    public:
    Abstract_Element_Breadcrumb(const UUID & uuid) : Element<CC>(uuid)
    { }

    u32 GetActive(const T& us) const
    {
      return AFActiveTimer::Read(this->GetBits(us));
    }

    void SetActive(T& us, const u32 age) const
    {
      AFActiveTimer::Write(this->GetBits(us), age);
    }
    bool IsActive(const T& us) const
    {
      return GetActive(us) > 0;
    }
    
    u32 GetPathID(const T& us) const
    {
      return AFPathID::Read(this->GetBits(us));
    }

    void SetPathID(T& us, const u32 id) const
    {
      AFPathID::Write(this->GetBits(us), id);
    }

    u32 GetIndex(const T& us) const
    {
      return AFIndex::Read(this->GetBits(us));
    }

    void SetIndex(T& us, const u32 idx) const
    {
      AFIndex::Write(this->GetBits(us), idx);
    }

    u32 GetPrevIndex(const T& us) const
    {
      return AFPrevIndex::Read(this->GetBits(us));
    }

    void SetPrevIndex(T& us, const u32 idx) const
    {
      AFPrevIndex::Write(this->GetBits(us), idx);
    }

    u32 GetNextIndex(const T& us) const
    {
      return AFNextIndex::Read(this->GetBits(us));
    }

    void SetNextIndex(T& us, const u32 idx) const
    {
      AFNextIndex::Write(this->GetBits(us), idx);
    }

    u32 GetUpstreamLocalDemand(const T& us) const
    {
      return AFUpstreamLocalDemand::Read(this->GetBits(us));
    }

    void SetUpstreamLocalDemand(T& us, const u32 idx) const
    {
      AFUpstreamLocalDemand::Write(this->GetBits(us), idx);
    }

    u32 GetDownstreamLocalDemand(const T& us) const
    {
      return AFDownstreamLocalDemand::Read(this->GetBits(us));
    }

    void SetDownstreamLocalDemand(T& us, const u32 idx) const
    {
      AFDownstreamLocalDemand::Write(this->GetBits(us), idx);
    }
    
    u32 GetUpstreamDemand(const T& us) const
    {
      return AFUpstreamDemand::Read(this->GetBits(us));
    }

    void SetUpstreamDemand(T& us, const u32 idx) const
    {
      AFUpstreamDemand::Write(this->GetBits(us), idx);
    }

    u32 GetDownstreamDemand(const T& us) const
    {
      return AFDownstreamDemand::Read(this->GetBits(us));
    }

    void SetDownstreamDemand(T& us, const u32 idx) const
    {
      AFDownstreamDemand::Write(this->GetBits(us), idx);
    }
    

    u32 GetTrafficDir(const T& us) const
    {
      return AFTrafficDir::Read(this->GetBits(us));
    }

    void SetTrafficDir(T& us, const u32 idx) const
    {
      AFTrafficDir::Write(this->GetBits(us), idx);
    }

    u32 GetIsEndpoint(const T& us) const
    {
      return AFIsEndpoint::Read(this->GetBits(us));
    }

    void SetIsEndpoint(T& us, const u32 bit) const
    {
      AFIsEndpoint::Write(this->GetBits(us), bit);
    }

    u32 GetEndpointSwitch(const T& us) const
    {
      return AFEndpointSwitch::Read(this->GetBits(us));
    }

    void SetEndpointSwitch(T& us, const u32 bit) const
    {
      AFEndpointSwitch::Write(this->GetBits(us), bit);
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
    
  };
}

#endif /* ABSTRACT_ELEMENT_BREADCRUMB_H */
