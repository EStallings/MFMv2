/*                                              -*- mode:C++ -*-
  Abstract_Element_Colonist.h Abstract Tower element for base class
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
  \file   Abstract_Element_Colonist.h Abstract Tower element for base class
  \author Trent R. Small.
  \author Ezra Stallings
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ABSTRACT_ELEMENT_COLONIST_H
#define ABSTRACT_ELEMENT_COLONIST_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"
#include "Element_Empty.h"

namespace MFM
{

  #define WAR_VERSION 1

  template <class CC>
  class Abstract_Element_Colonist : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  protected:

    ElementParameterS32<CC> m_defaultHealth;
    ElementParameterS32<CC> m_changeDirectionChance;
    ElementParameterS32<CC> m_stutterChance;
    ElementParameterS32<CC> m_towerChance;

    enum
    {
      BITS = P::BITS_PER_ATOM,

      //////
      // Element state fields

      CURRENT_HEALTH_POS = P3Atom<P>::P3_STATE_BITS_POS,
      CURRENT_HEALTH_LEN = 10,

      CURRENT_DIRECTION_POS = CURRENT_HEALTH_POS + CURRENT_HEALTH_LEN,
      CURRENT_DIRECTION_LEN = 3,

      TOWER_CHANCE_POS = CURRENT_DIRECTION_POS + CURRENT_DIRECTION_LEN,
      TOWER_CHANCE_LEN = 8
    };

    typedef BitField<BitVector<BITS>, CURRENT_HEALTH_LEN, CURRENT_HEALTH_POS> AFCurrentHealth;
    typedef BitField<BitVector<BITS>, CURRENT_DIRECTION_LEN, CURRENT_DIRECTION_POS> AFCurrentDirection;
    typedef BitField<BitVector<BITS>, TOWER_CHANCE_LEN, TOWER_CHANCE_POS> AFTowerChance;

  public:

    Abstract_Element_Colonist(UUID u)
      : Element<CC>(u),
        m_defaultHealth(this, "defaultHealth", "Default Health",
                  "This is the health the colonist will start with.", 1, 200, 1000, 10),
        m_changeDirectionChance(this, "changeDirectionChance", "Change Direction Chance",
		            "This is the chance of changing direction in a given tick.", 1, 90, 100, 1),
      	m_stutterChance(this, "stutterChance", "Stutter Movement Chance",
      			    "This is the chance of stuttering movement.", 1, 50, 100,1),
        m_towerChance(this, "towerChance", "Tower Placement Chance",

                "This is the chance of making a tower, decreases over time.", 50,1000,200,10)
    {}

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

    u32 GetTowerChance(const T& us) const
    {
      return AFTowerChance::Read(this->GetBits(us));
    }

    void SetTowerChance(T& us, const u32 i) const
    {
      AFTowerChance::Write(this->GetBits(us), i);
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 100;
    }

    T GetMutableMe(const T& me) const
    {
      T newMe(me.GetType(), 0, 0, 0);

      SetCurrentHealth(newMe, GetCurrentHealth(me));
      SetCurrentDirection(newMe, GetCurrentDirection(me));
      SetTowerChance(newMe, GetTowerChance(me));

      return newMe;
    }

    virtual const T& GetDefaultTower() const = 0;

    virtual void Behavior(EventWindow<CC>& window) const
    {
      const T& constSelf = window.GetCenterAtom();
      T self = GetMutableMe(constSelf);
      Random& rand = window.GetRandom();

      //If out of health, die
      if(GetCurrentHealth(self) <= 0){
        window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
        return;
      }

      //Reduce tower chance
      SetTowerChance(self, GetTowerChance(self) - 1);

      //Possibly turn into tower
      if(rand.OneIn(GetTowerChance(self)))
      {
        window.SetCenterAtom(GetDefaultTower());
        return;
      }

      //Randomly change direction & stutter movement
      if(rand.OneIn(m_changeDirectionChance.GetValue())){
        SetCurrentDirection(self, rand.Create(8));
      }
      Dir curDirection = (Dir)GetCurrentDirection(self);
      
      if(rand.OneIn(m_stutterChance.GetValue())){
        curDirection = (Dir)rand.Create(8);
      }

      //Move
      window.SetCenterAtom(self);

      SPoint vec;
      Dirs::FillDir(vec, curDirection);
      u32 speed = Dirs::IsCorner(curDirection) ? 2 : 4;
      
      for(u32 it = 0; it < 2; it++){
        vec *= (speed-it);
        

        if(window.IsLiveSite(vec) &&
          window.GetRelativeAtom(vec).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
        {
        	window.SwapAtoms(vec, SPoint(0,0));
          return;
        }
      }
    }
  };
}

#endif /* ABSTRACT_ELEMENT_COLONIST_H */
