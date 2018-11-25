#pragma once

struct surfacephysicsparams_t {
	// vphysics physical properties
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct surfaceaudioparams_t
{
	float    reflectivity;             // like elasticity, but how much sound should be reflected by this surface
	float    hardnessFactor;           // like elasticity, but only affects impact sound choices
	float    roughnessFactor;          // like friction, but only affects scrape sound choices   
	float    roughThreshold;           // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float    hardThreshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float    hardVelocityThreshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
	float    highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float    midPitchOcclusion;
	float    lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	unsigned short    walkStepLeft;
	unsigned short    walkStepRight;
	unsigned short	  runStepLeft;
	unsigned short	  runStepRight;
	unsigned short    impactSoft;
	unsigned short    impactHard;
	unsigned short    scrapeSmooth;
	unsigned short    scrapeRough;
	unsigned short    bulletImpact;
	unsigned short    rolling;
	unsigned short    breakSound;
	unsigned short    strainSound;
};

struct surfacegameprops_t
{
	float    maxSpeedFactor;
	float    jumpFactor;
	float    flPenetrationModifier;
	float    flDamageModifier;
	unsigned short    material;
	byte climbable;
	char pad00[0x4];
};

struct surfacedata_t
{
	surfacephysicsparams_t    physics;
	surfaceaudioparams_t    audio;
	surfacesoundnames_t        sounds;
	surfacegameprops_t        game;
};

class IPhysicsSurfaceProps
{
public:
	virtual ~IPhysicsSurfaceProps(void) {}

	// parses a text file containing surface prop keys
	virtual int ParseSurfaceData(const char *pFilename, const char *pTextfile) = 0;

	// current number of entries in the database
	virtual int SurfacePropCount(void) const = 0;

	virtual int GetSurfaceIndex(const char *pSurfacePropName) const = 0;

	virtual void GetPhysicsProperties(int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity) const = 0;

	// lazy way
//	virtual surfacedata_t *GetSurfaceData(int surfaceDataIndex) = 0;

	// Hook
	surfacedata_t	*GetSurfaceData(int surfaceDataIndex)
	{
		typedef surfacedata_t*(__thiscall* fnGetSurfaceData)(void*, int);
		return getvfunc<fnGetSurfaceData>(this, 5)(this, surfaceDataIndex);
	}

	virtual const char *GetString(unsigned short stringTableIndex) const = 0;

	virtual const char *GetPropName(int surfaceDataIndex) const = 0;

	// sets the global index table for world materials
	// UNDONE: Make this per-CPhysCollide
	virtual void SetWorldMaterialIndexTable(int *pMapArray, int mapSize) = 0;

	// NOTE: Same as GetPhysicsProperties, but maybe more convenient
	virtual void GetPhysicsParameters(int surfaceDataIndex, surfacephysicsparams_t *pParamsOut) const = 0;
};