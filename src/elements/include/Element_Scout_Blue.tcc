	/* -*- C++ -*- */

	#include "Element_Tower_Blue.h"
	#include "Element_Colonist_Blue.h"
	#include "Element_Scout_Blue.h"
	#include "Element_Soldier_Blue.h"

	namespace MFM
	{
	    template <class CC>
	    const bool Element_Scout_Blue<CC>::IsAtomEnemy(EventWindow<CC>& window, const T& atom) const
	    {
	    	const u32 type = atom.GetType();
	    	const Element<CC> * elt = window.GetTile().GetElement(type);

	    	//check against tower
	    	if( (dynamic_cast<const Abstract_Element_Tower<CC>*>(elt)) )
	    	{
	    		if(Element_Tower_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			return true;
	    		}
	    	}

	    	//check against colonist
	    	if( (dynamic_cast<const Abstract_Element_Colonist<CC>*>(elt)) )
	    	{
	    		if(Element_Colonist_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			return true;
	    		}
	    	}

	    	//check against soldier
	    	if( (dynamic_cast<const Abstract_Element_Soldier<CC>*>(elt)) )
	    	{
	    		if(Element_Soldier_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			return true;
	    		}
	    	}

	    	//check against scout
	    	if( (dynamic_cast<const Abstract_Element_Scout<CC>*>(elt)) )
	    	{
	    		if(Element_Scout_Blue<CC>::THE_INSTANCE.GetType() != type)
	    		{
	    			return true;
	    		}
	    	}

	    	return false;
	    }


	}
    