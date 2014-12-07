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
			if(this->GetActive(window.GetCenterAtom()) > 0){
				T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
				this->SetActive(mutableMe, this->GetActive(window.GetCenterAtom()) - 1);
				this->SetIsEndpoint(mutableMe, 0);
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
					if(this->GetIndex(window.GetRelativeAtom(pt)) == this->GetPrevIndex(window.GetCenterAtom()) && this->GetPathID(window.GetRelativeAtom(pt)) == this->GetPathID(window.GetCenterAtom()))
					{
						pred = pt;
						fP = true;
					}
					else if(this->GetIndex(window.GetRelativeAtom(pt)) == this->GetNextIndex(window.GetCenterAtom()) && this->GetPathID(window.GetRelativeAtom(pt)) == this->GetPathID(window.GetCenterAtom()))
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
				if(!this->GetIsEndpoint(window.GetCenterAtom())){
					T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
					this->SetIsEndpoint(mutableMe, 1);
					window.SetCenterAtom(mutableMe);
				}
				if(!fP)
				{
					pred = towerPos; //for positioning
					T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
					this->SetUpstreamLocalDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetLocalDemand(window.GetRelativeAtom(pred)));
					this->SetUpstreamDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetCurrentDemand(window.GetRelativeAtom(pred)));
					this->SetEndpointSwitch(mutableMe, 0);
					window.SetCenterAtom(mutableMe);
				}
				else if(!fS)
				{
					succ = towerPos; //for positioning
					T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
					this->SetDownstreamLocalDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetLocalDemand(window.GetRelativeAtom(succ)));
					this->SetDownstreamDemand(mutableMe, Element_Tower_Red<CC>::THE_INSTANCE.GetCurrentDemand(window.GetRelativeAtom(succ)));
					this->SetEndpointSwitch(mutableMe, 1);
					window.SetCenterAtom(mutableMe);
				}
			}

			//Otherwise, move res
			else if(fR)
			{

				SPoint goal = succ;
				if(this->GetTrafficDir(window.GetCenterAtom()) == 1)
					goal = pred;
				for(u32 i = md.GetFirstIndex(1); i <= md.GetLastIndex(R); i++)
				{
					SPoint pt = md.GetPoint(i);
					if(window.GetRelativeAtom(pt).GetType() == resType)
					{
						u32 minDist = 16;
						SPoint minPos = pt;
						for(u32 j = md.GetFirstIndex(1); j <= md.GetLastIndex(R); j++)
						{
							SPoint pt2 = md.GetPoint(j);
							if(window.GetRelativeAtom(pt2).GetType() == Element_Empty<CC>::THE_INSTANCE.GetType())
							{
								u32 dist = (pt2-goal).GetManhattanLength();
								if(dist < minDist)
								{
									minPos = pt2;
								}
								//LOG.Debug("Moving res a distance of %d from goal!", dist);
							}
						}
						window.SwapAtoms(minPos, pt);
					}
				}
			}

			//get pred's data
			if(fP)
			{
				T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
				this->SetUpstreamLocalDemand(mutableMe, this->GetUpstreamLocalDemand(window.GetRelativeAtom(pred)));
				this->SetUpstreamDemand(mutableMe, this->GetUpstreamDemand(window.GetRelativeAtom(pred)));
				window.SetCenterAtom(mutableMe);
			}

			//get succ's data
			if(fS)
			{
				T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
				this->SetDownstreamLocalDemand(mutableMe, this->GetDownstreamLocalDemand(window.GetRelativeAtom(succ)));
				this->SetDownstreamDemand(mutableMe, this->GetDownstreamDemand(window.GetRelativeAtom(succ)));
				window.SetCenterAtom(mutableMe);
			}


			// LOG.Debug("Breadcrumb %d|%d has UP=%d, DOWN=%d, Dir=%d", GetIndex(window.GetCenterAtom()), GetPathID(window.GetCenterAtom()), GetUpstreamLocalDemand(window.GetCenterAtom()), GetDownstreamLocalDemand(window.GetCenterAtom()), GetTrafficDir(window.GetCenterAtom()));
			//modify traffic direction
			if(this->GetUpstreamLocalDemand(window.GetCenterAtom()) > this->GetDownstreamLocalDemand(window.GetCenterAtom()) && this->GetTrafficDir(window.GetCenterAtom()) == 0)
			{
				T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
				this->SetTrafficDir(mutableMe, 1);
				window.SetCenterAtom(mutableMe);
			}

			else if(this->GetUpstreamLocalDemand(window.GetCenterAtom()) < this->GetDownstreamLocalDemand(window.GetCenterAtom()) && this->GetTrafficDir(window.GetCenterAtom()) == 1)
			{
				T mutableMe = this->GetMutableAtom(window.GetCenterAtom());
				this->SetTrafficDir(mutableMe, 0);
				window.SetCenterAtom(mutableMe);
			}


			//Move to average position
			if((pred-succ).GetManhattanLength() < R)
			{

				//LOG.Debug("Deleting BC:%d; Pred=%d, Succ=%d", GetIndex(window.GetCenterAtom()), GetPrevIndex(window.GetCenterAtom()), GetNextIndex(window.GetCenterAtom()));

				T predAtom = this->GetMutableAtom(window.GetRelativeAtom(pred));
				this->SetNextIndex(predAtom, this->GetNextIndex(window.GetCenterAtom()));
				window.SetRelativeAtom(pred, predAtom);

				T succAtom = this->GetMutableAtom(window.GetRelativeAtom(succ));
				this->SetPrevIndex(succAtom, this->GetPrevIndex(window.GetCenterAtom()));
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
