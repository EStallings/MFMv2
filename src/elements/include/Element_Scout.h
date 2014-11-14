/*                                              -*- mode:C++ -*-
  Element_Scout.h Abstract Tower element for base class
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
  \file   Element_Scout.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_SCOUT_H
#define ELEMENT_SCOUT_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Element_Empty.h"
#include "Element_Breadcrumb.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Element_Scout : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  protected:

    ElementParameterS32<CC> m_defaultHealth;
    ElementParameterS32<CC> m_changeDirectionChance;
    ElementParameterS32<CC> m_stutterChance;
    ElementParameterS32<CC> m_defaultLifeTimer;

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      CURRENT_HEALTH_POS = P3Atom<P>::P3_STATE_BITS_POS,
      CURRENT_HEALTH_LEN = 10,

      CURRENT_DIRECTION_POS = CURRENT_HEALTH_POS + CURRENT_HEALTH_LEN,
      CURRENT_DIRECTION_LEN = 3,

      CURRENT_BREADCRUMB_INDEX_POS = CURRENT_DIRECTION_POS + CURRENT_DIRECTION_LEN,
      CURRENT_BREADCRUMB_INDEX_LEN = 8,

      CURRENT_LIFE_TIMER_POS = CURRENT_BREADCRUMB_INDEX_POS + CURRENT_BREADCRUMB_INDEX_LEN,
      CURRENT_LIFE_TIMER_LEN = 10,

      ID_POS = CURRENT_LIFE_TIMER_POS + CURRENT_LIFE_TIMER_LEN,
      ID_LEN = 10
    };

    typedef BitField<BitVector<BITS>, CURRENT_HEALTH_LEN, CURRENT_HEALTH_POS> AFCurrentHealth;
    typedef BitField<BitVector<BITS>, CURRENT_DIRECTION_LEN, CURRENT_DIRECTION_POS> AFCurrentDirection;
    typedef BitField<BitVector<BITS>, CURRENT_BREADCRUMB_INDEX_LEN, CURRENT_BREADCRUMB_INDEX_POS>
            AFCurrentBreadcrumbIndex;
    typedef BitField<BitVector<BITS>, CURRENT_LIFE_TIMER_LEN, CURRENT_LIFE_TIMER_POS> AFCurrentLifeTimer;
    typedef BitField<BitVector<BITS>, ID_LEN, ID_POS> AFID;

  public:
     static Element_Scout_Red<CC> THE_INSTANCE;

    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Scout_Red()
      : Element<CC>(MFM_UUID_FOR("ScoutXBRed", WAR_VERSION)),
        m_defaultHealth(this, "defaultHealth", "Default Health",
          "This is the health the scout will start with.", 1, 2, 15, 1),
        m_changeDirectionChance(this, "changeDirectionChance", "Change Direction Chance",
          "This is the chance of changing direction in a given tick.", 1, 20, 100, 1),
        m_stutterChance(this, "stutterChance", "Stutter Movement Chance",
          "This is the chance of stuttering movement.", 1, 10, 100,1),
        m_defaultLifeTimer(this, "defaultLifeTimer", "Default Life Timer",
          "This is the natural lifespan of the scout.", 1, 350, 1000, 10)
    {
      Element<CC>::SetAtomicSymbol("Sc");
      Element<CC>::SetName("Red Scout");
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

      Abstract_Element_Scout<CC>::
      SetID(defaultAtom, rand() % (1<<Abstract_Element_Scout<CC>::ID_LEN));

      Abstract_Element_Scout<CC>::
      SetCurrentLifeTimer(defaultAtom, (u32) Abstract_Element_Scout<CC>::m_defaultLifeTimer.GetValue());

      return defaultAtom;
    }

    virtual const Abstract_Element_Breadcrumb<CC>& GetBreadcrumbElement() const
    {
      return Element_Breadcrumb_Red<CC>::THE_INSTANCE;
    }

    virtual const bool IsAtomInteresting(EventWindow<CC>& window, const T& atom) const;

    virtual const char* GetDescription() const
    {
      return "Red Scout element.";
    }

    u32 GetID(const T& us) const
    {
      return AFID::Read(this->GetBits(us));
    }

    void SetID(T& us, const u32 id) const
    {
      AFID::Write(this->GetBits(us), id);
    }

    u32 GetCurrentLifeTimer(const T& us) const
    {
      return AFCurrentLifeTimer::Read(this->GetBits(us));
    }

    void SetCurrentLifeTimer(T& us, const u32 time) const
    {
      AFCurrentLifeTimer::Write(this->GetBits(us), time);
    }

    u32 GetCurrentHealth(const T& us) const
    {
      return AFCurrentHealth::Read(this->GetBits(us));
    }

    void SetCurrentHealth(T& us, const u32 health) const
    {
      AFCurrentHealth::Write(this->GetBits(us), health);
    }

    u32 GetCurrentDirection(const T& us) const
    {
      return AFCurrentDirection::Read(this->GetBits(us));
    }

    void SetCurrentDirection(T& us, const u32 direction) const
    {
      AFCurrentDirection::Write(this->GetBits(us), direction);
    }

    u32 GetCurrentBreadcrumbIndex(const T& us) const
    {
      return AFCurrentBreadcrumbIndex::Read(this->GetBits(us));
    }

    void SetCurrentBreadcrumbIndex(T& us, const u32 i) const
    {
      AFCurrentBreadcrumbIndex::Write(this->GetBits(us), i);
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 100;
    }

    T GetMutableAtom(const T& oldMe) const
    {
      T me = oldMe;

      return me;
    }

    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& constSelf = window.GetCenterAtom();
      T self = GetMutableAtom(constSelf);
      Random& rand = window.GetRandom();

      //Reduce life timer
      SetCurrentLifeTimer(self, GetCurrentLifeTimer(self)-1);

      //If out of health, or at the end of natural lifespan, die
      if(GetCurrentHealth(self) <= 0 || GetCurrentLifeTimer(self) <= 0){
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        return;
      }
      window.SetCenterAtom(self);

      //Scan event window for points of interest; if one spotted, replace self with alerted breadcrumb
      //Do this ONLY if we've existed for a while.
      if(GetCurrentLifeTimer(self) < (u32)(m_defaultLifeTimer.GetValue() - 10))
      {
        MDist<R>& md = MDist<R>::get();
        for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
        {
          SPoint pt = md.GetPoint(i);
          
          if(IsAtomInteresting(window, window.GetRelativeAtom(pt)))
          {
            const Element_Breadcrumb<CC>& bcClass = GetBreadcrumbElement();
            T bc = bcClass.GetMutableAtom(bcClass.GetDefaultAtom());
            bcClass.SetIndex(bc, GetCurrentBreadcrumbIndex(self));
            bcClass.SetPrevIndex(bc, GetCurrentBreadcrumbIndex(self)-1);
            bcClass.Alert(bc);
            window.SetCenterAtom(bc);
            return;
          }
        }
      }

      //Randomly change direction & stutter movement
      if(rand.OneIn(m_changeDirectionChance.GetValue())){
        SetCurrentDirection(self, rand.Create(8));
      }

      Dir curDirection = (Dir)GetCurrentDirection(self);
      
      if(rand.OneIn(m_stutterChance.GetValue())){
        curDirection = (Dir)rand.Create(8);
      }

      //Move, leaving a breadcrumb behind
      window.SetCenterAtom(self);
      SPoint vec;
      Dirs::FillDir(vec, curDirection);
      u32 speed = Dirs::IsCorner(curDirection) ? 1 : 2;
      
      for(u32 it = 0; it < 2; it++){
        vec *= (speed-it);
        

        if(window.IsLiveSite(vec) &&
          window.GetRelativeAtom(vec).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
        {
        	const Element_Breadcrumb<CC>& bcClass = GetBreadcrumbElement();
        	T bc = bcClass.GetMutableAtom(bcClass.GetDefaultAtom());
        	bcClass.SetIndex(bc,     GetCurrentBreadcrumbIndex(self));
          bcClass.SetPrevIndex(bc, GetCurrentBreadcrumbIndex(self)-1);
          bcClass.SetNextIndex(bc, GetCurrentBreadcrumbIndex(self)+1);
        	SetCurrentBreadcrumbIndex(self, GetCurrentBreadcrumbIndex(self) + 1);
          bcClass.Cooldown(bc);
        	window.SetCenterAtom(self);

        	window.SetRelativeAtom(vec, bc);
        	window.SwapAtoms(vec, SPoint(0,0));
          return;
        }
      }
    }
  };
}

#endif /* ELEMENT_SCOUT_H */
