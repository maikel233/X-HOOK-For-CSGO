#include "Features.h"


static std::deque<CIncomingSequence>sequences;
static int lastincomingsequencenumber;
void LagCompensation::UpdateIncomingSequences()
{
	auto clientState = *reinterpret_cast<uintptr_t*>(uintptr_t(GetModuleHandle("engine.dll")) + 0x588B34); //DONT HARDCODE OFFESTS


	if (!clientState)
		return;

	auto intnetchan = *reinterpret_cast<uintptr_t*>(clientState + 0x9C); //Can optimise, already done in CM hook, make a global

	INetChannel* netchan = reinterpret_cast<INetChannel*>(intnetchan);
	if (netchan)
	{
		//if (LocalPlayer.Real_m_nInSequencenumber == 0)
		//{
		//	LocalPlayer.Real_m_nInSequencenumber = netchan->m_nInSequenceNr;
		//	lastincomingsequencenumber = netchan->m_nInSequenceNr;
		//	//LocalPlayer.ModifiedSequenceNr = false;
		//}

		if (netchan->m_nInSequenceNr > lastincomingsequencenumber)
		{
			//sequences.push_front(netchan->m_nInSequenceNr);
			lastincomingsequencenumber = netchan->m_nInSequenceNr;

			sequences.push_front(CIncomingSequence(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, pGlobalVars->realtime));
		}

		if (sequences.size() > 2048)
			sequences.pop_back();
	}
}

void LagCompensation::ClearIncomingSequences()
{
	m_last_incoming_sequence_number = 0;
	sequences.clear();
}

//Adds latency to the local player
void LagCompensation::AddLatencyToNetchan(INetChannel *netchan, float Latency)
{
	for (auto& seq : sequences)
	{
		if (pGlobalVars->realtime - seq.curtime >= Latency)
		{
			netchan->m_nInReliableState = seq.inreliablestate;
			//netchan->m_nOutReliableState = seq.outreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}
}