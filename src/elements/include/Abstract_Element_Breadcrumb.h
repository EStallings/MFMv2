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
#include "Element_Data.h"

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

      ALERT_TIMER_POS = NEXT_INDEX_POS + NEXT_INDEX_LEN,
      ALERT_TIMER_LEN = 1,

      COOLDOWN_TIMER_POS = ALERT_TIMER_POS + ALERT_TIMER_LEN,
      COOLDOWN_TIMER_LEN = 9
    };

   protected:
    typedef BitField<BitVector<BITS>, PATH_ID_LEN, PATH_ID_POS> AFPathID;
    typedef BitField<BitVector<BITS>, INDEX_LEN, INDEX_POS> AFIndex;
    typedef BitField<BitVector<BITS>, PREV_INDEX_LEN, PREV_INDEX_POS> AFPrevIndex;
    typedef BitField<BitVector<BITS>, NEXT_INDEX_LEN, NEXT_INDEX_POS> AFNextIndex;
    typedef BitField<BitVector<BITS>, ALERT_TIMER_LEN, ALERT_TIMER_POS> AFAlertTimer;
    typedef BitField<BitVector<BITS>, COOLDOWN_TIMER_LEN, COOLDOWN_TIMER_POS> AFCooldownTimer;

   private:
    ElementParameterS32<CC> m_alertLength;
    ElementParameterS32<CC> m_cooldownLength;

    u32 GetAlert(const T& us) const
    {
      return AFAlertTimer::Read(this->GetBits(us));
    }

    void SetAlert(T& us, const u32 age) const
    {
      AFAlertTimer::Write(this->GetBits(us), age);
    }

    u32 GetCooldownTimer(const T& us) const
    {
      return AFCooldownTimer::Read(this->GetBits(us));
    }

    void SetCooldownTimer(T& us, const u32 age) const
    {
      AFCooldownTimer::Write(this->GetBits(us), age);
    }

    bool IsCooldown(const T& us) const
    {
      return GetCooldownTimer(us) > 0;
    }

    void DecrementCooldown(T& us) const
    {
      SetCooldownTimer(us, GetCooldownTimer(us) - 1);
    }

   protected:

    virtual u32 GetMyBreadcrumbType() const = 0;

    

    public:
    Abstract_Element_Breadcrumb(const UUID & uuid) :
      Element<CC>(uuid),
      m_alertLength(this, "alertLength", "Alert Length",
        "Breadcrumb Alert timer initial length",
        1, 100, 255, 1),
      m_cooldownLength(this, "cooldownLength", "Cooldown Length",
        "Breadcrumb Cooldown timer initial length",
        1, 100, 255, 1)
    { }


    bool IsAlert(const T& us) const
    {
      return GetAlert(us) > 0;
    }
    
    void Alert(T& us) const
    {
      SetAlert(us, 1);
    }
    
    void Cooldown(T& us) const
    {
      SetCooldownTimer(us, (1 << COOLDOWN_TIMER_LEN) - 1);
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
    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& me = window.GetCenterAtom();
      //LOG.Debug("Examining BC:%d; Pred=%d, Succ=%d, timer=%d", GetIndex(me), GetPrevIndex(me), GetNextIndex(me), GetCooldownTimer(me));

      if(!IsAlert(me)){
        T mutableMe = GetMutableAtom(window.GetCenterAtom());
        DecrementCooldown(mutableMe);
        window.SetCenterAtom(mutableMe);
      }

      if(!IsCooldown(me))
      {
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
      }
      else
      {
        MDist<R>& md = MDist<R>::get();
        SPoint pred, succ;
        bool fP = false, fS = false;

        //Find predecessor and successor
        for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
        {
          SPoint pt = md.GetPoint(i);

          if(window.GetRelativeAtom(pt).GetType() ==
             GetMyBreadcrumbType())
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
        }

        //Become alert if neighbor is alert
        //if((!IsAlert(me)) && ((fP && IsAlert(window.GetRelativeAtom(pred))) || (fS && IsAlert(window.GetRelativeAtom(succ)))))
        if((!IsAlert(me)) && (fS && IsAlert(window.GetRelativeAtom(succ))))
        {
          T mutableMe = GetMutableAtom(window.GetCenterAtom());
          Alert(mutableMe);
          window.SetCenterAtom(mutableMe);
        }
        //Move to average position
        if(fP && fS){
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
      }
    }
  };
}

#endif /* ABSTRACT_ELEMENT_BREADCRUMB_H */
