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
#include "Element_Empty.h"
#include "Element_Res.h"

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
      IS_ENDPOINT_LEN = 1

    };

   protected:
    typedef BitField<BitVector<BITS>, PATH_ID_LEN, PATH_ID_POS> AFPathID;
    typedef BitField<BitVector<BITS>, INDEX_LEN, INDEX_POS> AFIndex;
    typedef BitField<BitVector<BITS>, PREV_INDEX_LEN, PREV_INDEX_POS> AFPrevIndex;
    typedef BitField<BitVector<BITS>, NEXT_INDEX_LEN, NEXT_INDEX_POS> AFNextIndex;
    typedef BitField<BitVector<BITS>, ACTIVE_TIMER_LEN, ACTIVE_TIMER_POS> AFActiveTimer;
    typedef BitField<BitVector<BITS>, UPSTREAM_LOCAL_DEMAND_LEN, UPSTREAM_LOCAL_DEMAND_POS> AFUpstreamLocalDemand;
    typedef BitField<BitVector<BITS>, DOWNSTREAM_LOCAL_DEMAND_LEN, DOWNSTREAM_LOCAL_DEMAND_POS> AFDownstreamLocalDemand;
    typedef BitField<BitVector<BITS>, UPSTREAM_DEMAND_LEN, UPSTREAM_DEMAND_POS> AFUpstreamDemand;
    typedef BitField<BitVector<BITS>, DOWNSTREAM_DEMAND_LEN, DOWNSTREAM_DEMAND_POS> AFDownstreamDemand;
    typedef BitField<BitVector<BITS>, TRAFFIC_DIR_LEN, TRAFFIC_DIR_POS> AFTrafficDir;

    virtual u32 GetMyBreadcrumbType() const = 0;
    virtual u32 GetMyTowerType() const = 0;

    

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

    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& me = window.GetCenterAtom();
      Random & random = window.GetRandom();

      //Delayed start to behavior to ensure everything gets linked!
      if(GetActive(me) > 0){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        SetActive(mutableMe, GetActive(me) - 1);
        window.SetCenterAtom(mutableMe);
        return;
      }

      //LOG.Debug("Examining BC:%d; Pred=%d, Succ=%d, timer=%d", GetIndex(me), GetPrevIndex(me), GetNextIndex(me), GetActive(me));

      MDist<R>& md = MDist<R>::get();
      SPoint pred, succ;
      bool fP = false, fS = false;

      u32 numTowers = 0;
      SPoint towerPos;
      bool fT = false;

      u32 resType = Element_Res<CC>::THE_INSTANCE.GetType();
      SPoint resPos;
      u32 numRes = 0;
      bool fR = false;


      //Find predecessor and successor, and a random valid tower if possible
      for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
      {
        SPoint pt = md.GetPoint(i);

        if(window.GetRelativeAtom(pt).GetType() == GetMyBreadcrumbType())
        {
          if(GetIndex(window.GetRelativeAtom(pt)) == GetPrevIndex(me) && GetPathID(window.GetRelativeAtom(pt)) == GetPathID(me))
          {
            pred = pt;
            fP = true;
          }
          else if(GetIndex(window.GetRelativeAtom(pt)) == GetNextIndex(me) && GetPathID(window.GetRelativeAtom(pt)) == GetPathID(me))
          {
            succ = pt;
            fS = true;
          }
        }
        else if(window.GetRelativeAtom(pt).GetType() == GetMyTowerType())
        {
          numTowers++;
          fT = true;
          if(random.OneIn(numTowers)){
            towerPos = pt;
          }
        }
        else if(window.GetRelativeAtom(pt).GetType() == resType)
        {
          numRes++;
          fR = true;
          if(random.OneIn(numRes)){
            resPos = pt;
          }
        }
      }

      //if disconnected, die
      if(!(fP && fS) && !(fT && (fS || fP))) {
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        return;
      }

      //logic if we need to interact with a tower
      if(!fP || !fS){
        if(!fP){
          pred = towerPos; //for positioning

        }
        else if(!fS){
          succ = towerPos; //for positioning

        }
      }

      //Otherwise, move res
      else if(fR){
        SPoint goal = succ;
        if(GetTrafficDir(me) == 1)
          goal = pred;
        u32 minDist = 16;
        SPoint minPos = resPos;
        for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
        {
          SPoint pt = md.GetPoint(i);
          if(window.GetRelativeAtom(pt).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType()){
            u32 dist = (pt-goal).GetManhattanLength();
            if(dist < minDist){
              minPos = pt;
            }
            //LOG.Debug("Moving res a distance of %d from goal!", dist);
          }
        }
        window.SwapAtoms(minPos, resPos);
      }

      //get pred's data
      if(fP){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        SetDownstreamLocalDemand(mutableMe, GetDownstreamLocalDemand(window.GetRelativeAtom(pred)));
        SetDownstreamDemand(mutableMe, GetDownstreamDemand(window.GetRelativeAtom(pred)));
        window.SetCenterAtom(mutableMe);
      }

      //get succ's data
      if(fS){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        SetUpstreamLocalDemand(mutableMe, GetUpstreamLocalDemand(window.GetRelativeAtom(pred)));
        SetUpstreamDemand(mutableMe, GetUpstreamDemand(window.GetRelativeAtom(pred)));
        window.SetCenterAtom(mutableMe);
      }

      //modify traffic direction
      if(GetUpstreamLocalDemand(me) > GetDownstreamLocalDemand(me) && GetTrafficDir(me) == 0){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        SetTrafficDir(mutableMe, 1);
        window.SetCenterAtom(mutableMe);
      }

      else if(GetUpstreamLocalDemand(me) < GetDownstreamLocalDemand(me) && GetTrafficDir(me) == 1){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        SetTrafficDir(mutableMe, 0);
        window.SetCenterAtom(mutableMe);
      }
      //Move to average position
      if((pred-succ).GetManhattanLength() < R){

        //LOG.Debug("Deleting BC:%d; Pred=%d, Succ=%d", GetIndex(me), GetPrevIndex(me), GetNextIndex(me));

        T predAtom = GetMutableAtom(window.GetRelativeAtom(pred));
        SetNextIndex(predAtom, GetNextIndex(me));
        window.SetRelativeAtom(pred, predAtom);

        T succAtom = GetMutableAtom(window.GetRelativeAtom(succ));
        SetPrevIndex(succAtom, GetPrevIndex(me));
        window.SetRelativeAtom(succ, succAtom);

        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
      }
      else{
        u32 Xpoint = pred.GetX() + succ.GetX();
        Xpoint = (abs(Xpoint) > 1) ? Xpoint/2 : Xpoint;

        u32 Ypoint = pred.GetY() + succ.GetY();
        Ypoint = (abs(Ypoint) > 1) ? Ypoint/2 : Ypoint;

        SPoint vec = SPoint(Xpoint,Ypoint);
        if(window.IsLiveSite(vec) && window.GetRelativeAtom(vec).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
          window.SwapAtoms(vec, SPoint(0,0));
      }
    }
    
    
  };
}

#endif /* ABSTRACT_ELEMENT_BREADCRUMB_H */
