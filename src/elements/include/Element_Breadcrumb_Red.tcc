	/* -*- C++ -*- */

	#include "Element_Tower_Red.h"
	#include "Element_Empty.h"
	#include "Element_Res.h"
	namespace MFM
	{
		template <class CC>
		void Element_Breadcrumb_Red<CC>::Behavior(EventWindow<CC>& window) const
		{
			Random & random = window.GetRandom();

			//Delayed start to behavior to ensure everything gets linked!
			if(GetActive(window.GetCenterAtom()) > 0){
				T mutableMe = GetMutableAtom(window.GetCenterAtom());
				SetActive(mutableMe, GetActive(window.GetCenterAtom()) - 1);
				SetIsEndpoint(mutableMe, 0);	
				window.SetCenterAtom(mutableMe);
				return;
			}

			//LOG.Debug("Examining BC:%d; Pred=%d, Succ=%d, timer=%d", GetIndex(window.GetCenterAtom()), GetPrevIndex(window.GetCenterAtom()), GetNextIndex(window.GetCenterAtom()), GetActive(window.GetCenterAtom()));

			MDist<R>& md = MDist<R>::get();
			SPoint pred, succ;
			bool fP = false, fS = false;

			u32 numTowers = 0;
			SPoint towerPos;
			bool fT = false;

			u32 resType = Element_Res<CC>::THE_INSTANCE.GetType();
			SPoint resPos;
			u32 numRes = 0;
			bool fR = false;


			//Find predecessor and successor, and a random valid tower if possible
			for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
			{
				SPoint pt = md.GetPoint(i);

				if(window.GetRelativeAtom(pt).GetType() == Element_Breadcrumb_Red<CC>::THE_INSTANCE.GetType())
				{
					if(GetIndex(window.GetRelativeAtom(pt)) == GetPrevIndex(window.GetCenterAtom()) && GetPathID(window.GetRelativeAtom(pt)) == GetPathID(window.GetCenterAtom()))
					{
						pred = pt;
						fP = true;
					}
					else if(GetIndex(window.GetRelativeAtom(pt)) == GetNextIndex(window.GetCenterAtom()) && GetPathID(window.GetRelativeAtom(pt)) == GetPathID(window.GetCenterAtom()))
					{
						succ = pt;
						fS = true;
					}
				}
				else if(window.GetRelativeAtom(pt).GetType() == Element_Tower_Red<CC>::THE_INSTANCE.GetType())
				{
					numTowers++;
					fT = true;
					if(random.OneIn(numTowers)){
					towerPos = pt;
					}
				}
				else if(window.GetRelativeAtom(pt).GetType() == resType)
				{
					numRes++;
					fR = true;
					if(random.OneIn(numRes)){
					resPos = pt;
					}
				}
			}

			//if disconnected, die
			if(!(fP && fS) && !(fT && (fS || fP))) {
				window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
				return;
			}

			//logic if we need to interact with a tower
			if(!fP || !fS){
				if(!GetIsEndpoint(window.GetCenterAtom())){
					T mutableMe = GetMutableAtom(window.GetCenterAtom());
					SetIsEndpoint(mutableMe, 1);	
					window.SetCenterAtom(mutableMe);
				}
				if(!fP)
				{
					pred = towerPos; //for positioning
					T mutableMe = GetMutableAtom(window.GetCenterAtom());
					SetUpstreamLocalDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetLocalDemand(window.GetRelativeAtom(pred)));
					SetUpstreamDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetCurrentDemand(window.GetRelativeAtom(pred)));
					SetEndpointSwitch(mutableMe, 0);
					window.SetCenterAtom(mutableMe);
				}
				else if(!fS)
				{
					succ = towerPos; //for positioning
					T mutableMe = GetMutableAtom(window.GetCenterAtom());
					SetDownstreamLocalDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetLocalDemand(window.GetRelativeAtom(succ)));
					SetDownstreamDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetCurrentDemand(window.GetRelativeAtom(succ)));
					SetEndpointSwitch(mutableMe, 1);
					window.SetCenterAtom(mutableMe);
				}
			}

			//Otherwise, move res
			else if(fR)
			{
				SPoint goal = succ;
				if(GetTrafficDir(window.GetCenterAtom()) == 1)
					goal = pred;
				u32 minDist = 16;
				SPoint minPos = resPos;
				for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
				{
					SPoint pt = md.GetPoint(i);
					if(window.GetRelativeAtom(pt).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
					{
						u32 dist = (pt-goal).GetManhattanLength();
						if(dist < minDist)
						{
							minPos = pt;
						}
						//LOG.Debug("Moving res a distance of %d from goal!", dist);
					}
				}
				window.SwapAtoms(minPos, resPos);
			}

			//get pred's data
			if(fP)
			{
				T mutableMe = GetMutableAtom(window.GetCenterAtom());
				SetUpstreamLocalDemand(mutableMe, GetUpstreamLocalDemand(window.GetRelativeAtom(pred)));
				SetUpstreamDemand(mutableMe, GetUpstreamDemand(window.GetRelativeAtom(pred)));
				window.SetCenterAtom(mutableMe);
			}

			//get succ's data
			if(fS)
			{
				T mutableMe = GetMutableAtom(window.GetCenterAtom());
				SetDownstreamLocalDemand(mutableMe, GetDownstreamLocalDemand(window.GetRelativeAtom(succ)));
				SetDownstreamDemand(mutableMe, GetDownstreamDemand(window.GetRelativeAtom(succ)));
				window.SetCenterAtom(mutableMe);
			}


			// LOG.Debug("Breadcrumb %d|%d has UP=%d, DOWN=%d, Dir=%d", GetIndex(window.GetCenterAtom()), GetPathID(window.GetCenterAtom()), GetUpstreamLocalDemand(window.GetCenterAtom()), GetDownstreamLocalDemand(window.GetCenterAtom()), GetTrafficDir(window.GetCenterAtom()));
			//modify traffic direction
			if(GetUpstreamLocalDemand(window.GetCenterAtom()) > GetDownstreamLocalDemand(window.GetCenterAtom()) && GetTrafficDir(window.GetCenterAtom()) == 0)
			{
				T mutableMe = GetMutableAtom(window.GetCenterAtom());
				SetTrafficDir(mutableMe, 1);
				window.SetCenterAtom(mutableMe);
			}

			else if(GetUpstreamLocalDemand(window.GetCenterAtom()) < GetDownstreamLocalDemand(window.GetCenterAtom()) && GetTrafficDir(window.GetCenterAtom()) == 1)
			{
				T mutableMe = GetMutableAtom(window.GetCenterAtom());
				SetTrafficDir(mutableMe, 0);
				window.SetCenterAtom(mutableMe);
			}


			//Move to average position
			if((pred-succ).GetManhattanLength() < R)
			{

				//LOG.Debug("Deleting BC:%d; Pred=%d, Succ=%d", GetIndex(window.GetCenterAtom()), GetPrevIndex(window.GetCenterAtom()), GetNextIndex(window.GetCenterAtom()));

				T predAtom = GetMutableAtom(window.GetRelativeAtom(pred));
				SetNextIndex(predAtom, GetNextIndex(window.GetCenterAtom()));
				window.SetRelativeAtom(pred, predAtom);

				T succAtom = GetMutableAtom(window.GetRelativeAtom(succ));
				SetPrevIndex(succAtom, GetPrevIndex(window.GetCenterAtom()));
				window.SetRelativeAtom(succ, succAtom);

				window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
			}
			else
			{
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
	