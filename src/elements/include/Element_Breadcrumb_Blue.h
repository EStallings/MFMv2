/*                                              -*- mode:C++ -*-
  Element_Breadcrumb_Blue.h An element that acts similarly to a Wa-Tor fish
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can Blueistribute it and/or
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
  \file Element_Breadcrumb_Blue.h An element that acts similarly to a Wa-Tor fish
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_BREADCRUMB_BLUE_H
#define ELEMENT_BREADCRUMB_BLUE_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "Atom.h"
#include "Abstract_Element_Breadcrumb.h"

namespace MFM
{

  template <class CC>
  class Element_Breadcrumb_Blue : public Abstract_Element_Breadcrumb<CC>
  {
    // Short names for params
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum
    {
      R = P::EVENT_WINDOW_RADIUS
    };

   public:

    static Element_Breadcrumb_Blue THE_INSTANCE;
    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      return defaultAtom;
    }

    Element_Breadcrumb_Blue() :
      Abstract_Element_Breadcrumb<CC>(MFM_UUID_FOR("BreadcrumbBlue", 1))
    {
      Element<CC>::SetAtomicSymbol("Bb");
      Element<CC>::SetName("BreadcrumbBlue");
    }

    virtual u32 LocalPhysicsColor(const T& atom, u32 selector) const
    {
      if(Abstract_Element_Breadcrumb<CC>::IsAlert(atom))
      {
	       return 0x7f7fff;
      }
      else
      {
	return DefaultPhysicsColor();
      }
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff4040ff;
    }

    // Non-diffusable
    virtual u32 Diffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return nowAt.Equals(maybeAt)?Element<CC>::COMPLETE_DIFFUSABILITY:0;
    }

    virtual u32 GetMyBreadcrumbType() const
    {
      return TYPE();
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }
  };

  template <class CC>
  Element_Breadcrumb_Blue<CC> Element_Breadcrumb_Blue<CC>::THE_INSTANCE;

}

#endif /* ELEMENT_BREADCRUMB_BLUE_H */
