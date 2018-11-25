#pragma once
class IHandleEntity;
struct Ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
class ICollideable;
class ITraceListData;
class CPhysCollide;
struct cplane_t
{
	Vector normal;
	float dist;
	unsigned char type;
	unsigned char signbits;
	unsigned char pad[2];
};
struct virtualmeshlist_t;

enum class TraceType
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask) = 0;
	virtual TraceType GetTraceType() const = 0;
};


//-----------------------------------------------------------------------------
// Classes are expected to inherit these + implement the ShouldHitEntity method
//-----------------------------------------------------------------------------

// This is the one most normal traces will inherit from
class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_ENTITIES_ONLY;
	}
};




class CSimpleTraceFilter : public ITraceFilter
{
public:
	CSimpleTraceFilter(PVOID pEnt)
	{
		m_pPassEnt = pEnt;
	}

	virtual bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask)
	{
		return pHandleEntity != m_pPassEnt;
	}

	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}

	PVOID m_pPassEnt;
};



class CTraceFilterNoPlayer : public CTraceFilter
{
public:
	CTraceFilterNoPlayer() {}
	virtual bool ShouldHitEntity(IHandleEntity *pServerEntity, C_BasePlayer* pEnt, int contentsMask)
	{
	//	IClientUnknown *pUnk = static_cast<IClientUnknown*>(pServerEntity);
		/*C_BaseEntity *pEnt;
		return !(pEnt->GetTeam2() > 0);*/
		return !(pEnt->GetTeam() > TeamID::TEAM_UNASSIGNED);
	}
};
//-----------------------------------------------------------------------------
// Classes need not inherit from these
//-----------------------------------------------------------------------------
class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_WORLD_ONLY;
	}
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return true;
	}
};
class CTraceWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType GetTraceType() const
	{
		return TraceType::TRACE_WORLD_ONLY;
	}
};

enum class DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

//-----------------------------------------------------------------------------
// Enumeration interface for EnumerateLinkEntities
//-----------------------------------------------------------------------------
class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity(IHandleEntity *pHandleEntity) = 0;
};


struct BrushSideInfo_t
{
//	Vector4D plane;               // The plane of the brush side
	unsigned short bevel;    // Bevel plane?
	unsigned short thin;     // Thin?
};

class CPhysCollide;

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide   **solids;
	char           *pKeyValues;
	void           *pUserData;
};

struct cmodel_t
{
	Vector         mins, maxs;
	Vector         origin;        // for sounds or lights
	int            headnode;
	vcollide_t     vcollisionData;
};

struct csurface_t
{
	const char     *name;
	short          surfaceProps;
	unsigned short flags;         // BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

//-----------------------------------------------------------------------------
// A ray...
//-----------------------------------------------------------------------------
struct Ray_t
{
	VectorAligned  m_Start;  // starting point, centered within the extents
	VectorAligned  m_Delta;  // direction + length of the ray
	VectorAligned  m_StartOffset; // Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;     // Describes an axis aligned box extruded along a ray
	const matrix3x4_t *m_pWorldAxisTransform;
	bool m_IsRay;  // are the extents zero?
	bool m_IsSwept;     // is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& start, Vector const& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Init();
		VectorCopy(start, m_Start);
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis) {
			if (m_Delta[iAxis] != 0.0f) {
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else {
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}

private:
};

class CBaseTrace
{
public:
	/*
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }
	*/
public:

	// these members are aligned!!
	Vector         startpos;                // start position
	Vector         endpos;                       // final position
	cplane_t       plane;                        // surface normal at impact

	float          fraction;                // time completed, 1.0 = didn't hit anything

	int            contents;                // contents on other side of surface hit
	unsigned short dispFlags;                    // displacement flags for marking surfaces with data

	bool           allsolid;                // if true, plane is not valid
	bool           startsolid;                   // if true, the initial point was in a solid area

	CBaseTrace() {}

};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;

public:

	float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
	csurface_t          surface;            // surface hit (impact surface)
	HitGroups                 hitgroup;           // 0 == generic, non-zero is specific body part
	short               physicsbone;        // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
	IClientEntity*      m_pEnt;
	int                 hitbox;                       // box hit by trace in studio

	CGameTrace()
	{ }

private:

	CGameTrace(const CGameTrace& vOther);
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}

class IEngineTrace
{
public:
	virtual int         GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
	virtual int         GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int         GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void        ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, CGameTrace *pTrace) = 0;
	virtual void        ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, CGameTrace *pTrace) = 0;
	virtual void        TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
};