#ifndef BASEANIMATING_H
#define BASEANIMATING_H
#ifdef _WIN32
#pragma once
#endif


struct animevent_t;
struct matrix3x4_t;
class CIKContext;
class KeyValues;

#define	BCF_NO_ANIMATION_SKIP	( 1 << 0 )	// Do not allow PVS animation skipping (mostly for attachments being critical to an entity)
#define	BCF_IS_IN_SPAWN			( 1 << 1 )	// Is currently inside of spawn, always evaluate animations

class CBaseAnimating : public C_BaseEntity
{
public:

	CBaseAnimating();
	~CBaseAnimating();

	enum
	{
		NUM_POSEPAREMETERS = 24,
		NUM_BONECTRLS = 4
	};

	virtual CBaseAnimating*	GetBaseAnimating() { return this; }

private:

	// FIXME: necessary so that cyclers can hack m_bSequenceFinished
	friend class CFlexCycler;
	friend class CCycler;
	friend class CBlendingCycler;
};



#define ANIMATION_SEQUENCE_BITS			12	// 4096 sequences
#define ANIMATION_SKIN_BITS				10	// 1024 body skin selections FIXME: this seems way high
#define ANIMATION_BODY_BITS				32	// body combinations
#define ANIMATION_HITBOXSET_BITS		2	// hit box sets
#if defined( TF_DLL )
#define ANIMATION_POSEPARAMETER_BITS	8	// pose parameter resolution
#else
#define ANIMATION_POSEPARAMETER_BITS	11	// pose parameter resolution
#endif
#define ANIMATION_PLAYBACKRATE_BITS		8	// default playback rate, only used on leading edge detect sequence changes

#endif // BASEANIMATING_H
