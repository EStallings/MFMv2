	/* -*- C++ -*- */

	#include "Element_Tower.h"
	#include "Element_Colonist.h"
	#include "Element_Scout.h"
	#include "Element_Breadcrumb.h"

	namespace MFM
	{
		template <class CC>
		const Element<CC>* Element_Soldier<CC>::GetScoutElement()      const
		{
			return &(Element_Scout<CC>::THE_INSTANCE);
		}
	    template <class CC>
	    const Element<CC>* Element_Soldier<CC>::GetTowerElement()      const
	    {
	    	return &(Element_Tower<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier<CC>::GetColonistElement()   const
	    {
	    	return &(Element_Colonist<CC>::THE_INSTANCE);
	    }
	    template <class CC>
	    const Element<CC>* Element_Soldier<CC>::GetBreadcrumbElement() const
	    {
	    	return &(Element_Breadcrumb<CC>::THE_INSTANCE);
	    }
		
	    template <class CC>
		const bool Element_Soldier<CC>::IsMyBreadcrumbType(const u32 type) const
		{
			return type == Element_Breadcrumb<CC>::THE_INSTANCE.GetType();
		}

		template <class CC>
		const bool Element_Soldier<CC>::IsMyBreadcrumbAlerted(const T& bc) const
	    {
	      	return Element_Breadcrumb<CC>::THE_INSTANCE.IsAlert(bc);
	    }

	    template <class CC>
	    const u32 Element_Soldier<CC>::GetMyBreadcrumbIndex(const T& bc) const
	    {
	      	return Element_Breadcrumb<CC>::THE_INSTANCE.GetIndex(bc);
	    }

	    template<class CC>
	    const bool Element_Soldier<CC>::AttemptAttack(EventWindow<CC>& window, SPoint location) const
	    {
	    	const T& atom = window.GetRelativeAtom(location);
	    	const u32 type = atom.GetType();
	    	const Element<CC> * elt = window.GetTile().GetElement(type);
	    	u32 damage = Element_Soldier<CC>::m_attackDamage.GetValue();

	    	//check against tower
	    	if( (dynamic_cast<const Abstract_Element_Tower<CC>*>(elt)) )
	    	{
	    		if(Element_Tower<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T tower = Element_Tower<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Tower<CC>::THE_INSTANCE.SetCurrentHealth(tower, Element_Tower<CC>::THE_INSTANCE.GetCurrentHealth(tower)-damage);
	    			window.SetRelativeAtom(location, tower);

	    			return true;
	    		}
	    	}

	    	//check against colonist
	    	if( (dynamic_cast<const Abstract_Element_Colonist<CC>*>(elt)) )
	    	{
	    		if(Element_Colonist<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T colonist = Element_Colonist<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Colonist<CC>::THE_INSTANCE.SetCurrentHealth(colonist, Element_Colonist<CC>::THE_INSTANCE.GetCurrentHealth(colonist)-damage);
	    			window.SetRelativeAtom(location, colonist);

	    			return true;
	    		}
	    	}

	    	//check against soldier
	    	if( (dynamic_cast<const Abstract_Element_Soldier<CC>*>(elt)) )
	    	{
	    		if(Element_Soldier<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T soldier = Element_Soldier<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Soldier<CC>::THE_INSTANCE.SetCurrentHealth(soldier, Element_Soldier<CC>::THE_INSTANCE.GetCurrentHealth(soldier)-damage);
	    			window.SetRelativeAtom(location, soldier);

	    			return true;
	    		}
	    	}

	    	//check against scout
	    	if( (dynamic_cast<const Abstract_Element_Scout<CC>*>(elt)) )
	    	{
	    		if(Element_Scout<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			T scout = Element_Scout<CC>::THE_INSTANCE.GetMutableAtom(atom);
	    			Element_Scout<CC>::THE_INSTANCE.SetCurrentHealth(scout, Element_Scout<CC>::THE_INSTANCE.GetCurrentHealth(scout)-damage);
	    			window.SetRelativeAtom(location, scout);

	    			return true;
	    		}
	    	}

	    	return false;
	    }
	}
    