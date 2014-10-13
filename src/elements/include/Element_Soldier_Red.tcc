	/* -*- C++ -*- */

	#include "Element_Tower_Red.h"
	#include "Element_Colonist_Red.h"
	#include "Element_Scout_Red.h"
	#include "Element_Breadcrumb_Red.h"

	namespace MFM
	{
		template <class CC>
		const Element<CC>* Element_Soldier_Red<CC>::GetScoutElement()      const
		{
			return &(Element_Scout_Red<CC>::THE_INSTANCE);
		}
	    template <class CC>
	    const Element<CC>* Element_Soldier_Red<CC>::GetTowerElement()      const
	    {
	    	return &(Element_Tower_Red<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier_Red<CC>::GetColonistElement()   const
	    {
	    	return &(Element_Colonist_Red<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier_Red<CC>::GetBreadcrumbElement() const
	    {
	    	return &(Element_Breadcrumb_Red<CC>::THE_INSTANCE);
	    }
	}
    