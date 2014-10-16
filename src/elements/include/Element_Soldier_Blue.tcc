	/* -*- C++ -*- */

	#include "Element_Tower_Blue.h"
	#include "Element_Colonist_Blue.h"
	#include "Element_Scout_Blue.h"
	#include "Element_Breadcrumb_Blue.h"

	namespace MFM
	{
		template <class CC>
		const Element<CC>* Element_Soldier_Blue<CC>::GetScoutElement()      const
		{
			return &(Element_Scout_Blue<CC>::THE_INSTANCE);
		}
	    template <class CC>
	    const Element<CC>* Element_Soldier_Blue<CC>::GetTowerElement()      const
	    {
	    	return &(Element_Tower_Blue<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier_Blue<CC>::GetColonistElement()   const
	    {
	    	return &(Element_Colonist_Blue<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier_Blue<CC>::GetBreadcrumbElement() const
	    {
	    	return &(Element_Breadcrumb_Blue<CC>::THE_INSTANCE);
	    }
		
	    template <class CC>
		const bool Element_Soldier_Blue<CC>::IsMyBreadcrumbType(const u32 type) const
		{
			return type == Element_Breadcrumb_Blue<CC>::THE_INSTANCE.GetType();
		}

		template <class CC>
		const bool Element_Soldier_Blue<CC>::IsMyBreadcrumbAlerted(const T& bc) const
	    {
	      	return Element_Breadcrumb_Blue<CC>::THE_INSTANCE.IsAlert(bc);
	    }

	    template <class CC>
	    const u32 Element_Soldier_Blue<CC>::GetMyBreadcrumbIndex(const T& bc) const
	    {
	      	return Element_Breadcrumb_Blue<CC>::THE_INSTANCE.GetIndex(bc);
	    }

	    template<class CC>
	    const bool Element_Soldier_Blue<CC>::AttemptAttack(EventWindow<CC>& window, SPoint location) const
	    {
	    	const T& atom = window.GetRelativeAtom(location);
	    	const u32 type = atom.GetType();
	    	const Element<CC> * elt = window.GetTile().GetElement(type);
	    	u32 damage = Element_Soldier_Blue<CC>::m_attackDamage.GetValue();

	    	//check against tower
	    	if( (dynamic_cast<const Abstract_Element_Tower<CC>*>(elt)) )
	    	{
	    		if(Element_Tower_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T tower = Element_Tower_Blue<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Tower_Blue<CC>::THE_INSTANCE.SetCurrentHealth(tower, Element_Tower_Blue<CC>::THE_INSTANCE.GetCurrentHealth(tower)-damage);
	    			window.SetRelativeAtom(location, tower);

	    			return true;
	    		}
	    	}

	    	//check against colonist
	    	if( (dynamic_cast<const Abstract_Element_Colonist<CC>*>(elt)) )
	    	{
	    		if(Element_Colonist_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T colonist = Element_Colonist_Blue<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Colonist_Blue<CC>::THE_INSTANCE.SetCurrentHealth(colonist, Element_Colonist_Blue<CC>::THE_INSTANCE.GetCurrentHealth(colonist)-damage);
	    			window.SetRelativeAtom(location, colonist);

	    			return true;
	    		}
	    	}

	    	//check against soldier
	    	if( (dynamic_cast<const Abstract_Element_Soldier<CC>*>(elt)) )
	    	{
	    		if(Element_Soldier_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T soldier = Element_Soldier_Blue<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Soldier_Blue<CC>::THE_INSTANCE.SetCurrentHealth(soldier, Element_Soldier_Blue<CC>::THE_INSTANCE.GetCurrentHealth(soldier)-damage);
	    			window.SetRelativeAtom(location, soldier);

	    			return true;
	    		}
	    	}

	    	//check against scout
	    	if( (dynamic_cast<const Abstract_Element_Scout<CC>*>(elt)) )
	    	{
	    		if(Element_Scout_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T scout = Element_Scout_Blue<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Scout_Blue<CC>::THE_INSTANCE.SetCurrentHealth(scout, Element_Scout_Blue<CC>::THE_INSTANCE.GetCurrentHealth(scout)-damage);
	    			window.SetRelativeAtom(location, scout);

	    			return true;
	    		}
	    	}

	    	return false;
	    }
	}
    