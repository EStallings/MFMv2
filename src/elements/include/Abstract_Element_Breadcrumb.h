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

      INDEX_POS = P3Atom<P>::P3_STATE_BITS_POS,
      INDEX_LEN = 8,

      ALERT_TIMER_POS = INDEX_POS + INDEX_LEN,
      ALERT_TIMER_LEN = 8,

      COOLDOWN_TIMER_POS = ALERT_TIMER_POS + ALERT_TIMER_LEN,
      COOLDOWN_TIMER_LEN = 8
    };

   protected:
    typedef BitField<BitVector<BITS>, INDEX_LEN, INDEX_POS> AFIndex;
    typedef BitField<BitVector<BITS>, ALERT_TIMER_LEN, ALERT_TIMER_POS> AFAlertTimer;
    typedef BitField<BitVector<BITS>, COOLDOWN_TIMER_LEN, COOLDOWN_TIMER_POS> AFCooldownTimer;

   private:
    ElementParameterS32<CC> m_alertLength;
    ElementParameterS32<CC> m_cooldownLength;

    u32 GetIndex(const T& us) const
    {
      return AFIndex::Read(this->GetBits(us));
    }

    u32 GetAlertTimer(const T& us) const
    {
      return AFAlertTimer::Read(this->GetBits(us));
    }

    void SetAlertTimer(T& us, const u32 age) const
    {
      AFAlertTimer::Write(this->GetBits(us), age);
    }

    void Alert(T& us) const
    {
      SetAlertTimer(us, m_alertLength.GetValue());
    }

    void DecrementAlert(T& us) const
    {
      SetAlertTimer(us, GetAlertTimer(us) - 1);

      if(!IsAlert(us))
      {
	Cooldown(us);
      }
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

    void Cooldown(T& us) const
    {
      SetCooldownTimer(us, m_cooldownLength.GetValue());
    }

    void DecrementCooldown(T& us) const
    {
      SetCooldownTimer(us, GetCooldownTimer(us) - 1);
    }

   protected:

    virtual u32 GetMyBreadcrumbType() const = 0;

    bool IsAlert(const T& us) const
    {
      return GetAlertTimer(us) > 0;
    }

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

    void SetIndex(T& us, const u32 age) const
    {
      AFIndex::Write(this->GetBits(us), age);
    }

    T GetMutableAtom(const T& oldMe) const
    {
      T me(oldMe.GetType(), 0, 0, 0);

      SetCooldownTimer(me, GetCooldownTimer(oldMe));
      SetAlertTimer(me, GetAlertTimer(oldMe));
      SetIndex(me, GetIndex(oldMe));

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
      if(IsCooldown(me))
      {
	T mutableMe = GetMutableAtom(window.GetCenterAtom());
	DecrementCooldown(mutableMe);
	window.SetCenterAtom(mutableMe);
      }
      else if(IsAlert(me))
      {
	T mutableMe = GetMutableAtom(window.GetCenterAtom());
	DecrementAlert(mutableMe);
	window.SetCenterAtom(mutableMe);
      }
      else
      {
	MDist<R>& md = MDist<R>::get();

	for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
	{
	  SPoint pt = md.GetPoint(i);

	  if(window.GetRelativeAtom(pt).GetType() ==
	     GetMyBreadcrumbType())
	  {
	    if(IsAlert(window.GetRelativeAtom(pt)))
	    {
	      T mutableMe = GetMutableAtom(window.GetCenterAtom());
	      Alert(mutableMe);
	      window.SetCenterAtom(mutableMe);
	      return;
	    }
	  }

	  if(window.GetRelativeAtom(pt).GetType() == Element_Data<CC>::THE_INSTANCE.GetType())
	  {
	    T mutableMe = GetMutableAtom(window.GetCenterAtom());
	    Alert(mutableMe);
	    window.SetCenterAtom(mutableMe);
	    return;
	  }
	}
      }
    }
  };
}

#endif /* ABSTRACT_ELEMENT_BREADCRUMB_H */
