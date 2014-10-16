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
    ElementParameterS32<CC> m_defaultLifeTimer;    

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      CURRENT_HEALTH_POS = P3Atom<P>::P3_STATE_BITS_POS,
      CURRENT_HEALTH_LEN = 10,

      CURRENT_LIFE_TIMER_POS = CURRENT_HEALTH_POS + CURRENT_HEALTH_LEN,
      CURRENT_LIFE_TIMER_LEN = 10
    };

    typedef BitField<BitVector<BITS>, CURRENT_HEALTH_LEN, CURRENT_HEALTH_POS> AFCurrentHealth;
    typedef BitField<BitVector<BITS>, CURRENT_LIFE_TIMER_LEN, CURRENT_LIFE_TIMER_POS> AFCurrentLifeTimer;

  public:

    Abstract_Element_Soldier(UUID u)
      : Element<CC>(u),
        m_defaultHealth(this, "defaultHealth", "Default Health",
                  "This is the health the soldier will start with.", 1, 200, 1000, 10),
        m_attackDamage(this, "attackDamage", "Attack Damage",
		            "This is the damage done in a single tick.", 1, 5, 100, 5),
        m_defaultLifeTimer(this, "defaultLifeTimer", "Default Life Timer",
                "This is the natural lifespan of the soldier.", 1, 250, 1000, 10)
    {}

    u32 GetCurrentHealth(const T& us) const
    {
      return AFCurrentHealth::Read(this->GetBits(us));
    }

    void SetCurrentHealth(T& us, const u32 health) const
    {
      AFCurrentHealth::Write(this->GetBits(us), health);
    }

    u32 GetCurrentLifeTimer(const T& us) const
    {
      return AFCurrentLifeTimer::Read(this->GetBits(us));
    }

    void SetCurrentLifeTimer(T& us, const u32 time) const
    {
      AFCurrentLifeTimer::Write(this->GetBits(us), time);
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

    virtual const Element<CC>* GetScoutElement()      const = 0;
    virtual const Element<CC>* GetTowerElement()      const = 0;
    virtual const Element<CC>* GetColonistElement()   const = 0;
    virtual const Element<CC>* GetBreadcrumbElement() const = 0;

    //Returns true if the attack was successful
    virtual const bool AttemptAttack(EventWindow<CC>& window, SPoint location) const = 0;

    virtual const bool IsMyBreadcrumbType(const u32 type) const = 0;
    virtual const bool IsMyBreadcrumbAlerted(const T& bc) const = 0;
    virtual const u32 GetMyBreadcrumbIndex(const T& bc) const = 0;

    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& constSelf = window.GetCenterAtom();
      T self = GetMutableAtom(constSelf);
      //Random& rand = window.GetRandom();

      //Reduce life timer
      SetCurrentLifeTimer(self, GetCurrentLifeTimer(self)-1);

      //If out of health, or at the end of natural lifespan, die
      if(GetCurrentHealth(self) <= 0 || GetCurrentLifeTimer(self) <= 0){
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        return;
      }
      window.SetCenterAtom(self);

      //Look through event window, and:
      // count enemy atoms nearby, and select a random one if applicable
      // count nearby breadcrumbs, and select the highest-indexed one if applicable
      // count nearby empty spaces, and select the one nearest a high-indexed breadcrumb
      
      SPoint emptyLocation, breadcrumbLocation;
      u32 curIndex = 0;
      bool foundBreadcrumb = false;
      // u32 enemyCount = 0;
      MDist<R> n = MDist<R>::get();

      for(u32 i = n.GetFirstIndex(1); i <= n.GetLastIndex(R); i++)
      {
        SPoint searchLoc = n.GetPoint(i);         
        //Enemy identification TODO

        if(AttemptAttack(window, searchLoc)){
          return;
        }


        //Breadcrumb identification TODO
        if( IsMyBreadcrumbType( window.GetRelativeAtom(searchLoc).GetType() ) )
        {
          //LOG.Debug("Found Breadcrumb");
          if(IsMyBreadcrumbAlerted(window.GetRelativeAtom(searchLoc)))
          {

            u32 testIndex = GetMyBreadcrumbIndex(window.GetRelativeAtom(searchLoc));
            //LOG.Debug("And its alerted! index=%d", testIndex);
            if(testIndex >= curIndex)
            {
              //LOG.Debug("And we should be going towards it!");
              breadcrumbLocation = searchLoc;
              curIndex = testIndex;
              foundBreadcrumb = true;
            }
          }
        }
      }
      //LOG.Debug("CurIndex: %d", curIndex);

      //Find the nearest empty spot
      if(foundBreadcrumb){
        for(u32 i = n.GetFirstIndex(1); i <= n.GetLastIndex(R); i++)
        {
          SPoint searchLoc = n.GetPoint(i)+breadcrumbLocation;
          if(searchLoc.GetManhattanLength() < R)
          {
            if(window.GetRelativeAtom(searchLoc).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
            {
              LOG.Debug("Next Position:%d,%d", searchLoc.GetX(), searchLoc.GetY());
            
              window.SwapAtoms(SPoint(0,0), searchLoc);
              return;
            }
          } 
        }
      }
    }
  };
}


#endif /* ABSTRACT_ELEMENT_SOLDIER_H */
